#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/climate/climate_mode.h"
#include "esphome/components/climate/climate_traits.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/number/number.h"
#include "esphome/components/button/button.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "registers.h"

namespace esphome
{
  namespace comfoair
  {

    static const char *TAG = "comfoair";

    class ComfoAirComponent;

    class ComfoAirSizeSelect : public select::Select
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void control(const std::string &value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirVentilationLevelNumber : public number::Number
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }
      void set_level_index(uint8_t index) { this->level_index_ = index; }

    protected:
      void control(float value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
      uint8_t level_index_{0};
    };

    class ComfoAirTimeDelayNumber : public number::Number
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }
      void set_delay_index(uint8_t index) { this->delay_index_ = index; }

    protected:
      void control(float value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
      uint8_t delay_index_{0};
    };

    class ComfoAirResetErrorsButton : public button::Button
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void press_action() override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirStartSelfTestButton : public button::Button
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void press_action() override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirPreheaterSwitch : public switch_::Switch
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void write_state(bool state) override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirRS232ModeSelect : public select::Select
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void control(const std::string &value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirFanModeSelect : public select::Select
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void control(const std::string &value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirTestModeSwitch : public switch_::Switch
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }

    protected:
      void write_state(bool state) override;

    private:
      ComfoAirComponent *parent_{nullptr};
    };

    class ComfoAirTestFanSpeedNumber : public number::Number
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }
      void set_is_supply_fan(bool is_supply) { this->is_supply_fan_ = is_supply; }

    protected:
      void control(float value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
      bool is_supply_fan_{true};
    };

    class ComfoAirTestOutputSwitch : public switch_::Switch
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }
      void set_output_mask(uint8_t mask, bool is_feedback)
      {
        this->output_mask_ = mask;
        this->is_feedback_ = is_feedback;
      }

    protected:
      void write_state(bool state) override;

    private:
      ComfoAirComponent *parent_{nullptr};
      uint8_t output_mask_{0};
      bool is_feedback_{false};
    };

    class ComfoAirTestFlapSelect : public select::Select
    {
    public:
      void set_parent(ComfoAirComponent *parent) { this->parent_ = parent; }
      void set_is_bypass_flap(bool is_bypass) { this->is_bypass_flap_ = is_bypass; }

    protected:
      void control(const std::string &value) override;

    private:
      ComfoAirComponent *parent_{nullptr};
      bool is_bypass_flap_{true};
    };

    class ComfoAirComponent : public climate::Climate, public PollingComponent, public uart::UARTDevice
    {
      friend class ComfoAirSizeSelect;
      friend class ComfoAirVentilationLevelNumber;
      friend class ComfoAirTimeDelayNumber;
      friend class ComfoAirResetErrorsButton;
      friend class ComfoAirStartSelfTestButton;
      friend class ComfoAirPreheaterSwitch;
      friend class ComfoAirRS232ModeSelect;
      friend class ComfoAirFanModeSelect;
      friend class ComfoAirTestModeSwitch;
      friend class ComfoAirTestFanSpeedNumber;
      friend class ComfoAirTestOutputSwitch;
      friend class ComfoAirTestFlapSelect;

    public:
      // Poll every 600ms
      ComfoAirComponent() : Climate(),
                            PollingComponent(600),
                            UARTDevice() {}

      /// Return the traits of this controller.
      climate::ClimateTraits traits() override
      {
        auto traits = climate::ClimateTraits();
        traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
        traits.set_supported_modes({climate::CLIMATE_MODE_FAN_ONLY});
        traits.clear_feature_flags(climate::CLIMATE_REQUIRES_TWO_POINT_TARGET_TEMPERATURE);
        traits.clear_feature_flags(climate::CLIMATE_SUPPORTS_ACTION);
        traits.set_visual_min_temperature(12);
        traits.set_visual_max_temperature(29);
        traits.set_visual_temperature_step(1);
        traits.set_supported_fan_modes({
            climate::CLIMATE_FAN_LOW,
            climate::CLIMATE_FAN_MEDIUM,
            climate::CLIMATE_FAN_HIGH,
            climate::CLIMATE_FAN_OFF,
        });
        return traits;
      }

      /// Override control to change settings of the climate device.
      void control(const climate::ClimateCall &call) override
      {
        if (call.get_fan_mode().has_value())
        {
          int level;

          fan_mode = *call.get_fan_mode();
          switch (fan_mode.value())
          {
          case climate::CLIMATE_FAN_HIGH:
            level = 0x04;
            break;
          case climate::CLIMATE_FAN_MEDIUM:
            level = 0x03;
            break;
          case climate::CLIMATE_FAN_LOW:
            level = 0x02;
            break;
          case climate::CLIMATE_FAN_OFF:
            level = 0x01;
            break;
          case climate::CLIMATE_FAN_ON:
          case climate::CLIMATE_FAN_MIDDLE:
          case climate::CLIMATE_FAN_DIFFUSE:
          default:
            level = -1;
            break;
          }

          if (level >= 0)
          {
            set_level_(level);
          }
        }
        if (call.get_target_temperature().has_value())
        {
          target_temperature = *call.get_target_temperature();
          set_comfort_temperature_(target_temperature);
        }

        publish_state();
      }

      void dump_config() override
      {
        uint8_t *p;
        ESP_LOGCONFIG(TAG, "ComfoAir:");
        // LOG_UPDATE_INTERVAL(this);
        p = bootloader_version_;
        ESP_LOGCONFIG(TAG, "  Bootloader %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1), *(p + 2));
        p = firmware_version_;
        ESP_LOGCONFIG(TAG, "  Firmware %.10s v%0d.%02d b%2d", p + 3, *p, *(p + 1), *(p + 2));
        p = connector_board_version_;
        ESP_LOGCONFIG(TAG, "  Connector Board %.10s v%0d.%02d", p + 2, *p, *(p + 1));

        if (*(p + 12) != 0)
        {
          ESP_LOGCONFIG(TAG, "  CC-Ease v%0d.%02d", *(p + 12) >> 4, *(p + 12) & 0x0f);
        }
        if (*(p + 13) != 0)
        {
          ESP_LOGCONFIG(TAG, "  CC-Luxe v%0d.%02d", *(p + 13) >> 4, *(p + 13) & 0x0f);
        }
        check_uart_settings(9600);
      }

      void update() override
      {
        switch (update_counter_)
        {
        case -4:
          write_command_(CMD_GET_BOOTLOADER_VERSION, nullptr, 0);
          break;
        case -3:
          write_command_(CMD_GET_FIRMWARE_VERSION, nullptr, 0);
          break;
        case -2:
          write_command_(CMD_GET_CONNECTOR_BOARD_VERSION, nullptr, 0);
          break;
        case -1:
          write_command_(CMD_GET_STATUS, nullptr, 0);
          break;
        case 0:
          get_fan_status_();
          break;
        case 1:
          get_valve_status_();
          break;
        case 2:
          get_sensor_data_();
          break;
        case 3:
          get_ventilation_level_();
          break;
        case 4:
          get_temperatures_();
          break;
        case 5:
          get_error_status_();
          break;
        case 6:
          get_bypass_control_status_();
          break;
        case 7:
          get_operation_hours_();
          break;
        case 8:
          get_preheating_status_();
          break;
        case 9:
          get_time_delay_();
          break;
        case 10:
          get_inputs_();
          break;
        case 11:
          get_analog_inputs_();
          break;
        }

        update_counter_++;
        if (update_counter_ > num_update_counter_elements_)
          update_counter_ = 0;
      }

      void loop() override
      {
        while (available() != 0)
        {
          read_byte(&data_[data_index_]);
          auto check = check_byte_();
          if (!check.has_value())
          {

            // finished
            if (data_[COMMAND_ID_ACK] != COMMAND_ACK)
            {
              parse_data_();
            }
            data_index_ = 0;
          }
          else if (!*check)
          {
            // wrong data
            ESP_LOGV(TAG, "Byte %i of received data frame is invalid.", data_index_);
            data_index_ = 0;
          }
          else
          {
            // next byte
            data_index_++;
          }
        }
      }

      float get_setup_priority() const override { return setup_priority::DATA; }

      void error_reset(void)
      {
        uint8_t reset_cmd[4] = {1, 0, 0, 0};
        write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
      }

      void filter_reset(void)
      {
        uint8_t reset_cmd[4] = {0, 0, 0, 1};
        write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
      }

      void setup() override
      {
        // Ensure test mode is exited on boot
        test_mode_active_ = false;
        test_relay_state_ = 0;
        test_feedback_state_ = 0;
        bypass_flap_state_ = 0;
        preheating_flap_state_ = 0;
      }

      void set_test_mode(bool active)
      {
        if (active)
        {
          ESP_LOGI(TAG, "Entering ComfoAir test mode");
          write_command_(CMD_SET_TEST_MODE, nullptr, 0);
        }
        else
        {
          ESP_LOGI(TAG, "Exiting ComfoAir test mode");
          write_command_(CMD_EXIT_TEST_MODE, nullptr, 0);
        }
        test_mode_active_ = active;

        // Update test mode switch state
        if (test_mode_switch_ != nullptr)
        {
          test_mode_switch_->publish_state(active);
        }
      }

      bool set_test_fan_speed(bool is_supply, uint8_t percent)
      {
        if (!test_mode_active_)
        {
          ESP_LOGW(TAG, "Test mode command rejected - test mode not active");
          return false;
        }

        if (percent > 100)
        {
          ESP_LOGW(TAG, "Invalid fan speed: %u%%", percent);
          return false;
        }

        if (is_supply)
        {
          test_supply_fan_speed_ = percent;
        }
        else
        {
          test_exhaust_fan_speed_ = percent;
        }

        // CMD_SET_ANALOG_OUTPUTS: Byte[0]=supply%, Byte[1]=exhaust%, Byte[2]=postheating% (always 0)
        uint8_t command[3] = {test_supply_fan_speed_, test_exhaust_fan_speed_, 0};
        ESP_LOGI(TAG, "Setting test fan speeds: supply=%u%%, exhaust=%u%%", test_supply_fan_speed_, test_exhaust_fan_speed_);
        write_command_(CMD_SET_ANALOG_OUTPUTS, command, sizeof(command));
        return true;
      }

      bool set_test_output(uint8_t mask, bool is_feedback, bool state)
      {
        if (!test_mode_active_)
        {
          ESP_LOGW(TAG, "Test mode command rejected - test mode not active");
          return false;
        }

        if (is_feedback)
        {
          if (state)
          {
            test_feedback_state_ |= mask;
          }
          else
          {
            test_feedback_state_ &= ~mask;
          }
        }
        else
        {
          if (state)
          {
            test_relay_state_ |= mask;
          }
          else
          {
            test_relay_state_ &= ~mask;
          }
        }

        // CMD_SET_OUTPUTS: Byte[0]=relay state, Byte[1]=feedback state
        uint8_t command[2] = {test_relay_state_, test_feedback_state_};
        ESP_LOGI(TAG, "Setting test outputs: relays=0x%02X, feedback=0x%02X", test_relay_state_, test_feedback_state_);
        write_command_(CMD_SET_OUTPUTS, command, sizeof(command));
        return true;
      }

      bool set_test_flap(bool is_bypass, uint8_t position)
      {
        if (!test_mode_active_)
        {
          ESP_LOGW(TAG, "Test mode command rejected - test mode not active");
          return false;
        }

        // Valid positions: 0=Closed, 1=Open, 3=Stop
        if (position != 0 && position != 1 && position != 3)
        {
          ESP_LOGW(TAG, "Invalid flap position: %u", position);
          return false;
        }

        if (is_bypass)
        {
          bypass_flap_state_ = position;
        }
        else
        {
          preheating_flap_state_ = position;
        }

        // CMD_SET_FLAPS: Byte[0]=bypass, Byte[1]=preheating
        uint8_t command[2] = {bypass_flap_state_, preheating_flap_state_};
        const char *flap_name = is_bypass ? "bypass" : "preheating";
        const char *position_name = position == 0 ? "Closed" : (position == 1 ? "Open" : "Stop");
        ESP_LOGI(TAG, "Setting %s flap to %s (bypass=%u, preheating=%u)", flap_name, position_name, bypass_flap_state_, preheating_flap_state_);
        write_command_(CMD_SET_FLAPS, command, sizeof(command));
        return true;
      }

      void set_name(const char *value) { name = value; }
      void set_name(const char *value, uint32_t name_hash)
      {
        (void)name_hash;
        name = value;
      }
      void set_uart_component(uart::UARTComponent *parent) { set_uart_parent(parent); }
      bool set_unit_size(uint8_t raw_size);
      void set_size_select(ComfoAirSizeSelect *size_select);
      bool set_preheater_present(bool present);

    protected:
      void set_level_(int level)
      {
        if (level < 0 || level > 5)
        {
          ESP_LOGI(TAG, "Ignoring invalid level request: %i", level);
          return;
        }

        ESP_LOGI(TAG, "Setting ventilation level to: %i", level);
        {
          uint8_t command[1] = {(uint8_t)level};
          write_command_(CMD_SET_LEVEL, command, sizeof(command));
        }
      }

      void set_comfort_temperature_(float temperature)
      {
        if (temperature < 12.0f || temperature > 29.0f)
        {
          ESP_LOGI(TAG, "Ignoring invalid temperature request: %i", temperature);
          return;
        }

        ESP_LOGI(TAG, "Setting comfort temperature to: %.1f°C (raw value: %u)", temperature, (uint8_t)((temperature + 20.0f) * 2.0f));
        {
          uint8_t command[1] = {(uint8_t)((temperature + 20.0f) * 2.0f)};
          write_command_(CMD_SET_COMFORT_TEMPERATURE, command, sizeof(command));
        }
      }

      bool set_ventilation_level_percent(uint8_t level_index, uint8_t percent)
      {
        if (percent > 100)
        {
          ESP_LOGW(TAG, "Ignoring invalid ventilation percent: %u", percent);
          return false;
        }

        if (!ventilation_levels_valid_)
        {
          ESP_LOGW(TAG, "Ventilation levels cannot be changed before initial read");
          return false;
        }

        if (level_index > 7)
        {
          ESP_LOGW(TAG, "Invalid ventilation level index: %u", level_index);
          return false;
        }

        // Update cache
        ventilation_levels_[level_index] = percent;

        // Send command with all 8 levels (protocol requires all values)
        uint8_t command[9];
        // Order: exhaust absent, low, med, high, supply absent, low, med, high, unused byte
        command[0] = ventilation_levels_[4]; // exhaust absent
        command[1] = ventilation_levels_[5]; // exhaust low
        command[2] = ventilation_levels_[6]; // exhaust medium
        command[3] = ventilation_levels_[0]; // supply absent
        command[4] = ventilation_levels_[1]; // supply low
        command[5] = ventilation_levels_[2]; // supply medium
        command[6] = ventilation_levels_[7]; // exhaust high
        command[7] = ventilation_levels_[3]; // supply high
        command[8] = 0x00;

        const char *level_names[] = {"supply_absent", "supply_low", "supply_medium", "supply_high",
                                     "exhaust_absent", "exhaust_low", "exhaust_medium", "exhaust_high"};
        ESP_LOGI(TAG, "Setting ventilation level %s (index %u) to %u%% (all levels: SA=%u SL=%u SM=%u SH=%u EA=%u EL=%u EM=%u EH=%u)",
                 level_names[level_index], level_index, percent,
                 ventilation_levels_[0], ventilation_levels_[1], ventilation_levels_[2], ventilation_levels_[3],
                 ventilation_levels_[4], ventilation_levels_[5], ventilation_levels_[6], ventilation_levels_[7]);
        write_command_(CMD_SET_VENTILATION_LEVEL, command, sizeof(command));
        return true;
      }

      bool set_time_delay(uint8_t delay_index, uint8_t value)
      {
        if (!time_delays_valid_)
        {
          ESP_LOGW(TAG, "Time delays cannot be changed before initial read");
          return false;
        }

        if (delay_index > 7)
        {
          ESP_LOGW(TAG, "Invalid time delay index: %u", delay_index);
          return false;
        }

        // Update cache
        time_delays_[delay_index] = value;

        // Send command with all 8 values (protocol requires all values)
        uint8_t command[8];
        for (int i = 0; i < 8; i++)
        {
          command[i] = time_delays_[i];
        }

        const char *delay_names[] = {"bathroom_on", "bathroom_off", "l1_off", "boost",
                                     "filter_warning", "rf_short", "rf_long", "extractor_hood"};
        ESP_LOGI(TAG, "Setting time delay %s (index %u) to %u (all delays: %u,%u,%u,%u,%u,%u,%u,%u)",
                 delay_names[delay_index], delay_index, value,
                 time_delays_[0], time_delays_[1], time_delays_[2], time_delays_[3],
                 time_delays_[4], time_delays_[5], time_delays_[6], time_delays_[7]);
        write_command_(CMD_SET_TIME_DELAY, command, sizeof(command));
        return true;
      }

      void write_command_(const uint8_t command, const uint8_t *command_data, uint8_t command_data_length)
      {
        write_byte(COMMAND_PREFIX);
        write_byte(COMMAND_HEAD);
        write_byte(0x00);

        uint16_t checksum = 173;
        checksum += command;
        checksum += command_data_length;

        write_escaped_byte_(command);
        write_escaped_byte_(command_data_length);

        for (uint8_t i = 0; i < command_data_length; i++)
        {
          uint8_t data_byte = command_data[i];
          checksum += data_byte;
          write_escaped_byte_(data_byte);
        }

        uint8_t checksum_byte = static_cast<uint8_t>(checksum & 0xFF);
        write_escaped_byte_(checksum_byte);

        write_byte(COMMAND_PREFIX);
        write_byte(COMMAND_TAIL);
        flush();
      }

      void write_escaped_byte_(uint8_t value)
      {
        write_byte(value);
        if (value == COMMAND_PREFIX)
        {
          write_byte(value);
        }
      }

      uint8_t comfoair_checksum_(uint8_t command, uint8_t length, const uint8_t *command_data) const
      {
        uint16_t sum = 173;
        sum += command;
        sum += length;
        if (command_data != nullptr)
        {
          for (uint8_t i = 0; i < length; i++)
          {
            sum += command_data[i];
          }
        }
        return static_cast<uint8_t>(sum & 0xFF);
      }

      optional<bool> check_byte_() const
      {
        uint8_t index = data_index_;
        uint8_t byte = data_[index];

        if (index == 0)
        {
          return byte == COMMAND_PREFIX;
        }

        if (index == 1)
        {
          if (byte == COMMAND_ACK)
          {
            return {};
          }
          else
          {
            return byte == COMMAND_HEAD;
          }
        }

        if (index == 2)
        {
          return byte == 0x00;
        }

        if (index < COMMAND_LEN_HEAD)
        {
          return true;
        }

        uint8_t data_length = data_[COMMAND_IDX_DATA];

        if ((COMMAND_LEN_HEAD + data_length + COMMAND_LEN_TAIL) > sizeof(data_))
        {
          ESP_LOGW(TAG, "ComfoAir message too large");
          return false;
        }

        if (index < COMMAND_LEN_HEAD + data_length)
        {
          return true;
        }

        if (index == COMMAND_LEN_HEAD + data_length)
        {
          // checksum is without checksum bytes
          uint8_t checksum = comfoair_checksum_(
              data_[COMMAND_IDX_MSG_ID], data_length, data_ + COMMAND_LEN_HEAD);
          if (checksum != byte)
          {
            // ESP_LOGW(TAG, "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", data_[0], data_[1], data_[2], data_[3], data_[4], data_[5], data_[6], data_[7], data_[8], data_[9], data_[10]);
            ESP_LOGW(TAG, "ComfoAir Checksum doesn't match: 0x%02X!=0x%02X", byte, checksum);
            return false;
          }
          return true;
        }

        if (index == COMMAND_LEN_HEAD + data_length + 1)
        {
          return byte == COMMAND_PREFIX;
        }

        if (index == COMMAND_LEN_HEAD + data_length + 2)
        {
          if (byte != COMMAND_TAIL)
          {
            return false;
          }
        }

        return {};
      }

      void parse_data_()
      {
        status_clear_warning();
        uint8_t *msg = &data_[COMMAND_LEN_HEAD];

        switch (data_[COMMAND_IDX_MSG_ID])
        {
        case RES_GET_BOOTLOADER_VERSION:
          memcpy(bootloader_version_, msg, data_[COMMAND_IDX_DATA]);
          break;
        case RES_GET_FIRMWARE_VERSION:
          memcpy(firmware_version_, msg, data_[COMMAND_IDX_DATA]);
          break;
        case RES_GET_CONNECTOR_BOARD_VERSION:
          memcpy(connector_board_version_, msg, data_[COMMAND_IDX_DATA]);
          break;
        case RES_SET_TEST_MODE:
          ESP_LOGI(TAG, "Test mode activated");
          test_mode_active_ = true;
          if (test_mode_switch_ != nullptr)
          {
            test_mode_switch_->publish_state(true);
          }
          break;
        case RES_EXIT_TEST_MODE:
          ESP_LOGI(TAG, "Test mode deactivated");
          test_mode_active_ = false;
          if (test_mode_switch_ != nullptr)
          {
            test_mode_switch_->publish_state(false);
          }
          break;
        case RES_GET_FAN_STATUS:
        {
          if (intake_fan_speed != nullptr)
          {
            intake_fan_speed->publish_state(msg[0]);
          }
          if (exhaust_fan_speed != nullptr)
          {
            exhaust_fan_speed->publish_state(msg[1]);
          }
          if (intake_fan_speed_rpm != nullptr)
          {
            intake_fan_speed_rpm->publish_state(static_cast<int>(1875000.0f / get_uint16_(2)));
          }
          if (exhaust_fan_speed_rpm != nullptr)
          {
            exhaust_fan_speed_rpm->publish_state(static_cast<int>(1875000.0f / get_uint16_(4)));
          }
          break;
        }
        case RES_GET_VALVE_STATUS:
        {
          if (bypass_valve != nullptr)
          {
            bypass_valve->publish_state(msg[0]);
          }
          if (bypass_valve_open != nullptr)
          {
            bypass_valve_open->publish_state(msg[0] != 0);
          }
          if (preheating_state != nullptr)
          {
            preheating_state->publish_state(msg[1] != 0);
          }
          if (motor_current_bypass != nullptr)
          {
            motor_current_bypass->publish_state(msg[2]);
          }
          if (motor_current_preheating != nullptr)
          {
            motor_current_preheating->publish_state(msg[3]);
          }
          break;
        }
        case RES_GET_BYPASS_CONTROL_STATUS:
        {
          if (bypass_factor != nullptr)
          {
            bypass_factor->publish_state(msg[2]);
          }
          if (bypass_step != nullptr)
          {
            bypass_step->publish_state(msg[3]);
          }
          if (bypass_correction != nullptr)
          {
            bypass_correction->publish_state(msg[4]);
          }
          if (summer_mode != nullptr)
          {
            summer_mode->publish_state(msg[6] != 0);
          }
          break;
        }
        case RES_GET_TEMPERATURE_STATUS:
        {

          // T1 / outside air
          if (outside_air_temperature != nullptr)
          {
            outside_air_temperature->publish_state((float)msg[0] / 2.0f - 20.0f);
          }
          // T2 / supply air
          if (supply_air_temperature != nullptr)
          {
            supply_air_temperature->publish_state((float)msg[1] / 2.0f - 20.0f);
          }
          // T3 / return air
          if (return_air_temperature != nullptr)
          {
            return_air_temperature->publish_state((float)msg[2] / 2.0f - 20.0f);
          }
          // T4 / exhaust air
          if (exhaust_air_temperature != nullptr)
          {
            exhaust_air_temperature->publish_state((float)msg[3] / 2.0f - 20.0f);
          }
          break;
        }
        case RES_GET_SENSOR_DATA:
        {

          if (enthalpy_temperature != nullptr)
          {
            enthalpy_temperature->publish_state((float)msg[0] / 2.0f - 20.0f);
          }

          break;
        }
        case RES_GET_VENTILATION_LEVEL:
        {

          ESP_LOGD(TAG, "Level %02x", msg[8]);

          // Cache ventilation levels for write operations
          // msg[0] = exhaust absent, msg[1] = exhaust low, msg[2] = exhaust medium
          // msg[3] = supply absent, msg[4] = supply low, msg[5] = supply medium
          // msg[10] = exhaust high, msg[11] = supply high
          ventilation_levels_[0] = msg[3];  // supply absent
          ventilation_levels_[1] = msg[4];  // supply low
          ventilation_levels_[2] = msg[5];  // supply medium
          ventilation_levels_[3] = msg[11]; // supply high
          ventilation_levels_[4] = msg[0];  // exhaust absent
          ventilation_levels_[5] = msg[1];  // exhaust low
          ventilation_levels_[6] = msg[2];  // exhaust medium
          ventilation_levels_[7] = msg[10]; // exhaust high
          ventilation_levels_valid_ = true;

          // Publish to number components
          if (supply_absent_percent != nullptr)
            supply_absent_percent->publish_state(msg[3]);
          if (supply_low_percent != nullptr)
            supply_low_percent->publish_state(msg[4]);
          if (supply_medium_percent != nullptr)
            supply_medium_percent->publish_state(msg[5]);
          if (supply_high_percent != nullptr)
            supply_high_percent->publish_state(msg[11]);
          if (exhaust_absent_percent != nullptr)
            exhaust_absent_percent->publish_state(msg[0]);
          if (exhaust_low_percent != nullptr)
            exhaust_low_percent->publish_state(msg[1]);
          if (exhaust_medium_percent != nullptr)
            exhaust_medium_percent->publish_state(msg[2]);
          if (exhaust_high_percent != nullptr)
            exhaust_high_percent->publish_state(msg[10]);

          if (return_air_level != nullptr)
          {
            return_air_level->publish_state(msg[6]);
          }
          if (supply_air_level != nullptr)
          {
            supply_air_level->publish_state(msg[7]);
          }

          if (ventilation_level != nullptr)
          {
            ventilation_level->publish_state(msg[8] - 1);
          }

          // Fan Speed
          switch (msg[8])
          {
          case 0x00:
            fan_mode.reset();
            mode = climate::CLIMATE_MODE_FAN_ONLY;
            break;
          case 0x01:
            fan_mode = climate::CLIMATE_FAN_OFF;
            mode = climate::CLIMATE_MODE_OFF;
            break;
          case 0x02:
            fan_mode = climate::CLIMATE_FAN_LOW;
            mode = climate::CLIMATE_MODE_FAN_ONLY;
            break;
          case 0x03:
            fan_mode = climate::CLIMATE_FAN_MEDIUM;
            mode = climate::CLIMATE_MODE_FAN_ONLY;
            break;
          case 0x04:
            fan_mode = climate::CLIMATE_FAN_HIGH;
            mode = climate::CLIMATE_MODE_FAN_ONLY;
            break;
          }

          publish_state();

          // Supply air fan active (1 = active / 0 = inactive)
          if (supply_fan_active != nullptr)
          {
            supply_fan_active->publish_state(msg[9] == 1);
          }
          break;
        }
        case RES_GET_FAULTS:
        {
          if (filter_status != nullptr)
          {
            uint8_t status = msg[8];
            filter_status->publish_state(status == 0 ? "Ok" : (status == 1 ? "Full" : "Unknown"));
          }

          // Parse error codes according to protocol spec (CMD_GET_FAULTS response)
          // Byte[0] = Current error A (bits 0-7 = A1-A8)
          if (error_a1 != nullptr)
            error_a1->publish_state(msg[0] & 0x01);
          if (error_a2 != nullptr)
            error_a2->publish_state(msg[0] & 0x02);
          if (error_a3 != nullptr)
            error_a3->publish_state(msg[0] & 0x04);
          if (error_a4 != nullptr)
            error_a4->publish_state(msg[0] & 0x08);
          if (error_a5 != nullptr)
            error_a5->publish_state(msg[0] & 0x10);
          if (error_a6 != nullptr)
            error_a6->publish_state(msg[0] & 0x20);
          if (error_a7 != nullptr)
            error_a7->publish_state(msg[0] & 0x40);
          if (error_a8 != nullptr)
            error_a8->publish_state(msg[0] & 0x80);

          // Byte[1] = Current error E (bits 0-7 = E1-E8, but only E1-E2 defined)
          if (error_e1 != nullptr)
            error_e1->publish_state(msg[1] & 0x01);
          if (error_e2 != nullptr)
            error_e2->publish_state(msg[1] & 0x02);

          // Byte[9] = Current error EA (bits 0-7 = EA1-EA8)
          if (error_ea1 != nullptr)
            error_ea1->publish_state(msg[9] & 0x01);
          if (error_ea2 != nullptr)
            error_ea2->publish_state(msg[9] & 0x02);
          if (error_ea3 != nullptr)
            error_ea3->publish_state(msg[9] & 0x04);
          if (error_ea4 != nullptr)
            error_ea4->publish_state(msg[9] & 0x08);
          if (error_ea5 != nullptr)
            error_ea5->publish_state(msg[9] & 0x10);
          if (error_ea6 != nullptr)
            error_ea6->publish_state(msg[9] & 0x20);
          if (error_ea7 != nullptr)
            error_ea7->publish_state(msg[9] & 0x40);
          if (error_ea8 != nullptr)
            error_ea8->publish_state(msg[9] & 0x80);

          // Byte[13] = Current error A (high) (bits 0-6 = A9-A15, bit 7 = A0)
          if (error_a9 != nullptr)
            error_a9->publish_state(msg[13] & 0x01);
          if (error_a10 != nullptr)
            error_a10->publish_state(msg[13] & 0x02);
          if (error_a11 != nullptr)
            error_a11->publish_state(msg[13] & 0x04);
          if (error_a12 != nullptr)
            error_a12->publish_state(msg[13] & 0x08);
          if (error_a13 != nullptr)
            error_a13->publish_state(msg[13] & 0x10);
          if (error_a14 != nullptr)
            error_a14->publish_state(msg[13] & 0x20);
          if (error_a15 != nullptr)
            error_a15->publish_state(msg[13] & 0x40);
          if (error_a0 != nullptr)
            error_a0->publish_state(msg[13] & 0x80);

          break;
        }
        case RES_GET_TEMPERATURES:
        {

          // comfort temperature
          target_temperature = (float)msg[0] / 2.0f - 20.0f;
          publish_state();

          // T1 / outside air
          if (outside_air_temperature != nullptr && msg[5] & 0x01)
          {
            outside_air_temperature->publish_state((float)msg[1] / 2.0f - 20.0f);
          }
          // T2 / supply air
          if (supply_air_temperature != nullptr && msg[5] & 0x02)
          {
            supply_air_temperature->publish_state((float)msg[2] / 2.0f - 20.0f);
          }
          // T3 / exhaust air
          if (return_air_temperature != nullptr && msg[5] & 0x04)
          {
            return_air_temperature->publish_state((float)msg[3] / 2.0f - 20.0f);
            current_temperature = (float)msg[3] / 2.0f - 20.0f;
          }
          // T4 / continued air
          if (exhaust_air_temperature != nullptr && msg[5] & 0x08)
          {
            exhaust_air_temperature->publish_state((float)msg[4] / 2.0f - 20.0f);
          }
          // EWT
          if (ewt_temperature != nullptr && msg[5] & 0x10)
          {
            ewt_temperature->publish_state((float)msg[6] / 2.0f - 20.0f);
          }
          // postheating
          if (postheating_temperature != nullptr && msg[5] & 0x20)
          {
            postheating_temperature->publish_state((float)msg[7] / 2.0f - 20.0f);
          }
          // kitchen hood
          if (kitchen_hood_temperature != nullptr && msg[5] & 0x40)
          {
            kitchen_hood_temperature->publish_state((float)msg[8] / 2.0f - 20.0f);
          }

          break;
        }
        case RES_GET_STATUS:
        {
          if (preheating_present != nullptr)
          {
            preheating_present->publish_state(msg[0]);
          }

          // Also update the writable switch entity
          if (preheater_present_switch_ != nullptr)
          {
            preheater_present_switch_->publish_state(msg[0] != 0);
          }

          if (bypass_present != nullptr)
          {
            bypass_present->publish_state(msg[1]);
          }

          if (type != nullptr)
          {
            type->publish_state(msg[2] == 1 ? "Left" : (msg[2] == 2 ? "Right" : "Unknown"));
          }

          publish_size_entities_(msg[3]);

          if (options_present != nullptr)
          {
            options_present->publish_state(msg[4]);
          }

          if (fireplace_present != nullptr)
          {
            fireplace_present->publish_state(msg[4] & 0x01);
          }

          if (kitchen_hood_present != nullptr)
          {
            kitchen_hood_present->publish_state(msg[4] & 0x02);
          }

          if (postheating_present != nullptr)
          {
            postheating_present->publish_state(msg[4] & 0x04);
          }

          if (postheating_pwm_mode_present != nullptr)
          {
            postheating_pwm_mode_present->publish_state(msg[4] & 0x40);
          }

          if (p10_active != nullptr)
          {
            p10_active->publish_state(msg[6] & 0x01);
          }

          if (p11_active != nullptr)
          {
            p11_active->publish_state(msg[6] & 0x02);
          }

          if (p12_active != nullptr)
          {
            p12_active->publish_state(msg[6] & 0x04);
          }

          if (p13_active != nullptr)
          {
            p13_active->publish_state(msg[6] & 0x08);
          }

          if (p14_active != nullptr)
          {
            p14_active->publish_state(msg[6] & 0x10);
          }

          if (p15_active != nullptr)
          {
            p15_active->publish_state(msg[6] & 0x20);
          }

          if (p16_active != nullptr)
          {
            p16_active->publish_state(msg[6] & 0x40);
          }

          if (p17_active != nullptr)
          {
            p17_active->publish_state(msg[6] & 0x80);
          }

          if (p18_active != nullptr)
          {
            p18_active->publish_state(msg[7] & 0x01);
          }

          if (p19_active != nullptr)
          {
            p19_active->publish_state(msg[7] & 0x02);
          }

          if (p90_active != nullptr)
          {
            p90_active->publish_state(msg[8] & 0x01);
          }

          if (p91_active != nullptr)
          {
            p91_active->publish_state(msg[8] & 0x02);
          }

          if (p92_active != nullptr)
          {
            p92_active->publish_state(msg[8] & 0x04);
          }

          if (p93_active != nullptr)
          {
            p93_active->publish_state(msg[8] & 0x08);
          }

          if (p94_active != nullptr)
          {
            p94_active->publish_state(msg[8] & 0x10);
          }

          if (p95_active != nullptr)
          {
            p95_active->publish_state(msg[8] & 0x20);
          }

          if (p96_active != nullptr)
          {
            p96_active->publish_state(msg[8] & 0x40);
          }

          if (p97_active != nullptr)
          {
            p97_active->publish_state(msg[8] & 0x80);
          }

          if (enthalpy_present != nullptr)
          {
            enthalpy_present->publish_state(msg[9]);
          }

          if (ewt_present != nullptr)
          {
            ewt_present->publish_state(msg[10]);
          }

          if (data_[COMMAND_IDX_DATA] >= 11)
          {
            status_payload_[0] = msg[0];
            status_payload_[1] = msg[1];
            status_payload_[2] = msg[2];
            status_payload_[3] = msg[3];
            status_payload_[4] = msg[4];
            status_payload_[5] = msg[5];
            status_payload_[6] = msg[9];
            status_payload_[7] = msg[10];
            status_payload_valid_ = true;
          }
          else
          {
            status_payload_valid_ = false;
          }
          break;
        }
        case RES_GET_OPERATION_HOURS:
        {
          if (level0_hours != nullptr)
          {
            level0_hours->publish_state((msg[0] << 16) | (msg[1] << 8) | msg[2]);
          }

          if (level1_hours != nullptr)
          {
            level1_hours->publish_state((msg[3] << 16) | (msg[4] << 8) | msg[5]);
          }

          if (level2_hours != nullptr)
          {
            level2_hours->publish_state((msg[6] << 16) | (msg[7] << 8) | msg[8]);
          }

          if (level3_hours != nullptr)
          {
            level3_hours->publish_state((msg[17] << 16) | (msg[18] << 8) | msg[19]);
          }

          if (frost_protection_hours != nullptr)
          {
            frost_protection_hours->publish_state((msg[9] << 8) | msg[10]);
          }

          if (bypass_open_hours != nullptr)
          {
            bypass_open_hours->publish_state((msg[13] << 8) | msg[14]);
          }

          if (preheating_hours != nullptr)
          {
            preheating_hours->publish_state((msg[11] << 8) | msg[12]);
          }

          if (filter_hours != nullptr)
          {
            filter_hours->publish_state((msg[15] << 8) | msg[16]);
          }
          break;
        }

        case RES_GET_PREHEATING_STATUS:
        {
          if (preheating_valve != nullptr)
          {
            std::string name_preheating_valve;
            switch (msg[0])
            {
            case 0:
              name_preheating_valve = "Closed";
              break;

            case 1:
              name_preheating_valve = "Open";
              break;

            default:
              name_preheating_valve = "Unknown";
              break;
            }
            preheating_valve->publish_state(name_preheating_valve);
          }

          if (frost_protection_active != nullptr)
          {
            frost_protection_active->publish_state(msg[1] != 0);
          }

          if (preheating_state != nullptr)
          {
            preheating_state->publish_state(msg[2] != 0);
          }

          if (frost_protection_minutes != nullptr)
          {
            frost_protection_minutes->publish_state((msg[3] << 8) | msg[4]);
          }

          if (frost_protection_level != nullptr)
          {
            std::string name_frost_protection_level;
            switch (msg[5])
            {
            case 0:
              name_frost_protection_level = "GuaranteedProtection";
              break;

            case 1:
              name_frost_protection_level = "HighProtection";
              break;

            case 2:
              name_frost_protection_level = "NominalProtection";
              break;

            case 3:
              name_frost_protection_level = "Economy";
              break;

            default:
              name_frost_protection_level = "Unknown";
              break;
            }
            frost_protection_level->publish_state(name_frost_protection_level);
          }
          break;
        }
        case RES_GET_TIME_DELAY:
        {
          // Cache all time delay values
          for (int i = 0; i < 8; i++)
          {
            time_delays_[i] = msg[i];
          }
          time_delays_valid_ = true;

          // Publish to read-only sensors (existing)
          if (bathroom_switch_on_delay_minutes != nullptr)
          {
            bathroom_switch_on_delay_minutes->publish_state(msg[0]);
          }

          if (bathroom_switch_off_delay_minutes != nullptr)
          {
            bathroom_switch_off_delay_minutes->publish_state(msg[1]);
          }

          if (l1_switch_off_delay_minutes != nullptr)
          {
            l1_switch_off_delay_minutes->publish_state(msg[2]);
          }

          if (boost_ventilation_minutes != nullptr)
          {
            boost_ventilation_minutes->publish_state(msg[3]);
          }

          if (filter_warning_weeks != nullptr)
          {
            filter_warning_weeks->publish_state(msg[4]);
          }

          if (rf_high_time_short_minutes != nullptr)
          {
            rf_high_time_short_minutes->publish_state(msg[5]);
          }

          if (rf_high_time_long_minutes != nullptr)
          {
            rf_high_time_long_minutes->publish_state(msg[6]);
          }

          if (extractor_hood_switch_off_delay_minutes != nullptr)
          {
            extractor_hood_switch_off_delay_minutes->publish_state(msg[7]);
          }

          // Publish to writable number components (new)
          if (bathroom_switch_on_delay_minutes_number != nullptr)
          {
            bathroom_switch_on_delay_minutes_number->publish_state(msg[0]);
          }

          if (bathroom_switch_off_delay_minutes_number != nullptr)
          {
            bathroom_switch_off_delay_minutes_number->publish_state(msg[1]);
          }

          if (l1_switch_off_delay_minutes_number != nullptr)
          {
            l1_switch_off_delay_minutes_number->publish_state(msg[2]);
          }

          if (boost_ventilation_minutes_number != nullptr)
          {
            boost_ventilation_minutes_number->publish_state(msg[3]);
          }

          if (filter_warning_weeks_number != nullptr)
          {
            filter_warning_weeks_number->publish_state(msg[4]);
          }

          if (rf_high_time_short_minutes_number != nullptr)
          {
            rf_high_time_short_minutes_number->publish_state(msg[5]);
          }

          if (rf_high_time_long_minutes_number != nullptr)
          {
            rf_high_time_long_minutes_number->publish_state(msg[6]);
          }

          if (extractor_hood_switch_off_delay_minutes_number != nullptr)
          {
            extractor_hood_switch_off_delay_minutes_number->publish_state(msg[7]);
          }

          break;
        }
        case RES_GET_INPUTS:
        {
          // Byte[0] = Step switch (1 = active / 0 = inactive)
          //   0x01 = L1, 0x02 = L2
          if (input_l1 != nullptr)
            input_l1->publish_state(msg[0] & 0x01);
          if (input_l2 != nullptr)
            input_l2->publish_state(msg[0] & 0x02);

          // Byte[1] = Switching inputs (1 = active / 0 = inactive)
          //   0x01 = bathroom switch
          //   0x02 = kitchen hood switch
          //   0x04 = External filter
          //   0x08 = heat recovery (WTW)
          //   0x10 = bathroom switch 2 (luxe)
          if (input_bathroom_switch != nullptr)
            input_bathroom_switch->publish_state(msg[1] & 0x01);
          if (input_kitchen_hood_switch != nullptr)
            input_kitchen_hood_switch->publish_state(msg[1] & 0x02);
          if (input_external_filter != nullptr)
            input_external_filter->publish_state(msg[1] & 0x04);
          if (input_wtw != nullptr)
            input_wtw->publish_state(msg[1] & 0x08);
          if (input_bathroom_switch_2 != nullptr)
            input_bathroom_switch_2->publish_state(msg[1] & 0x10);

          break;
        }
        case RES_GET_ANALOG_INPUTS:
        {
          // Byte[0-3] = Analog 1-4 (0..255 = 0..10V)
          // Convert to volts: value / 25.5
          if (analog_input_1 != nullptr)
            analog_input_1->publish_state(msg[0] / 25.5f);
          if (analog_input_2 != nullptr)
            analog_input_2->publish_state(msg[1] / 25.5f);
          if (analog_input_3 != nullptr)
            analog_input_3->publish_state(msg[2] / 25.5f);
          if (analog_input_4 != nullptr)
            analog_input_4->publish_state(msg[3] / 25.5f);

          break;
        }
        }
      }

      void get_fan_status_()
      {
        if (intake_fan_speed != nullptr ||
            exhaust_fan_speed != nullptr ||
            intake_fan_speed_rpm != nullptr ||
            exhaust_fan_speed_rpm != nullptr)
        {
          ESP_LOGD(TAG, "getting fan status");
          write_command_(CMD_GET_FAN_STATUS, nullptr, 0);
        }
      }

      void get_valve_status_()
      {
        if (bypass_valve != nullptr ||
            bypass_valve_open != nullptr ||
            preheating_state != nullptr)
        {
          ESP_LOGD(TAG, "getting valve status");
          write_command_(CMD_GET_VALVE_STATUS, nullptr, 0);
        }
      }

      void get_error_status_()
      {
        if (filter_status != nullptr)
        {
          ESP_LOGD(TAG, "getting error status");
          write_command_(CMD_GET_FAULTS, nullptr, 0);
        }
      }

      void get_bypass_control_status_()
      {
        if (bypass_factor != nullptr ||
            bypass_step != nullptr ||
            bypass_correction != nullptr ||
            summer_mode != nullptr)
        {
          ESP_LOGD(TAG, "getting bypass control");
          write_command_(CMD_GET_BYPASS_CONTROL_STATUS, nullptr, 0);
        }
      }

      void get_temperature_()
      {
        if (outside_air_temperature != nullptr ||
            supply_air_temperature != nullptr ||
            return_air_temperature != nullptr ||
            outside_air_temperature != nullptr)
        {
          ESP_LOGD(TAG, "getting temperature");
          write_command_(CMD_GET_TEMPERATURE_STATUS, nullptr, 0);
        }
      }

      void get_sensor_data_()
      {
        if (enthalpy_temperature != nullptr)
        {
          ESP_LOGD(TAG, "getting sensor data");
          write_command_(CMD_GET_SENSOR_DATA, nullptr, 0);
        }
      }

      void get_ventilation_level_()
      {
        ESP_LOGD(TAG, "getting ventilation level");
        write_command_(CMD_GET_VENTILATION_LEVEL, nullptr, 0);
      }

      void get_temperatures_()
      {
        ESP_LOGD(TAG, "getting temperatures");
        write_command_(CMD_GET_TEMPERATURES, nullptr, 0);
      }

      void get_operation_hours_()
      {
        ESP_LOGD(TAG, "getting operation hours");
        write_command_(CMD_GET_OPERATION_HOURS, nullptr, 0);
      }

      void get_preheating_status_()
      {
        ESP_LOGD(TAG, "getting preheating status");
        write_command_(CMD_GET_PREHEATING_STATUS, nullptr, 0);
      }

      void get_time_delay_()
      {
        ESP_LOGD(TAG, "getting time delay");
        write_command_(CMD_GET_TIME_DELAY, nullptr, 0);
      }

      void get_inputs_()
      {
        if (input_l1 != nullptr ||
            input_l2 != nullptr ||
            input_bathroom_switch != nullptr ||
            input_kitchen_hood_switch != nullptr ||
            input_external_filter != nullptr ||
            input_wtw != nullptr ||
            input_bathroom_switch_2 != nullptr)
        {
          ESP_LOGD(TAG, "getting inputs");
          write_command_(CMD_GET_INPUTS, nullptr, 0);
        }
      }

      void get_analog_inputs_()
      {
        if (analog_input_1 != nullptr ||
            analog_input_2 != nullptr ||
            analog_input_3 != nullptr ||
            analog_input_4 != nullptr)
        {
          ESP_LOGD(TAG, "getting analog inputs");
          write_command_(CMD_GET_ANALOG_INPUTS, nullptr, 0);
        }
      }

      uint8_t get_uint8_t_(uint8_t start_index) const
      {
        return data_[COMMAND_LEN_HEAD + start_index];
      }

      uint16_t get_uint16_(uint8_t start_index) const
      {
        return (uint16_t(data_[COMMAND_LEN_HEAD + start_index + 1] | data_[COMMAND_LEN_HEAD + start_index] << 8));
      }

      void publish_size_entities_(uint8_t raw_size);
      const char *unit_size_text_label_(uint8_t raw_size) const;
      const char *unit_size_option_label_(uint8_t raw_size) const;

      uint8_t data_[30];
      uint8_t data_index_{0};
      int8_t update_counter_{-4};
      const int8_t num_update_counter_elements_{11};
      uint8_t status_payload_[8]{0};
      bool status_payload_valid_{false};
      uint8_t current_unit_size_{0};
      ComfoAirSizeSelect *size_select_{nullptr};

      // Ventilation level cache (8 values: exhaust absent/low/med/high, supply absent/low/med/high)
      uint8_t ventilation_levels_[8]{0, 0, 0, 0, 0, 0, 0, 0};
      bool ventilation_levels_valid_{false};

      // Time delay cache (8 values according to CMD_GET_TIME_DELAY)
      uint8_t time_delays_[8]{0, 0, 0, 0, 0, 0, 0, 0};
      bool time_delays_valid_{false};

      uint8_t bootloader_version_[13]{0};
      uint8_t firmware_version_[13]{0};
      uint8_t connector_board_version_[14]{0};
      const char *name{0};

    public:
      text_sensor::TextSensor *type{nullptr};
      text_sensor::TextSensor *size{nullptr};
      text_sensor::TextSensor *filter_status{nullptr};
      text_sensor::TextSensor *frost_protection_level{nullptr};
      text_sensor::TextSensor *preheating_valve{nullptr};
      sensor::Sensor *intake_fan_speed{nullptr};
      sensor::Sensor *exhaust_fan_speed{nullptr};
      sensor::Sensor *intake_fan_speed_rpm{nullptr};
      sensor::Sensor *exhaust_fan_speed_rpm{nullptr};
      sensor::Sensor *ventilation_level{nullptr};
      sensor::Sensor *outside_air_temperature{nullptr};
      sensor::Sensor *supply_air_temperature{nullptr};
      sensor::Sensor *return_air_temperature{nullptr};
      sensor::Sensor *exhaust_air_temperature{nullptr};
      sensor::Sensor *enthalpy_temperature{nullptr};
      sensor::Sensor *ewt_temperature{nullptr};
      sensor::Sensor *postheating_temperature{nullptr};
      sensor::Sensor *kitchen_hood_temperature{nullptr};
      sensor::Sensor *return_air_level{nullptr};
      sensor::Sensor *supply_air_level{nullptr};
      sensor::Sensor *bypass_factor{nullptr};
      sensor::Sensor *bypass_step{nullptr};
      sensor::Sensor *bypass_correction{nullptr};
      sensor::Sensor *bypass_open_hours{nullptr};
      sensor::Sensor *motor_current_bypass{nullptr};
      sensor::Sensor *motor_current_preheating{nullptr};
      sensor::Sensor *preheating_hours{nullptr};
      sensor::Sensor *level0_hours{nullptr};
      sensor::Sensor *level1_hours{nullptr};
      sensor::Sensor *level2_hours{nullptr};
      sensor::Sensor *level3_hours{nullptr};
      binary_sensor::BinarySensor *frost_protection_active{nullptr};
      sensor::Sensor *frost_protection_hours{nullptr};
      sensor::Sensor *frost_protection_minutes{nullptr};
      sensor::Sensor *filter_hours{nullptr};
      sensor::Sensor *bypass_valve{nullptr};
      binary_sensor::BinarySensor *bypass_present{nullptr};
      binary_sensor::BinarySensor *enthalpy_present{nullptr};
      binary_sensor::BinarySensor *ewt_present{nullptr};
      binary_sensor::BinarySensor *preheating_present{nullptr};
      binary_sensor::BinarySensor *options_present{nullptr};
      binary_sensor::BinarySensor *fireplace_present{nullptr};
      binary_sensor::BinarySensor *kitchen_hood_present{nullptr};
      binary_sensor::BinarySensor *postheating_present{nullptr};
      binary_sensor::BinarySensor *postheating_pwm_mode_present{nullptr};
      binary_sensor::BinarySensor *bypass_valve_open{nullptr};
      binary_sensor::BinarySensor *preheating_state{nullptr};
      binary_sensor::BinarySensor *summer_mode{nullptr};
      binary_sensor::BinarySensor *supply_fan_active{nullptr};
      binary_sensor::BinarySensor *p10_active{nullptr};
      binary_sensor::BinarySensor *p11_active{nullptr};
      binary_sensor::BinarySensor *p12_active{nullptr};
      binary_sensor::BinarySensor *p13_active{nullptr};
      binary_sensor::BinarySensor *p14_active{nullptr};
      binary_sensor::BinarySensor *p15_active{nullptr};
      binary_sensor::BinarySensor *p16_active{nullptr};
      binary_sensor::BinarySensor *p17_active{nullptr};
      binary_sensor::BinarySensor *p18_active{nullptr};
      binary_sensor::BinarySensor *p19_active{nullptr};
      binary_sensor::BinarySensor *p90_active{nullptr};
      binary_sensor::BinarySensor *p91_active{nullptr};
      binary_sensor::BinarySensor *p92_active{nullptr};
      binary_sensor::BinarySensor *p93_active{nullptr};
      binary_sensor::BinarySensor *p94_active{nullptr};
      binary_sensor::BinarySensor *p95_active{nullptr};
      binary_sensor::BinarySensor *p96_active{nullptr};
      binary_sensor::BinarySensor *p97_active{nullptr};
      sensor::Sensor *bathroom_switch_on_delay_minutes{nullptr};
      sensor::Sensor *bathroom_switch_off_delay_minutes{nullptr};
      sensor::Sensor *l1_switch_off_delay_minutes{nullptr};
      sensor::Sensor *boost_ventilation_minutes{nullptr};
      sensor::Sensor *filter_warning_weeks{nullptr};
      sensor::Sensor *rf_high_time_short_minutes{nullptr};
      sensor::Sensor *rf_high_time_long_minutes{nullptr};
      sensor::Sensor *extractor_hood_switch_off_delay_minutes{nullptr};

      // Error code binary sensors
      binary_sensor::BinarySensor *error_a0{nullptr};
      binary_sensor::BinarySensor *error_a1{nullptr};
      binary_sensor::BinarySensor *error_a2{nullptr};
      binary_sensor::BinarySensor *error_a3{nullptr};
      binary_sensor::BinarySensor *error_a4{nullptr};
      binary_sensor::BinarySensor *error_a5{nullptr};
      binary_sensor::BinarySensor *error_a6{nullptr};
      binary_sensor::BinarySensor *error_a7{nullptr};
      binary_sensor::BinarySensor *error_a8{nullptr};
      binary_sensor::BinarySensor *error_a9{nullptr};
      binary_sensor::BinarySensor *error_a10{nullptr};
      binary_sensor::BinarySensor *error_a11{nullptr};
      binary_sensor::BinarySensor *error_a12{nullptr};
      binary_sensor::BinarySensor *error_a13{nullptr};
      binary_sensor::BinarySensor *error_a14{nullptr};
      binary_sensor::BinarySensor *error_a15{nullptr};
      binary_sensor::BinarySensor *error_e1{nullptr};
      binary_sensor::BinarySensor *error_e2{nullptr};
      binary_sensor::BinarySensor *error_ea1{nullptr};
      binary_sensor::BinarySensor *error_ea2{nullptr};
      binary_sensor::BinarySensor *error_ea3{nullptr};
      binary_sensor::BinarySensor *error_ea4{nullptr};
      binary_sensor::BinarySensor *error_ea5{nullptr};
      binary_sensor::BinarySensor *error_ea6{nullptr};
      binary_sensor::BinarySensor *error_ea7{nullptr};
      binary_sensor::BinarySensor *error_ea8{nullptr};

      // Ventilation level number components
      number::Number *supply_absent_percent{nullptr};
      number::Number *supply_low_percent{nullptr};
      number::Number *supply_medium_percent{nullptr};
      number::Number *supply_high_percent{nullptr};
      number::Number *exhaust_absent_percent{nullptr};
      number::Number *exhaust_low_percent{nullptr};
      number::Number *exhaust_medium_percent{nullptr};
      number::Number *exhaust_high_percent{nullptr};

      // Time delay number components (writable)
      number::Number *bathroom_switch_on_delay_minutes_number{nullptr};
      number::Number *bathroom_switch_off_delay_minutes_number{nullptr};
      number::Number *l1_switch_off_delay_minutes_number{nullptr};
      number::Number *boost_ventilation_minutes_number{nullptr};
      number::Number *filter_warning_weeks_number{nullptr};
      number::Number *rf_high_time_short_minutes_number{nullptr};
      number::Number *rf_high_time_long_minutes_number{nullptr};
      number::Number *extractor_hood_switch_off_delay_minutes_number{nullptr};

      // Digital input binary sensors
      binary_sensor::BinarySensor *input_l1{nullptr};
      binary_sensor::BinarySensor *input_l2{nullptr};
      binary_sensor::BinarySensor *input_bathroom_switch{nullptr};
      binary_sensor::BinarySensor *input_kitchen_hood_switch{nullptr};
      binary_sensor::BinarySensor *input_external_filter{nullptr};
      binary_sensor::BinarySensor *input_wtw{nullptr};
      binary_sensor::BinarySensor *input_bathroom_switch_2{nullptr};

      // Analog input sensors (voltage)
      sensor::Sensor *analog_input_1{nullptr};
      sensor::Sensor *analog_input_2{nullptr};
      sensor::Sensor *analog_input_3{nullptr};
      sensor::Sensor *analog_input_4{nullptr};

      // Button entities
      button::Button *reset_errors_{nullptr};
      button::Button *start_self_test_{nullptr};

      // Switch entities
      switch_::Switch *preheater_present_switch_{nullptr};

      // Select entities
      select::Select *rs232_mode_select_{nullptr};
      select::Select *fan_mode_select_{nullptr};

      // Test mode entities
      switch_::Switch *test_mode_switch_{nullptr};
      number::Number *test_supply_fan_number_{nullptr};
      number::Number *test_exhaust_fan_number_{nullptr};
      switch_::Switch *test_preheating_relay_switch_{nullptr};
      switch_::Switch *test_preheating_triac_switch_{nullptr};
      switch_::Switch *test_kitchen_hood_switch_{nullptr};
      switch_::Switch *test_error_led_switch_{nullptr};
      switch_::Switch *test_filter_led_switch_{nullptr};
      select::Select *test_bypass_flap_select_{nullptr};
      select::Select *test_preheating_flap_select_{nullptr};

      // Test mode state tracking
      bool test_mode_active_{false};
      uint8_t test_relay_state_{0};
      uint8_t test_feedback_state_{0};
      uint8_t test_supply_fan_speed_{0};
      uint8_t test_exhaust_fan_speed_{0};
      uint8_t bypass_flap_state_{0};
      uint8_t preheating_flap_state_{0};

      // Default ventilation percentages for fan_mode control
      static constexpr uint8_t DEFAULT_SUPPLY_ABSENT = 15;
      static constexpr uint8_t DEFAULT_SUPPLY_LOW = 35;
      static constexpr uint8_t DEFAULT_SUPPLY_MEDIUM = 50;
      static constexpr uint8_t DEFAULT_SUPPLY_HIGH = 85;
      static constexpr uint8_t DEFAULT_EXHAUST_ABSENT = 15;
      static constexpr uint8_t DEFAULT_EXHAUST_LOW = 35;
      static constexpr uint8_t DEFAULT_EXHAUST_MEDIUM = 50;
      static constexpr uint8_t DEFAULT_EXHAUST_HIGH = 85;

      void set_type(text_sensor::TextSensor *type) { this->type = type; };
      void set_size(text_sensor::TextSensor *size) { this->size = size; };
      void set_intake_fan_speed(sensor::Sensor *intake_fan_speed) { this->intake_fan_speed = intake_fan_speed; };
      void set_exhaust_fan_speed(sensor::Sensor *exhaust_fan_speed) { this->exhaust_fan_speed = exhaust_fan_speed; };
      void set_intake_fan_speed_rpm(sensor::Sensor *intake_fan_speed_rpm)
      {
        this->intake_fan_speed_rpm = intake_fan_speed_rpm;
        this->intake_fan_speed_rpm->set_accuracy_decimals(0);
      };
      void set_exhaust_fan_speed_rpm(sensor::Sensor *exhaust_fan_speed_rpm)
      {
        this->exhaust_fan_speed_rpm = exhaust_fan_speed_rpm;
        this->exhaust_fan_speed_rpm->set_accuracy_decimals(0);
      };
      void set_ventilation_level(sensor::Sensor *ventilation_level) { this->ventilation_level = ventilation_level; };
      void set_bypass_valve(sensor::Sensor *bypass_valve) { this->bypass_valve = bypass_valve; };
      void set_bypass_present(binary_sensor::BinarySensor *bypass_present) { this->bypass_present = bypass_present; };
      void set_enthalpy_present(binary_sensor::BinarySensor *enthalpy_present) { this->enthalpy_present = enthalpy_present; };
      void set_ewt_present(binary_sensor::BinarySensor *ewt_present) { this->ewt_present = ewt_present; };
      void set_preheating_present(binary_sensor::BinarySensor *preheating_present) { this->preheating_present = preheating_present; };
      void set_options_present(binary_sensor::BinarySensor *options_present) { this->options_present = options_present; };
      void set_fireplace_present(binary_sensor::BinarySensor *fireplace_present) { this->fireplace_present = fireplace_present; };
      void set_kitchen_hood_present(binary_sensor::BinarySensor *kitchen_hood_present) { this->kitchen_hood_present = kitchen_hood_present; };
      void set_postheating_present(binary_sensor::BinarySensor *postheating_present) { this->postheating_present = postheating_present; };
      void set_postheating_pwm_mode_present(binary_sensor::BinarySensor *postheating_pwm_mode_present) { this->postheating_pwm_mode_present = postheating_pwm_mode_present; };
      void set_bypass_valve_open(binary_sensor::BinarySensor *bypass_valve_open) { this->bypass_valve_open = bypass_valve_open; };
      void set_preheating_state(binary_sensor::BinarySensor *preheating_state) { this->preheating_state = preheating_state; };
      void set_outside_air_temperature(sensor::Sensor *outside_air_temperature) { this->outside_air_temperature = outside_air_temperature; };
      void set_supply_air_temperature(sensor::Sensor *supply_air_temperature) { this->supply_air_temperature = supply_air_temperature; };
      void set_return_air_temperature(sensor::Sensor *return_air_temperature) { this->return_air_temperature = return_air_temperature; };
      void set_exhaust_air_temperature(sensor::Sensor *exhaust_air_temperature) { this->exhaust_air_temperature = exhaust_air_temperature; };
      void set_enthalpy_temperature(sensor::Sensor *enthalpy_temperature) { this->enthalpy_temperature = enthalpy_temperature; };
      void set_ewt_temperature(sensor::Sensor *ewt_temperature) { this->ewt_temperature = ewt_temperature; };
      void set_postheating_temperature(sensor::Sensor *postheating_temperature) { this->postheating_temperature = postheating_temperature; };
      void set_kitchen_hood_temperature(sensor::Sensor *kitchen_hood_temperature) { this->kitchen_hood_temperature = kitchen_hood_temperature; };
      void set_return_air_level(sensor::Sensor *return_air_level) { this->return_air_level = return_air_level; };
      void set_supply_air_level(sensor::Sensor *supply_air_level) { this->supply_air_level = supply_air_level; };
      void set_supply_fan_active(binary_sensor::BinarySensor *supply_fan_active) { this->supply_fan_active = supply_fan_active; };
      void set_filter_status(text_sensor::TextSensor *filter_status) { this->filter_status = filter_status; };
      void set_bypass_factor(sensor::Sensor *bypass_factor) { this->bypass_factor = bypass_factor; };
      void set_bypass_step(sensor::Sensor *bypass_step) { this->bypass_step = bypass_step; };
      void set_bypass_correction(sensor::Sensor *bypass_correction) { this->bypass_correction = bypass_correction; };
      void set_bypass_open_hours(sensor::Sensor *bypass_open_hours) { this->bypass_open_hours = bypass_open_hours; };
      void set_motor_current_bypass(sensor::Sensor *motor_current_bypass) { this->motor_current_bypass = motor_current_bypass; };
      void set_motor_current_preheating(sensor::Sensor *motor_current_preheating) { this->motor_current_preheating = motor_current_preheating; };
      void set_preheating_hours(sensor::Sensor *preheating_hours) { this->preheating_hours = preheating_hours; };
      void set_preheating_valve(text_sensor::TextSensor *preheating_valve) { this->preheating_valve = preheating_valve; };
      void set_level0_hours(sensor::Sensor *level0_hours) { this->level0_hours = level0_hours; };
      void set_level1_hours(sensor::Sensor *level1_hours) { this->level1_hours = level1_hours; };
      void set_level2_hours(sensor::Sensor *level2_hours) { this->level2_hours = level2_hours; };
      void set_level3_hours(sensor::Sensor *level3_hours) { this->level3_hours = level3_hours; };
      void set_frost_protection_active(binary_sensor::BinarySensor *frost_protection_active) { this->frost_protection_active = frost_protection_active; };
      void set_frost_protection_hours(sensor::Sensor *frost_protection_hours) { this->frost_protection_hours = frost_protection_hours; };
      void set_frost_protection_minutes(sensor::Sensor *frost_protection_minutes) { this->frost_protection_minutes = frost_protection_minutes; };
      void set_frost_protection_level(text_sensor::TextSensor *frost_protection_level) { this->frost_protection_level = frost_protection_level; };
      void set_filter_hours(sensor::Sensor *filter_hours) { this->filter_hours = filter_hours; };
      void set_summer_mode(binary_sensor::BinarySensor *summer_mode) { this->summer_mode = summer_mode; };
      void set_p10_active(binary_sensor::BinarySensor *p10_active) { this->p10_active = p10_active; };
      void set_p11_active(binary_sensor::BinarySensor *p11_active) { this->p11_active = p11_active; };
      void set_p12_active(binary_sensor::BinarySensor *p12_active) { this->p12_active = p12_active; };
      void set_p13_active(binary_sensor::BinarySensor *p13_active) { this->p13_active = p13_active; };
      void set_p14_active(binary_sensor::BinarySensor *p14_active) { this->p14_active = p14_active; };
      void set_p15_active(binary_sensor::BinarySensor *p15_active) { this->p15_active = p15_active; };
      void set_p16_active(binary_sensor::BinarySensor *p16_active) { this->p16_active = p16_active; };
      void set_p17_active(binary_sensor::BinarySensor *p17_active) { this->p17_active = p17_active; };
      void set_p18_active(binary_sensor::BinarySensor *p18_active) { this->p18_active = p18_active; };
      void set_p19_active(binary_sensor::BinarySensor *p19_active) { this->p19_active = p19_active; };
      void set_p90_active(binary_sensor::BinarySensor *p90_active) { this->p90_active = p90_active; };
      void set_p91_active(binary_sensor::BinarySensor *p91_active) { this->p91_active = p91_active; };
      void set_p92_active(binary_sensor::BinarySensor *p92_active) { this->p92_active = p92_active; };
      void set_p93_active(binary_sensor::BinarySensor *p93_active) { this->p93_active = p93_active; };
      void set_p94_active(binary_sensor::BinarySensor *p94_active) { this->p94_active = p94_active; };
      void set_p95_active(binary_sensor::BinarySensor *p95_active) { this->p95_active = p95_active; };
      void set_p96_active(binary_sensor::BinarySensor *p96_active) { this->p96_active = p96_active; };
      void set_p97_active(binary_sensor::BinarySensor *p97_active) { this->p97_active = p97_active; };
      void set_bathroom_switch_on_delay_minutes(sensor::Sensor *bathroom_switch_on_delay_minutes) { this->bathroom_switch_on_delay_minutes = bathroom_switch_on_delay_minutes; };
      void set_bathroom_switch_off_delay_minutes(sensor::Sensor *bathroom_switch_off_delay_minutes) { this->bathroom_switch_off_delay_minutes = bathroom_switch_off_delay_minutes; };
      void set_l1_switch_off_delay_minutes(sensor::Sensor *l1_switch_off_delay_minutes) { this->l1_switch_off_delay_minutes = l1_switch_off_delay_minutes; };
      void set_boost_ventilation_minutes(sensor::Sensor *boost_ventilation_minutes) { this->boost_ventilation_minutes = boost_ventilation_minutes; };
      void set_filter_warning_weeks(sensor::Sensor *filter_warning_weeks) { this->filter_warning_weeks = filter_warning_weeks; };
      void set_rf_high_time_short_minutes(sensor::Sensor *rf_high_time_short_minutes) { this->rf_high_time_short_minutes = rf_high_time_short_minutes; };
      void set_rf_high_time_long_minutes(sensor::Sensor *rf_high_time_long_minutes) { this->rf_high_time_long_minutes = rf_high_time_long_minutes; };
      void set_extractor_hood_switch_off_delay_minutes(sensor::Sensor *extractor_hood_switch_off_delay_minutes) { this->extractor_hood_switch_off_delay_minutes = extractor_hood_switch_off_delay_minutes; };

      void set_error_a0(binary_sensor::BinarySensor *error_a0) { this->error_a0 = error_a0; };
      void set_error_a1(binary_sensor::BinarySensor *error_a1) { this->error_a1 = error_a1; };
      void set_error_a2(binary_sensor::BinarySensor *error_a2) { this->error_a2 = error_a2; };
      void set_error_a3(binary_sensor::BinarySensor *error_a3) { this->error_a3 = error_a3; };
      void set_error_a4(binary_sensor::BinarySensor *error_a4) { this->error_a4 = error_a4; };
      void set_error_a5(binary_sensor::BinarySensor *error_a5) { this->error_a5 = error_a5; };
      void set_error_a6(binary_sensor::BinarySensor *error_a6) { this->error_a6 = error_a6; };
      void set_error_a7(binary_sensor::BinarySensor *error_a7) { this->error_a7 = error_a7; };
      void set_error_a8(binary_sensor::BinarySensor *error_a8) { this->error_a8 = error_a8; };
      void set_error_a9(binary_sensor::BinarySensor *error_a9) { this->error_a9 = error_a9; };
      void set_error_a10(binary_sensor::BinarySensor *error_a10) { this->error_a10 = error_a10; };
      void set_error_a11(binary_sensor::BinarySensor *error_a11) { this->error_a11 = error_a11; };
      void set_error_a12(binary_sensor::BinarySensor *error_a12) { this->error_a12 = error_a12; };
      void set_error_a13(binary_sensor::BinarySensor *error_a13) { this->error_a13 = error_a13; };
      void set_error_a14(binary_sensor::BinarySensor *error_a14) { this->error_a14 = error_a14; };
      void set_error_a15(binary_sensor::BinarySensor *error_a15) { this->error_a15 = error_a15; };
      void set_error_e1(binary_sensor::BinarySensor *error_e1) { this->error_e1 = error_e1; };
      void set_error_e2(binary_sensor::BinarySensor *error_e2) { this->error_e2 = error_e2; };
      void set_error_ea1(binary_sensor::BinarySensor *error_ea1) { this->error_ea1 = error_ea1; };
      void set_error_ea2(binary_sensor::BinarySensor *error_ea2) { this->error_ea2 = error_ea2; };
      void set_error_ea3(binary_sensor::BinarySensor *error_ea3) { this->error_ea3 = error_ea3; };
      void set_error_ea4(binary_sensor::BinarySensor *error_ea4) { this->error_ea4 = error_ea4; };
      void set_error_ea5(binary_sensor::BinarySensor *error_ea5) { this->error_ea5 = error_ea5; };
      void set_error_ea6(binary_sensor::BinarySensor *error_ea6) { this->error_ea6 = error_ea6; };
      void set_error_ea7(binary_sensor::BinarySensor *error_ea7) { this->error_ea7 = error_ea7; };
      void set_error_ea8(binary_sensor::BinarySensor *error_ea8) { this->error_ea8 = error_ea8; };

      void set_supply_absent_percent(number::Number *supply_absent_percent) { this->supply_absent_percent = supply_absent_percent; };
      void set_supply_low_percent(number::Number *supply_low_percent) { this->supply_low_percent = supply_low_percent; };
      void set_supply_medium_percent(number::Number *supply_medium_percent) { this->supply_medium_percent = supply_medium_percent; };
      void set_supply_high_percent(number::Number *supply_high_percent) { this->supply_high_percent = supply_high_percent; };
      void set_exhaust_absent_percent(number::Number *exhaust_absent_percent) { this->exhaust_absent_percent = exhaust_absent_percent; };
      void set_exhaust_low_percent(number::Number *exhaust_low_percent) { this->exhaust_low_percent = exhaust_low_percent; };
      void set_exhaust_medium_percent(number::Number *exhaust_medium_percent) { this->exhaust_medium_percent = exhaust_medium_percent; };
      void set_exhaust_high_percent(number::Number *exhaust_high_percent) { this->exhaust_high_percent = exhaust_high_percent; };

      void set_bathroom_switch_on_delay_minutes_number(number::Number *bathroom_switch_on_delay_minutes_number) { this->bathroom_switch_on_delay_minutes_number = bathroom_switch_on_delay_minutes_number; };
      void set_bathroom_switch_off_delay_minutes_number(number::Number *bathroom_switch_off_delay_minutes_number) { this->bathroom_switch_off_delay_minutes_number = bathroom_switch_off_delay_minutes_number; };
      void set_l1_switch_off_delay_minutes_number(number::Number *l1_switch_off_delay_minutes_number) { this->l1_switch_off_delay_minutes_number = l1_switch_off_delay_minutes_number; };
      void set_boost_ventilation_minutes_number(number::Number *boost_ventilation_minutes_number) { this->boost_ventilation_minutes_number = boost_ventilation_minutes_number; };
      void set_filter_warning_weeks_number(number::Number *filter_warning_weeks_number) { this->filter_warning_weeks_number = filter_warning_weeks_number; };
      void set_rf_high_time_short_minutes_number(number::Number *rf_high_time_short_minutes_number) { this->rf_high_time_short_minutes_number = rf_high_time_short_minutes_number; };
      void set_rf_high_time_long_minutes_number(number::Number *rf_high_time_long_minutes_number) { this->rf_high_time_long_minutes_number = rf_high_time_long_minutes_number; };
      void set_extractor_hood_switch_off_delay_minutes_number(number::Number *extractor_hood_switch_off_delay_minutes_number) { this->extractor_hood_switch_off_delay_minutes_number = extractor_hood_switch_off_delay_minutes_number; };

      void set_input_l1(binary_sensor::BinarySensor *input_l1) { this->input_l1 = input_l1; };
      void set_input_l2(binary_sensor::BinarySensor *input_l2) { this->input_l2 = input_l2; };
      void set_input_bathroom_switch(binary_sensor::BinarySensor *input_bathroom_switch) { this->input_bathroom_switch = input_bathroom_switch; };
      void set_input_kitchen_hood_switch(binary_sensor::BinarySensor *input_kitchen_hood_switch) { this->input_kitchen_hood_switch = input_kitchen_hood_switch; };
      void set_input_external_filter(binary_sensor::BinarySensor *input_external_filter) { this->input_external_filter = input_external_filter; };
      void set_input_wtw(binary_sensor::BinarySensor *input_wtw) { this->input_wtw = input_wtw; };
      void set_input_bathroom_switch_2(binary_sensor::BinarySensor *input_bathroom_switch_2) { this->input_bathroom_switch_2 = input_bathroom_switch_2; };

      void set_analog_input_1(sensor::Sensor *analog_input_1) { this->analog_input_1 = analog_input_1; };
      void set_analog_input_2(sensor::Sensor *analog_input_2) { this->analog_input_2 = analog_input_2; };
      void set_analog_input_3(sensor::Sensor *analog_input_3) { this->analog_input_3 = analog_input_3; };
      void set_analog_input_4(sensor::Sensor *analog_input_4) { this->analog_input_4 = analog_input_4; };

      void set_reset_errors(button::Button *reset_errors) { this->reset_errors_ = reset_errors; };
      void set_start_self_test(button::Button *start_self_test) { this->start_self_test_ = start_self_test; };
      void set_preheater_present_switch(switch_::Switch *preheater_present_switch) { this->preheater_present_switch_ = preheater_present_switch; };
      void set_rs232_mode_select(select::Select *rs232_mode_select) { this->rs232_mode_select_ = rs232_mode_select; };
      void set_fan_mode_select(select::Select *fan_mode_select) { this->fan_mode_select_ = fan_mode_select; };
      void set_test_mode_switch(switch_::Switch *test_mode_switch) { this->test_mode_switch_ = test_mode_switch; };
      void set_test_supply_fan_number(number::Number *test_supply_fan_number) { this->test_supply_fan_number_ = test_supply_fan_number; };
      void set_test_exhaust_fan_number(number::Number *test_exhaust_fan_number) { this->test_exhaust_fan_number_ = test_exhaust_fan_number; };
      void set_test_preheating_relay_switch(switch_::Switch *test_preheating_relay_switch) { this->test_preheating_relay_switch_ = test_preheating_relay_switch; };
      void set_test_preheating_triac_switch(switch_::Switch *test_preheating_triac_switch) { this->test_preheating_triac_switch_ = test_preheating_triac_switch; };
      void set_test_kitchen_hood_switch(switch_::Switch *test_kitchen_hood_switch) { this->test_kitchen_hood_switch_ = test_kitchen_hood_switch; };
      void set_test_error_led_switch(switch_::Switch *test_error_led_switch) { this->test_error_led_switch_ = test_error_led_switch; };
      void set_test_filter_led_switch(switch_::Switch *test_filter_led_switch) { this->test_filter_led_switch_ = test_filter_led_switch; };
      void set_test_bypass_flap_select(select::Select *test_bypass_flap_select) { this->test_bypass_flap_select_ = test_bypass_flap_select; };
      void set_test_preheating_flap_select(select::Select *test_preheating_flap_select) { this->test_preheating_flap_select_ = test_preheating_flap_select; };
      bool set_rs232_mode(uint8_t mode);
      bool set_fan_mode(const std::string &mode);
    };

    inline const char *ComfoAirComponent::unit_size_text_label_(uint8_t raw_size) const
    {
      switch (raw_size)
      {
      case 1:
        return "Large";
      case 2:
        return "Small";
      default:
        return "Unknown";
      }
    }

    inline const char *ComfoAirComponent::unit_size_option_label_(uint8_t raw_size) const
    {
      switch (raw_size)
      {
      case 1:
        return "Large";
      case 2:
        return "Small";
      default:
        return nullptr;
      }
    }

    inline void ComfoAirComponent::publish_size_entities_(uint8_t raw_size)
    {
      current_unit_size_ = raw_size;
      if (size != nullptr)
      {
        size->publish_state(unit_size_text_label_(raw_size));
      }
      if (size_select_ != nullptr)
      {
        if (const char *option = unit_size_option_label_(raw_size))
        {
          size_select_->publish_state(option);
        }
        else
        {
          ESP_LOGW(TAG, "Unsupported unit size value: %u", raw_size);
        }
      }
    }

    inline bool ComfoAirComponent::set_unit_size(uint8_t raw_size)
    {
      if (raw_size != 1 && raw_size != 2)
      {
        ESP_LOGW(TAG, "Ignoring invalid unit size request: %u", raw_size);
        return false;
      }

      if (!status_payload_valid_)
      {
        ESP_LOGW(TAG, "Unit size cannot be changed before initial status is received");
        publish_size_entities_(current_unit_size_);
        return false;
      }

      if (status_payload_[3] == raw_size)
      {
        publish_size_entities_(raw_size);
        return true;
      }

      uint8_t payload[sizeof(status_payload_)];
      memcpy(payload, status_payload_, sizeof(status_payload_));
      payload[3] = raw_size;

      ESP_LOGI(TAG, "Setting unit size to %s (raw value: %u)", unit_size_text_label_(raw_size), raw_size);
      write_command_(CMD_SET_STATUS, payload, sizeof(payload));

      status_payload_[3] = raw_size;
      publish_size_entities_(raw_size);

      return true;
    }

    inline void ComfoAirComponent::set_size_select(ComfoAirSizeSelect *size_select)
    {
      this->size_select_ = size_select;
      if (this->size_select_ != nullptr)
      {
        this->size_select_->set_parent(this);
        if (const char *option = this->unit_size_option_label_(this->current_unit_size_))
        {
          this->size_select_->publish_state(option);
        }
      }
    }

    inline void ComfoAirSizeSelect::control(const std::string &value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Unit size select has no parent component configured");
        return;
      }

      auto index = this->index_of(value);
      if (!index.has_value())
      {
        ESP_LOGW(TAG, "Unit size select received invalid option: %s", value.c_str());
        return;
      }

      uint8_t raw_size = 0;
      switch (index.value())
      {
      case 0:
        raw_size = 1;
        break;
      case 1:
        raw_size = 2;
        break;
      default:
        ESP_LOGW(TAG, "Unit size select index %zu not supported", index.value());
        return;
      }

      if (!this->parent_->set_unit_size(raw_size))
      {
        if (const char *current_option = this->parent_->unit_size_option_label_(this->parent_->current_unit_size_))
        {
          this->publish_state(current_option);
        }
      }
    }

    inline void ComfoAirVentilationLevelNumber::control(float value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Ventilation level number has no parent component configured");
        return;
      }

      uint8_t percent = static_cast<uint8_t>(value);
      if (!this->parent_->set_ventilation_level_percent(this->level_index_, percent))
      {
        // Revert to current value if setting failed
        this->publish_state(this->parent_->ventilation_levels_[this->level_index_]);
      }
    }

    inline void ComfoAirTimeDelayNumber::control(float value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Time delay number has no parent component configured");
        return;
      }

      uint8_t delay_value = static_cast<uint8_t>(value);
      if (!this->parent_->set_time_delay(this->delay_index_, delay_value))
      {
        // Revert to current value if setting failed
        this->publish_state(this->parent_->time_delays_[this->delay_index_]);
      }
    }

    inline void ComfoAirResetErrorsButton::press_action()
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Reset errors button has no parent component configured");
        return;
      }

      ESP_LOGI(TAG, "Resetting ComfoAir error codes");
      uint8_t reset_cmd[4] = {1, 0, 0, 0}; // Reset faults only (byte 0)
      this->parent_->write_command_(CMD_RESET_AND_SELF_TEST, reset_cmd, sizeof(reset_cmd));
    }

    inline void ComfoAirStartSelfTestButton::press_action()
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Start self-test button has no parent component configured");
        return;
      }

      ESP_LOGI(TAG, "Starting ComfoAir self-test diagnostic mode");
      uint8_t test_cmd[4] = {0, 0, 1, 0}; // Start self-test (byte 2)
      this->parent_->write_command_(CMD_RESET_AND_SELF_TEST, test_cmd, sizeof(test_cmd));
    }

    inline bool ComfoAirComponent::set_preheater_present(bool present)
    {
      if (!status_payload_valid_)
      {
        ESP_LOGW(TAG, "Preheater presence cannot be changed before initial status is received");
        if (preheater_present_switch_ != nullptr)
        {
          preheater_present_switch_->publish_state(status_payload_[0] != 0);
        }
        return false;
      }

      uint8_t target_value = present ? 1 : 0;
      if (status_payload_[0] == target_value)
      {
        if (preheater_present_switch_ != nullptr)
        {
          preheater_present_switch_->publish_state(present);
        }
        return true;
      }

      uint8_t payload[sizeof(status_payload_)];
      memcpy(payload, status_payload_, sizeof(status_payload_));
      payload[0] = target_value;

      ESP_LOGI(TAG, "Setting preheater present to %s (raw value: %u)", present ? "ON" : "OFF", target_value);
      write_command_(CMD_SET_STATUS, payload, sizeof(payload));

      status_payload_[0] = target_value;
      if (preheater_present_switch_ != nullptr)
      {
        preheater_present_switch_->publish_state(present);
      }

      return true;
    }

    inline void ComfoAirPreheaterSwitch::write_state(bool state)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Preheater present switch has no parent component configured");
        return;
      }

      if (!this->parent_->set_preheater_present(state))
      {
        // Revert to current state if setting failed
        this->publish_state(this->parent_->status_payload_[0] != 0);
      }
    }

    inline const char *rs232_mode_to_string(uint8_t mode)
    {
      switch (mode)
      {
      case 0:
        return "End";
      case 1:
        return "PC_Only";
      case 3:
        return "PC_Master";
      case 4:
        return "PC_Log_Mode";
      default:
        return nullptr;
      }
    }

    inline bool ComfoAirComponent::set_rs232_mode(uint8_t mode)
    {
      // Valid modes: 0 = End, 1 = PC_Only, 3 = PC_Master, 4 = PC_Log_Mode
      if (mode != 0 && mode != 1 && mode != 3 && mode != 4)
      {
        ESP_LOGW(TAG, "Ignoring invalid RS232 mode request: %u", mode);
        return false;
      }

      const char *mode_name = rs232_mode_to_string(mode);
      uint8_t command[1] = {mode};
      ESP_LOGI(TAG, "Setting RS232 mode to %s (raw value: %u)", mode_name ? mode_name : "unknown", mode);
      write_command_(CMD_SET_RS232_MODE, command, sizeof(command));

      // Update select entity to reflect the change
      if (rs232_mode_select_ != nullptr)
      {
        if (mode_name != nullptr)
        {
          rs232_mode_select_->publish_state(mode_name);
        }
      }

      return true;
    }

    inline void ComfoAirRS232ModeSelect::control(const std::string &value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "RS232 mode select has no parent component configured");
        return;
      }

      auto index = this->index_of(value);
      if (!index.has_value())
      {
        ESP_LOGW(TAG, "RS232 mode select received invalid option: %s", value.c_str());
        return;
      }

      // Map index to protocol values: End=0, PC_Only=1, PC_Master=3, PC_Log_Mode=4
      uint8_t mode = 0;
      switch (index.value())
      {
      case 0: // End
        mode = 0;
        break;
      case 1: // PC_Only
        mode = 1;
        break;
      case 2: // PC_Master
        mode = 3;
        break;
      case 3: // PC_Log_Mode
        mode = 4;
        break;
      default:
        ESP_LOGW(TAG, "RS232 mode select index %zu not supported", index.value());
        return;
      }

      this->parent_->set_rs232_mode(mode);
    }

    inline bool ComfoAirComponent::set_fan_mode(const std::string &mode)
    {
      if (!ventilation_levels_valid_)
      {
        ESP_LOGW(TAG, "Fan mode cannot be changed before initial ventilation levels are received");
        return false;
      }

      // Determine which fans should be enabled based on mode
      bool enable_supply = false;
      bool enable_exhaust = false;

      if (mode == "Both")
      {
        enable_supply = true;
        enable_exhaust = true;
      }
      else if (mode == "Supply Only")
      {
        enable_supply = true;
        enable_exhaust = false;
      }
      else if (mode == "Exhaust Only")
      {
        enable_supply = false;
        enable_exhaust = true;
      }
      else if (mode == "Off")
      {
        enable_supply = false;
        enable_exhaust = false;
      }
      else
      {
        ESP_LOGW(TAG, "Unknown fan mode: %s", mode.c_str());
        return false;
      }

      // Set ventilation levels based on desired fan states - always use DEFAULT values
      if (enable_supply)
      {
        ventilation_levels_[0] = DEFAULT_SUPPLY_ABSENT;
        ventilation_levels_[1] = DEFAULT_SUPPLY_LOW;
        ventilation_levels_[2] = DEFAULT_SUPPLY_MEDIUM;
        ventilation_levels_[3] = DEFAULT_SUPPLY_HIGH;
      }
      else
      {
        // Disable supply fan - set all to absent level
        ventilation_levels_[0] = DEFAULT_SUPPLY_ABSENT;
        ventilation_levels_[1] = DEFAULT_SUPPLY_ABSENT;
        ventilation_levels_[2] = DEFAULT_SUPPLY_ABSENT;
        ventilation_levels_[3] = DEFAULT_SUPPLY_ABSENT;
      }

      if (enable_exhaust)
      {
        ventilation_levels_[4] = DEFAULT_EXHAUST_ABSENT;
        ventilation_levels_[5] = DEFAULT_EXHAUST_LOW;
        ventilation_levels_[6] = DEFAULT_EXHAUST_MEDIUM;
        ventilation_levels_[7] = DEFAULT_EXHAUST_HIGH;
      }
      else
      {
        // Disable exhaust fan - set all to absent level
        ventilation_levels_[4] = DEFAULT_EXHAUST_ABSENT;
        ventilation_levels_[5] = DEFAULT_EXHAUST_ABSENT;
        ventilation_levels_[6] = DEFAULT_EXHAUST_ABSENT;
        ventilation_levels_[7] = DEFAULT_EXHAUST_ABSENT;
      }

      ESP_LOGI(TAG, "Setting fan mode to: %s (Supply: %s, Exhaust: %s) - levels: SA=%u SL=%u SM=%u SH=%u EA=%u EL=%u EM=%u EH=%u",
               mode.c_str(),
               enable_supply ? "ON" : "OFF",
               enable_exhaust ? "ON" : "OFF",
               ventilation_levels_[0], ventilation_levels_[1], ventilation_levels_[2], ventilation_levels_[3],
               ventilation_levels_[4], ventilation_levels_[5], ventilation_levels_[6], ventilation_levels_[7]);

      // Send command with all 8 levels (protocol requires all values)
      uint8_t command[9];
      command[0] = ventilation_levels_[4]; // exhaust absent
      command[1] = ventilation_levels_[5]; // exhaust low
      command[2] = ventilation_levels_[6]; // exhaust medium
      command[3] = ventilation_levels_[0]; // supply absent
      command[4] = ventilation_levels_[1]; // supply low
      command[5] = ventilation_levels_[2]; // supply medium
      command[6] = ventilation_levels_[7]; // exhaust high
      command[7] = ventilation_levels_[3]; // supply high
      command[8] = 0x00;

      write_command_(CMD_SET_VENTILATION_LEVEL, command, sizeof(command));

      // Update the select entity state
      if (fan_mode_select_ != nullptr)
      {
        fan_mode_select_->publish_state(mode);
      }

      return true;
    }

    inline void ComfoAirFanModeSelect::control(const std::string &value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Fan mode select has no parent component configured");
        return;
      }

      // Validate the option
      auto index = this->index_of(value);
      if (!index.has_value())
      {
        ESP_LOGW(TAG, "Fan mode select received invalid option: %s", value.c_str());
        return;
      }

      // Forward to parent component
      this->parent_->set_fan_mode(value);
    }

    inline void ComfoAirTestModeSwitch::write_state(bool state)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Test mode switch has no parent component configured");
        return;
      }

      this->parent_->set_test_mode(state);
      this->publish_state(state);
    }

    inline void ComfoAirTestFanSpeedNumber::control(float value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Test fan speed number has no parent component configured");
        return;
      }

      uint8_t percent = static_cast<uint8_t>(value);
      if (!this->parent_->set_test_fan_speed(this->is_supply_fan_, percent))
      {
        // Revert to current value if setting failed
        uint8_t current = this->is_supply_fan_ ? this->parent_->test_supply_fan_speed_ : this->parent_->test_exhaust_fan_speed_;
        this->publish_state(current);
      }
      else
      {
        this->publish_state(percent);
      }
    }

    inline void ComfoAirTestOutputSwitch::write_state(bool state)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Test output switch has no parent component configured");
        return;
      }

      if (this->parent_->set_test_output(this->output_mask_, this->is_feedback_, state))
      {
        this->publish_state(state);
      }
      else
      {
        // Revert to previous state if command failed
        this->publish_state(!state);
      }
    }

    inline void ComfoAirTestFlapSelect::control(const std::string &value)
    {
      if (this->parent_ == nullptr)
      {
        ESP_LOGW(TAG, "Test flap select has no parent component configured");
        return;
      }

      // Map option to position value
      uint8_t position = 0;
      if (value == "Closed")
      {
        position = 0;
      }
      else if (value == "Open")
      {
        position = 1;
      }
      else if (value == "Stop")
      {
        position = 3;
      }
      else
      {
        ESP_LOGW(TAG, "Test flap select received invalid option: %s", value.c_str());
        return;
      }

      if (!this->parent_->set_test_flap(this->is_bypass_flap_, position))
      {
        // Revert to current value if setting failed
        uint8_t current = this->is_bypass_flap_ ? this->parent_->bypass_flap_state_ : this->parent_->preheating_flap_state_;
        const char *current_name = current == 0 ? "Closed" : (current == 1 ? "Open" : "Stop");
        this->publish_state(current_name);
      }
      else
      {
        this->publish_state(value);
      }
    }

  } // namespace comfoair
} // namespace esphome
