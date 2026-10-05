#pragma once

// ComfoAir serial framing, free of ESPHome dependencies so it can be unit-tested on the host.
//
// Wire format: 07 F0 00 <cmd> <len> <data...> <checksum> 07 0F
// Every 0x07 between <cmd> and <checksum> (inclusive) is sent doubled; the checksum
// counts it once. An acknowledgement is the bare pair 07 F3.

#include <cstddef>
#include <cstdint>

#include "registers.h"

namespace esphome
{
  namespace comfoair
  {
    namespace frame
    {

      static constexpr size_t MAX_FRAME = 64;
      static constexpr size_t MAX_DATA = MAX_FRAME - COMMAND_LEN_HEAD - 1;
      static constexpr size_t MAX_ENCODED = 3 + 2 * (2 + MAX_DATA + 1) + 2;

      inline uint8_t checksum(uint8_t command, uint8_t length, const uint8_t *data)
      {
        uint16_t sum = 173 + command + length;
        for (uint8_t i = 0; i < length; i++)
          sum += data[i];
        return static_cast<uint8_t>(sum & 0xFF);
      }

      // Writes the wire bytes into out (room for MAX_ENCODED) and returns their count,
      // or 0 when length exceeds MAX_DATA.
      inline size_t encode(uint8_t command, const uint8_t *data, uint8_t length, uint8_t *out)
      {
        if (length > MAX_DATA)
          return 0;

        size_t n = 0;
        auto put_escaped = [&](uint8_t value)
        {
          out[n++] = value;
          if (value == COMMAND_PREFIX)
            out[n++] = value;
        };

        out[n++] = COMMAND_PREFIX;
        out[n++] = COMMAND_HEAD;
        out[n++] = 0x00;
        put_escaped(command);
        put_escaped(length);
        for (uint8_t i = 0; i < length; i++)
          put_escaped(data[i]);
        put_escaped(checksum(command, length, data));
        out[n++] = COMMAND_PREFIX;
        out[n++] = COMMAND_TAIL;
        return n;
      }

      enum class Result
      {
        PENDING,
        FRAME,
        ACK,
        CHECKSUM_ERROR,
        FRAMING_ERROR,
      };

      class Decoder
      {
      public:
        // Feed one wire byte. After FRAME (or CHECKSUM_ERROR) the accessors below describe
        // the frame until the next byte is fed.
        Result feed(uint8_t byte)
        {
          if (escape_pending_)
          {
            escape_pending_ = false;
            if (byte == COMMAND_PREFIX)
              return Result::PENDING;
            // A lone 0x07 inside the body: the frame is broken, but the 0x07 may have
            // been the start of the next one.
            index_ = 0;
            if (byte == COMMAND_HEAD)
            {
              buf_[0] = COMMAND_PREFIX;
              buf_[1] = COMMAND_HEAD;
              index_ = 2;
            }
            return Result::FRAMING_ERROR;
          }

          switch (index_)
          {
          case 0:
            if (byte == COMMAND_PREFIX)
              buf_[index_++] = byte;
            return Result::PENDING;
          case 1:
            if (byte == COMMAND_ACK)
            {
              index_ = 0;
              return Result::ACK;
            }
            if (byte == COMMAND_HEAD)
            {
              buf_[index_++] = byte;
              return Result::PENDING;
            }
            if (byte != COMMAND_PREFIX)
              index_ = 0;
            return Result::PENDING;
          case 2:
            if (byte != 0x00)
              return fail_();
            buf_[index_++] = byte;
            return Result::PENDING;
          default:
            break;
          }

          if (index_ == COMMAND_IDX_DATA && byte > MAX_DATA)
            return fail_();

          if (index_ <= COMMAND_IDX_DATA || index_ <= checksum_index_())
          {
            buf_[index_++] = byte;
            escape_pending_ = (byte == COMMAND_PREFIX);
            return Result::PENDING;
          }

          if (index_ == checksum_index_() + 1)
          {
            if (byte != COMMAND_PREFIX)
              return fail_();
            index_++;
            return Result::PENDING;
          }

          if (byte != COMMAND_TAIL)
            return fail_();
          index_ = 0;
          return received_checksum() == expected_checksum() ? Result::FRAME : Result::CHECKSUM_ERROR;
        }

        // Unescaped frame laid out as 07 F0 00 <cmd> <len> <data...> <checksum>.
        const uint8_t *raw() const { return buf_; }
        uint8_t command() const { return buf_[COMMAND_IDX_MSG_ID]; }
        uint8_t length() const { return buf_[COMMAND_IDX_DATA]; }
        const uint8_t *data() const { return buf_ + COMMAND_LEN_HEAD; }
        uint8_t received_checksum() const { return buf_[checksum_index_()]; }
        uint8_t expected_checksum() const { return checksum(command(), length(), data()); }

      private:
        size_t checksum_index_() const { return COMMAND_LEN_HEAD + length(); }

        Result fail_()
        {
          index_ = 0;
          return Result::FRAMING_ERROR;
        }

        uint8_t buf_[MAX_FRAME]{};
        size_t index_{0};
        bool escape_pending_{false};
      };

    } // namespace frame
  } // namespace comfoair
} // namespace esphome
