#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>

#include "frame.h"

using namespace esphome::comfoair::frame;
using Bytes = std::vector<uint8_t>;

namespace
{
  Bytes encode_bytes(uint8_t command, const Bytes &data)
  {
    uint8_t out[MAX_ENCODED];
    size_t n = encode(command, data.data(), static_cast<uint8_t>(data.size()), out);
    return Bytes(out, out + n);
  }

  std::vector<Result> feed_all(Decoder &decoder, const Bytes &wire)
  {
    std::vector<Result> results;
    for (uint8_t byte : wire)
    {
      Result r = decoder.feed(byte);
      if (r != Result::PENDING)
        results.push_back(r);
    }
    return results;
  }

  Bytes payload(const Decoder &decoder)
  {
    return Bytes(decoder.data(), decoder.data() + decoder.length());
  }
}

// Golden bytes captured from the pre-frame.h write_command_ implementation.
TEST_CASE("encode matches the legacy writer byte for byte")
{
  CHECK(encode_bytes(0x0B, {}) == Bytes{0x07, 0xF0, 0x00, 0x0B, 0x00, 0xB8, 0x07, 0x0F});
  CHECK(encode_bytes(0x99, {2}) == Bytes{0x07, 0xF0, 0x00, 0x99, 0x01, 0x02, 0x49, 0x07, 0x0F});
  CHECK(encode_bytes(0x99, {7}) == Bytes{0x07, 0xF0, 0x00, 0x99, 0x01, 0x07, 0x07, 0x4E, 0x07, 0x0F});
  CHECK(encode_bytes(0x9B, {3}) == Bytes{0x07, 0xF0, 0x00, 0x9B, 0x01, 0x03, 0x4C, 0x07, 0x0F});
  CHECK(encode_bytes(0xCF, {0, 30, 50, 0, 40, 60, 65, 75, 0}) ==
        Bytes{0x07, 0xF0, 0x00, 0xCF, 0x09, 0x00, 0x1E, 0x32, 0x00, 0x28, 0x3C, 0x41, 0x4B, 0x00, 0xC5, 0x07, 0x0F});
  CHECK(encode_bytes(0x99, {0xC0}) == Bytes{0x07, 0xF0, 0x00, 0x99, 0x01, 0xC0, 0x07, 0x07, 0x07, 0x0F});
}

TEST_CASE("encode refuses oversized payloads")
{
  uint8_t out[MAX_ENCODED];
  Bytes data(MAX_DATA + 1, 0);
  CHECK(encode(0x99, data.data(), static_cast<uint8_t>(data.size()), out) == 0);
}

// Exhaust fan at ~1025 RPM: raw period 0x0724 puts a doubled 0x07 in the data.
// The legacy reader rejected exactly this frame every poll cycle (2026-10-05).
TEST_CASE("fan status frame with a doubled 0x07 in the data decodes")
{
  Decoder decoder;
  Bytes wire{0x07, 0xF0, 0x00, 0x0C, 0x06, 0x28, 0x1E, 0x05, 0x96, 0x07, 0x07, 0x24, 0xCB, 0x07, 0x0F};

  CHECK(feed_all(decoder, wire) == std::vector<Result>{Result::FRAME});
  CHECK(decoder.command() == 0x0C);
  CHECK(payload(decoder) == Bytes{0x28, 0x1E, 0x05, 0x96, 0x07, 0x24});
}

TEST_CASE("checksum equal to 0x07 right before the tail")
{
  Decoder decoder;
  CHECK(feed_all(decoder, {0x07, 0xF0, 0x00, 0x99, 0x01, 0xC0, 0x07, 0x07, 0x07, 0x0F}) ==
        std::vector<Result>{Result::FRAME});
  CHECK(payload(decoder) == Bytes{0xC0});
}

TEST_CASE("data length equal to 0x07")
{
  Bytes data{1, 2, 3, 4, 5, 6, 8};
  Decoder decoder;
  CHECK(feed_all(decoder, encode_bytes(0x0C, data)) == std::vector<Result>{Result::FRAME});
  CHECK(decoder.length() == 7);
  CHECK(payload(decoder) == data);
}

TEST_CASE("consecutive 0x07 data bytes")
{
  Bytes data{0x07, 0x07, 0x07};
  Decoder decoder;
  CHECK(feed_all(decoder, encode_bytes(0x0C, data)) == std::vector<Result>{Result::FRAME});
  CHECK(payload(decoder) == data);
}

TEST_CASE("acknowledgement is reported as ACK, not a frame")
{
  Decoder decoder;
  CHECK(feed_all(decoder, {0x07, 0xF3}) == std::vector<Result>{Result::ACK});
}

TEST_CASE("bad checksum is reported and the next frame still decodes")
{
  Decoder decoder;
  Bytes bad{0x07, 0xF0, 0x00, 0x99, 0x01, 0x02, 0x48, 0x07, 0x0F};
  CHECK(feed_all(decoder, bad) == std::vector<Result>{Result::CHECKSUM_ERROR});
  CHECK(decoder.received_checksum() == 0x48);
  CHECK(decoder.expected_checksum() == 0x49);

  CHECK(feed_all(decoder, encode_bytes(0x99, {2})) == std::vector<Result>{Result::FRAME});
}

TEST_CASE("garbage before the frame is skipped")
{
  Bytes wire{0x00, 0x42, 0x0F, 0x07, 0x55};
  Bytes frame = encode_bytes(0x0C, {1, 2});
  wire.insert(wire.end(), frame.begin(), frame.end());

  Decoder decoder;
  CHECK(feed_all(decoder, wire) == std::vector<Result>{Result::FRAME});
  CHECK(payload(decoder) == Bytes{1, 2});
}

TEST_CASE("lone 0x07 inside the body resynchronises on a new frame start")
{
  Bytes wire{0x07, 0xF0, 0x00, 0x0C, 0x02, 0x07};
  Bytes frame = encode_bytes(0x0C, {9, 9});
  wire.insert(wire.end(), frame.begin() + 1, frame.end());

  Decoder decoder;
  CHECK(feed_all(decoder, wire) == std::vector<Result>{Result::FRAMING_ERROR, Result::FRAME});
  CHECK(payload(decoder) == Bytes{9, 9});
}

TEST_CASE("broken tail is a framing error")
{
  Decoder decoder;
  CHECK(feed_all(decoder, {0x07, 0xF0, 0x00, 0x99, 0x01, 0x02, 0x49, 0x07, 0x0E}) ==
        std::vector<Result>{Result::FRAMING_ERROR});
}

TEST_CASE("oversized length is a framing error")
{
  Decoder decoder;
  CHECK(feed_all(decoder, {0x07, 0xF0, 0x00, 0x0C, static_cast<uint8_t>(MAX_DATA + 1)}) ==
        std::vector<Result>{Result::FRAMING_ERROR});
}

TEST_CASE("round trip for every single-byte payload")
{
  Decoder decoder;
  for (int value = 0; value <= 0xFF; value++)
  {
    Bytes data{static_cast<uint8_t>(value)};
    CAPTURE(value);
    CHECK(feed_all(decoder, encode_bytes(0x99, data)) == std::vector<Result>{Result::FRAME});
    CHECK(payload(decoder) == data);
  }
}
