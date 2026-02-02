""" Creates module ComfoAir """

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, sensor, text_sensor, uart, climate, select, number, button, switch
from esphome.const import (CONF_ID, CONF_UART_ID, DEVICE_CLASS_CURRENT,
                           DEVICE_CLASS_EMPTY, DEVICE_CLASS_SPEED,
                           DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_VOLUME, DEVICE_CLASS_VOLTAGE,
                           STATE_CLASS_MEASUREMENT, UNIT_AMPERE, UNIT_CELSIUS,
                           UNIT_CUBIC_METER, UNIT_HOUR, UNIT_MINUTE, UNIT_VOLT,
                           UNIT_PERCENT, UNIT_REVOLUTIONS_PER_MINUTE, CONF_DISABLED_BY_DEFAULT,
                           DEVICE_CLASS_RESTART)

comfoair_ns = cg.esphome_ns.namespace("comfoair")
ComfoAirComponent = comfoair_ns.class_('ComfoAirComponent', climate.Climate, cg.Component, uart.UARTDevice)
ComfoAirVentilationLevelNumber = comfoair_ns.class_("ComfoAirVentilationLevelNumber", number.Number)
ComfoAirTimeDelayNumber = comfoair_ns.class_("ComfoAirTimeDelayNumber", number.Number)
ComfoAirResetErrorsButton = comfoair_ns.class_("ComfoAirResetErrorsButton", button.Button)
ComfoAirStartSelfTestButton = comfoair_ns.class_("ComfoAirStartSelfTestButton", button.Button)
ComfoAirPreheaterSwitch = comfoair_ns.class_("ComfoAirPreheaterSwitch", switch.Switch)
ComfoAirSizeSelect = comfoair_ns.class_("ComfoAirSizeSelect", select.Select)
ComfoAirRS232ModeSelect = comfoair_ns.class_("ComfoAirRS232ModeSelect", select.Select)
ComfoAirFanModeSelect = comfoair_ns.class_("ComfoAirFanModeSelect", select.Select)
ComfoAirTestModeSwitch = comfoair_ns.class_("ComfoAirTestModeSwitch", switch.Switch)
ComfoAirTestFanSpeedNumber = comfoair_ns.class_("ComfoAirTestFanSpeedNumber", number.Number)
ComfoAirTestOutputSwitch = comfoair_ns.class_("ComfoAirTestOutputSwitch", switch.Switch)
ComfoAirTestFlapSelect = comfoair_ns.class_("ComfoAirTestFlapSelect", select.Select)

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor", "climate", "binary_sensor", "text_sensor", "select", "number", "button", "switch"]
REQUIRED_KEY_NAME = "name"
CONF_HUB_ID = "comfoair"

UNIT_WEEK = "weeks"

CONF_TYPE = "type"
CONF_SIZE = "size"
CONF_SIZE_SELECT = "size_select"
CONF_RS232_MODE_SELECT = "rs232_mode_select"
CONF_FAN_MODE_SELECT = "fan_mode_select"
CONF_INTAKE_FAN_SPEED = "intake_fan_speed"
CONF_EXHAUST_FAN_SPEED = "exhaust_fan_speed"
CONF_INTAKE_FAN_SPEED_RPM = "intake_fan_speed_rpm"
CONF_EXHAUST_FAN_SPEED_RPM = "exhaust_fan_speed_rpm"
CONF_VENTILATION_LEVEL = "ventilation_level"
CONF_PREHEATING_STATE = "preheating_state"
CONF_OUTSIDE_AIR_TEMPERATURE = "outside_air_temperature"
CONF_SUPPLY_AIR_TEMPERATURE = "supply_air_temperature"
CONF_RETURN_AIR_TEMPERATURE = "return_air_temperature"
CONF_EXHAUST_AIR_TEMPERATURE = "exhaust_air_temperature"
CONF_ENTHALPY_TEMPERATURE = "enthalpy_temperature"
CONF_EWT_TEMPERATURE = "ewt_temperature"
CONF_POSTHEATING_TEMPERATURE = "postheating_temperature"
CONF_KITCHEN_HOOD_TEMPERATURE = "kitchen_hood_temperature"
CONF_RETURN_AIR_LEVEL = "return_air_level"
CONF_SUPPLY_AIR_LEVEL = "supply_air_level"
CONF_SUPPLY_FAN_ACTIVE = "supply_fan_active"
CONF_FILTER_STATUS = "filter_status"
CONF_BYPASS_PRESENT = "bypass_present"
CONF_ENTHALPY_PRESENT = "enthalpy_present"
CONF_EWT_PRESENT = "ewt_present"
CONF_OPTIONS_PRESENT = "options_present"
CONF_FIREPLACE_PRESENT = "fireplace_present"
CONF_KITCHEN_HOOD_PRESENT = "kitchen_hood_present"
CONF_POSTHEATING_PRESENT = "postheating_present"
CONF_POSTHEATING_PWM_MODE_PRESENT = "postheating_pwm_mode_present"
CONF_PREHEATING_PRESENT = "preheating_present"
CONF_BYPASS_VALVE = "bypass_valve"
CONF_BYPASS_VALVE_OPEN = "bypass_valve_open"
CONF_BYPASS_FACTOR = "bypass_factor"
CONF_BYPASS_STEP = "bypass_step"
CONF_BYPASS_CORRECTION = "bypass_correction"
CONF_BYPASS_OPEN_HOURS = "bypass_open_hours"
CONF_MOTOR_CURRENT_BYPASS = "motor_current_bypass"
CONF_MOTOR_CURRENT_PREHEATING = "motor_current_preheating"
CONF_PREHEATING_HOURS = "preheating_hours"
CONF_PREHEATING_VALVE = "preheating_valve"
CONF_LEVEL0_HOURS = "level0_hours"
CONF_LEVEL1_HOURS = "level1_hours"
CONF_LEVEL2_HOURS = "level2_hours"
CONF_LEVEL3_HOURS = "level3_hours"
CONF_FROST_PROTECTION_ACTIVE = "frost_protection_active"
CONF_FROST_PROTECTION_HOURS = "frost_protection_hours"
CONF_FROST_PROTECTION_MINUTES = "frost_protection_minutes"
CONF_FROST_PROTECTION_LEVEL = "frost_protection_level"
CONF_FILTER_HOURS = "filter_hours"
CONF_SUMMER_MODE = "summer_mode"

CONF_P10_ACTIVE = "p10_active"
CONF_P11_ACTIVE = "p11_active"
CONF_P12_ACTIVE = "p12_active"
CONF_P13_ACTIVE = "p13_active"
CONF_P14_ACTIVE = "p14_active"
CONF_P15_ACTIVE = "p15_active"
CONF_P16_ACTIVE = "p16_active"
CONF_P17_ACTIVE = "p17_active"
CONF_P18_ACTIVE = "p18_active"
CONF_P19_ACTIVE = "p19_active"
CONF_P90_ACTIVE = "p90_active"
CONF_P91_ACTIVE = "p91_active"
CONF_P92_ACTIVE = "p92_active"
CONF_P93_ACTIVE = "p93_active"
CONF_P94_ACTIVE = "p94_active"
CONF_P95_ACTIVE = "p95_active"
CONF_P96_ACTIVE = "p96_active"
CONF_P97_ACTIVE = "p97_active"

CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES = "bathroom_switch_on_delay_minutes"
CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES = "bathroom_switch_off_delay_minutes"
CONF_L1_SWITCH_OFF_DELAY_MINUTES = "l1_switch_off_delay_minutes"
CONF_BOOST_VENTILATION_MINUTES = "boost_ventilation_minutes"
CONF_FILTER_WARNING_WEEKS = "filter_warning_weeks"
CONF_RF_HIGH_TIME_SHORT_MINUTES = "rf_high_time_short_minutes"
CONF_RF_HIGH_TIME_LONG_MINUTES = "rf_high_time_long_minutes"
CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES = "extractor_hood_switch_off_delay_minutes"

# Error code binary sensors
CONF_ERROR_A0 = "error_a0"
CONF_ERROR_A1 = "error_a1"
CONF_ERROR_A2 = "error_a2"
CONF_ERROR_A3 = "error_a3"
CONF_ERROR_A4 = "error_a4"
CONF_ERROR_A5 = "error_a5"
CONF_ERROR_A6 = "error_a6"
CONF_ERROR_A7 = "error_a7"
CONF_ERROR_A8 = "error_a8"
CONF_ERROR_A9 = "error_a9"
CONF_ERROR_A10 = "error_a10"
CONF_ERROR_A11 = "error_a11"
CONF_ERROR_A12 = "error_a12"
CONF_ERROR_A13 = "error_a13"
CONF_ERROR_A14 = "error_a14"
CONF_ERROR_A15 = "error_a15"
CONF_ERROR_E1 = "error_e1"
CONF_ERROR_E2 = "error_e2"
CONF_ERROR_EA1 = "error_ea1"
CONF_ERROR_EA2 = "error_ea2"
CONF_ERROR_EA3 = "error_ea3"
CONF_ERROR_EA4 = "error_ea4"
CONF_ERROR_EA5 = "error_ea5"
CONF_ERROR_EA6 = "error_ea6"
CONF_ERROR_EA7 = "error_ea7"
CONF_ERROR_EA8 = "error_ea8"

# Ventilation level number components
CONF_SUPPLY_ABSENT_PERCENT = "supply_absent_percent"
CONF_SUPPLY_LOW_PERCENT = "supply_low_percent"
CONF_SUPPLY_MEDIUM_PERCENT = "supply_medium_percent"
CONF_SUPPLY_HIGH_PERCENT = "supply_high_percent"
CONF_EXHAUST_ABSENT_PERCENT = "exhaust_absent_percent"
CONF_EXHAUST_LOW_PERCENT = "exhaust_low_percent"
CONF_EXHAUST_MEDIUM_PERCENT = "exhaust_medium_percent"
CONF_EXHAUST_HIGH_PERCENT = "exhaust_high_percent"

# Time delay number components
CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES_NUMBER = "bathroom_switch_on_delay_minutes_number"
CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES_NUMBER = "bathroom_switch_off_delay_minutes_number"
CONF_L1_SWITCH_OFF_DELAY_MINUTES_NUMBER = "l1_switch_off_delay_minutes_number"
CONF_BOOST_VENTILATION_MINUTES_NUMBER = "boost_ventilation_minutes_number"
CONF_FILTER_WARNING_WEEKS_NUMBER = "filter_warning_weeks_number"
CONF_RF_HIGH_TIME_SHORT_MINUTES_NUMBER = "rf_high_time_short_minutes_number"
CONF_RF_HIGH_TIME_LONG_MINUTES_NUMBER = "rf_high_time_long_minutes_number"
CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES_NUMBER = "extractor_hood_switch_off_delay_minutes_number"

# Digital input binary sensors
CONF_INPUT_L1 = "input_l1"
CONF_INPUT_L2 = "input_l2"
CONF_INPUT_BATHROOM_SWITCH = "input_bathroom_switch"
CONF_INPUT_KITCHEN_HOOD_SWITCH = "input_kitchen_hood_switch"
CONF_INPUT_EXTERNAL_FILTER = "input_external_filter"
CONF_INPUT_WTW = "input_wtw"
CONF_INPUT_BATHROOM_SWITCH_2 = "input_bathroom_switch_2"

# Analog input sensors
CONF_ANALOG_INPUT_1 = "analog_input_1"
CONF_ANALOG_INPUT_2 = "analog_input_2"
CONF_ANALOG_INPUT_3 = "analog_input_3"
CONF_ANALOG_INPUT_4 = "analog_input_4"

# Button entities
CONF_RESET_ERRORS = "reset_errors"
CONF_START_SELF_TEST = "start_self_test"

# Switch entities
CONF_PREHEATER_PRESENT_SWITCH = "preheater_present_switch"

# Test mode entities
CONF_TEST_MODE_SWITCH = "test_mode_switch"
CONF_TEST_SUPPLY_FAN_NUMBER = "test_supply_fan_number"
CONF_TEST_EXHAUST_FAN_NUMBER = "test_exhaust_fan_number"
CONF_TEST_PREHEATING_RELAY_SWITCH = "test_preheating_relay_switch"
CONF_TEST_PREHEATING_TRIAC_SWITCH = "test_preheating_triac_switch"
CONF_TEST_KITCHEN_HOOD_SWITCH = "test_kitchen_hood_switch"
CONF_TEST_ERROR_LED_SWITCH = "test_error_led_switch"
CONF_TEST_FILTER_LED_SWITCH = "test_filter_led_switch"
CONF_TEST_BYPASS_FLAP_SELECT = "test_bypass_flap_select"
CONF_TEST_PREHEATING_FLAP_SELECT = "test_preheating_flap_select"

helper_comfoair = {
    "sensor": [
        CONF_INTAKE_FAN_SPEED,
        CONF_EXHAUST_FAN_SPEED,
        CONF_INTAKE_FAN_SPEED_RPM,
        CONF_EXHAUST_FAN_SPEED_RPM,
        CONF_VENTILATION_LEVEL,
        CONF_OUTSIDE_AIR_TEMPERATURE,
        CONF_SUPPLY_AIR_TEMPERATURE,
        CONF_RETURN_AIR_TEMPERATURE,
        CONF_EXHAUST_AIR_TEMPERATURE,
        CONF_ENTHALPY_TEMPERATURE,
        CONF_EWT_TEMPERATURE,
        CONF_POSTHEATING_TEMPERATURE,
        CONF_KITCHEN_HOOD_TEMPERATURE,
        CONF_RETURN_AIR_LEVEL,
        CONF_SUPPLY_AIR_LEVEL,
        CONF_BYPASS_VALVE,
        CONF_BYPASS_FACTOR,
        CONF_BYPASS_STEP,
        CONF_BYPASS_CORRECTION,
        CONF_BYPASS_OPEN_HOURS,
        CONF_MOTOR_CURRENT_BYPASS,
        CONF_MOTOR_CURRENT_PREHEATING,
        CONF_PREHEATING_HOURS,
        CONF_LEVEL0_HOURS,
        CONF_LEVEL1_HOURS,
        CONF_LEVEL2_HOURS,
        CONF_LEVEL3_HOURS,
        CONF_FROST_PROTECTION_HOURS,
        CONF_FROST_PROTECTION_MINUTES,
        CONF_FILTER_HOURS,
        CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES,
        CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES,
        CONF_L1_SWITCH_OFF_DELAY_MINUTES,
        CONF_BOOST_VENTILATION_MINUTES,
        CONF_FILTER_WARNING_WEEKS,
        CONF_RF_HIGH_TIME_SHORT_MINUTES,
        CONF_RF_HIGH_TIME_LONG_MINUTES,
        CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES,
        
        # Analog inputs
        CONF_ANALOG_INPUT_1,
        CONF_ANALOG_INPUT_2,
        CONF_ANALOG_INPUT_3,
        CONF_ANALOG_INPUT_4,
    ],
    "binary_sensor": [
        CONF_BYPASS_PRESENT,
        CONF_ENTHALPY_PRESENT,
        CONF_EWT_PRESENT,
        CONF_OPTIONS_PRESENT,
        CONF_FIREPLACE_PRESENT,
        CONF_KITCHEN_HOOD_PRESENT,
        CONF_POSTHEATING_PRESENT,
        CONF_POSTHEATING_PWM_MODE_PRESENT,
        CONF_PREHEATING_PRESENT,
        CONF_BYPASS_VALVE_OPEN,
        CONF_PREHEATING_STATE,
        CONF_SUMMER_MODE,
        CONF_SUPPLY_FAN_ACTIVE,
        CONF_FROST_PROTECTION_ACTIVE,

        CONF_P10_ACTIVE,
        CONF_P11_ACTIVE,
        CONF_P12_ACTIVE,
        CONF_P13_ACTIVE,
        CONF_P14_ACTIVE,
        CONF_P15_ACTIVE,
        CONF_P16_ACTIVE,
        CONF_P17_ACTIVE,
        CONF_P18_ACTIVE,
        CONF_P19_ACTIVE,
        CONF_P90_ACTIVE,
        CONF_P91_ACTIVE,
        CONF_P92_ACTIVE,
        CONF_P93_ACTIVE,
        CONF_P94_ACTIVE,
        CONF_P95_ACTIVE,
        CONF_P96_ACTIVE,
        CONF_P97_ACTIVE,
        
        # Error codes
        CONF_ERROR_A0,
        CONF_ERROR_A1,
        CONF_ERROR_A2,
        CONF_ERROR_A3,
        CONF_ERROR_A4,
        CONF_ERROR_A5,
        CONF_ERROR_A6,
        CONF_ERROR_A7,
        CONF_ERROR_A8,
        CONF_ERROR_A9,
        CONF_ERROR_A10,
        CONF_ERROR_A11,
        CONF_ERROR_A12,
        CONF_ERROR_A13,
        CONF_ERROR_A14,
        CONF_ERROR_A15,
        CONF_ERROR_E1,
        CONF_ERROR_E2,
        CONF_ERROR_EA1,
        CONF_ERROR_EA2,
        CONF_ERROR_EA3,
        CONF_ERROR_EA4,
        CONF_ERROR_EA5,
        CONF_ERROR_EA6,
        CONF_ERROR_EA7,
        CONF_ERROR_EA8,
        
        # Digital inputs
        CONF_INPUT_L1,
        CONF_INPUT_L2,
        CONF_INPUT_BATHROOM_SWITCH,
        CONF_INPUT_KITCHEN_HOOD_SWITCH,
        CONF_INPUT_EXTERNAL_FILTER,
        CONF_INPUT_WTW,
        CONF_INPUT_BATHROOM_SWITCH_2,
    ],
    "text_sensor": [
        CONF_TYPE,
        CONF_SIZE,
        CONF_FILTER_STATUS,
        CONF_FROST_PROTECTION_LEVEL,
        CONF_PREHEATING_VALVE,
    ],
    "select": [
        CONF_SIZE_SELECT,
        CONF_RS232_MODE_SELECT,
        CONF_FAN_MODE_SELECT,
        CONF_TEST_BYPASS_FLAP_SELECT,
        CONF_TEST_PREHEATING_FLAP_SELECT,
    ],
    "number": [
        # Ventilation levels
        CONF_SUPPLY_ABSENT_PERCENT,
        CONF_SUPPLY_LOW_PERCENT,
        CONF_SUPPLY_MEDIUM_PERCENT,
        CONF_SUPPLY_HIGH_PERCENT,
        CONF_EXHAUST_ABSENT_PERCENT,
        CONF_EXHAUST_LOW_PERCENT,
        CONF_EXHAUST_MEDIUM_PERCENT,
        CONF_EXHAUST_HIGH_PERCENT,
        
        # Time delays
        CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES_NUMBER,
        CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES_NUMBER,
        CONF_L1_SWITCH_OFF_DELAY_MINUTES_NUMBER,
        CONF_BOOST_VENTILATION_MINUTES_NUMBER,
        CONF_FILTER_WARNING_WEEKS_NUMBER,
        CONF_RF_HIGH_TIME_SHORT_MINUTES_NUMBER,
        CONF_RF_HIGH_TIME_LONG_MINUTES_NUMBER,
        CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES_NUMBER,
        
        # Test mode numbers
        CONF_TEST_SUPPLY_FAN_NUMBER,
        CONF_TEST_EXHAUST_FAN_NUMBER,
    ],
    "button": [
        CONF_RESET_ERRORS,
        CONF_START_SELF_TEST,
    ],
    "switch": [
        CONF_PREHEATER_PRESENT_SWITCH,
        CONF_TEST_MODE_SWITCH,
        CONF_TEST_PREHEATING_RELAY_SWITCH,
        CONF_TEST_PREHEATING_TRIAC_SWITCH,
        CONF_TEST_KITCHEN_HOOD_SWITCH,
        CONF_TEST_ERROR_LED_SWITCH,
        CONF_TEST_FILTER_LED_SWITCH,
    ],
}

comfoair_sensors_schemas = cv.Schema(
    {
        cv.Optional(CONF_TYPE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_SIZE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_FILTER_STATUS): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_FROST_PROTECTION_LEVEL): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_PREHEATING_VALVE): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_SIZE_SELECT, default={}): select.select_schema(
            ComfoAirSizeSelect
        ).extend(),
        cv.Optional(CONF_RS232_MODE_SELECT): select.select_schema(
            ComfoAirRS232ModeSelect
        ).extend(),
        cv.Optional(CONF_FAN_MODE_SELECT): select.select_schema(
            ComfoAirFanModeSelect
        ).extend(),

        cv.Optional(CONF_INTAKE_FAN_SPEED): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXHAUST_FAN_SPEED): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_INTAKE_FAN_SPEED_RPM): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXHAUST_FAN_SPEED_RPM): sensor.sensor_schema(
            device_class=DEVICE_CLASS_SPEED,
            unit_of_measurement=UNIT_REVOLUTIONS_PER_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_VENTILATION_LEVEL): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_OUTSIDE_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_SUPPLY_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RETURN_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXHAUST_AIR_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_ENTHALPY_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EWT_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_POSTHEATING_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_KITCHEN_HOOD_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RETURN_AIR_LEVEL): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_SUPPLY_AIR_LEVEL): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_CUBIC_METER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_FACTOR): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_STEP): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_CORRECTION): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BYPASS_OPEN_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_MOTOR_CURRENT_BYPASS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_MOTOR_CURRENT_PREHEATING): sensor.sensor_schema(
            device_class=DEVICE_CLASS_CURRENT,
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_PREHEATING_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL0_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL1_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL2_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_LEVEL3_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FROST_PROTECTION_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FROST_PROTECTION_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_MINUTE,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FILTER_HOURS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_BYPASS_VALVE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_L1_SWITCH_OFF_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_BOOST_VENTILATION_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_FILTER_WARNING_WEEKS): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_WEEK,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RF_HIGH_TIME_SHORT_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_RF_HIGH_TIME_LONG_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES): sensor.sensor_schema(
            device_class=DEVICE_CLASS_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),

        cv.Optional(CONF_FROST_PROTECTION_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_BYPASS_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_ENTHALPY_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_EWT_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_OPTIONS_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_FIREPLACE_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_KITCHEN_HOOD_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_POSTHEATING_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_POSTHEATING_PWM_MODE_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_PREHEATING_PRESENT): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_BYPASS_VALVE_OPEN): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_PREHEATING_STATE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_SUMMER_MODE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_SUPPLY_FAN_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),

        cv.Optional(CONF_P10_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P11_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P12_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P13_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P14_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P15_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P16_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P17_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P18_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P19_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P90_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P91_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P92_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P93_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P94_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P95_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P96_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        cv.Optional(CONF_P97_ACTIVE): binary_sensor.binary_sensor_schema(
            device_class=DEVICE_CLASS_EMPTY
        ).extend(),
        
        # Error code binary sensors
        cv.Optional(CONF_ERROR_A0): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A1): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A2): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A3): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A4): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A5): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A6): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A7): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A8): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A9): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A10): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A11): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A12): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A13): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A14): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_A15): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_E1): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_E2): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA1): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA2): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA3): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA4): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA5): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA6): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA7): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_ERROR_EA8): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        
        # Digital input binary sensors
        cv.Optional(CONF_INPUT_L1): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_INPUT_L2): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_INPUT_BATHROOM_SWITCH): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_INPUT_KITCHEN_HOOD_SWITCH): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_INPUT_EXTERNAL_FILTER): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_INPUT_WTW): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        cv.Optional(CONF_INPUT_BATHROOM_SWITCH_2): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_EMPTY).extend(),
        
        # Analog input sensors  
        cv.Optional(CONF_ANALOG_INPUT_1): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLTAGE,
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_ANALOG_INPUT_2): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLTAGE,
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_ANALOG_INPUT_3): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLTAGE,
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        cv.Optional(CONF_ANALOG_INPUT_4): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLTAGE,
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ).extend(),
        
        # Button entities
        cv.Optional(CONF_RESET_ERRORS): button.button_schema(
            ComfoAirResetErrorsButton,
            device_class=DEVICE_CLASS_RESTART,
        ).extend(),
        cv.Optional(CONF_START_SELF_TEST): button.button_schema(
            ComfoAirStartSelfTestButton,
            device_class=DEVICE_CLASS_RESTART,
        ).extend(),
        
        # Switch entities
        cv.Optional(CONF_PREHEATER_PRESENT_SWITCH): switch.switch_schema(
            ComfoAirPreheaterSwitch,
        ).extend(),
        
        # Test mode switch
        cv.Optional(CONF_TEST_MODE_SWITCH): switch.switch_schema(
            ComfoAirTestModeSwitch,
        ).extend(),
        
        # Test mode fan speed numbers
        cv.Optional(CONF_TEST_SUPPLY_FAN_NUMBER): number.number_schema(
            ComfoAirTestFanSpeedNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_TEST_EXHAUST_FAN_NUMBER): number.number_schema(
            ComfoAirTestFanSpeedNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        
        # Test mode relay/output switches
        cv.Optional(CONF_TEST_PREHEATING_RELAY_SWITCH): switch.switch_schema(
            ComfoAirTestOutputSwitch,
        ).extend(),
        cv.Optional(CONF_TEST_PREHEATING_TRIAC_SWITCH): switch.switch_schema(
            ComfoAirTestOutputSwitch,
        ).extend(),
        cv.Optional(CONF_TEST_KITCHEN_HOOD_SWITCH): switch.switch_schema(
            ComfoAirTestOutputSwitch,
        ).extend(),
        cv.Optional(CONF_TEST_ERROR_LED_SWITCH): switch.switch_schema(
            ComfoAirTestOutputSwitch,
        ).extend(),
        cv.Optional(CONF_TEST_FILTER_LED_SWITCH): switch.switch_schema(
            ComfoAirTestOutputSwitch,
        ).extend(),
        
        # Test mode flap selects
        cv.Optional(CONF_TEST_BYPASS_FLAP_SELECT): select.select_schema(
            ComfoAirTestFlapSelect,
        ).extend(),
        cv.Optional(CONF_TEST_PREHEATING_FLAP_SELECT): select.select_schema(
            ComfoAirTestFlapSelect,
        ).extend(),
        
        # Ventilation level number components
        cv.Optional(CONF_SUPPLY_ABSENT_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_SUPPLY_LOW_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_SUPPLY_MEDIUM_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_SUPPLY_HIGH_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_EXHAUST_ABSENT_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_EXHAUST_LOW_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_EXHAUST_MEDIUM_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_EXHAUST_HIGH_PERCENT): number.number_schema(
            ComfoAirVentilationLevelNumber,
            unit_of_measurement=UNIT_PERCENT,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        
        # Time delay number components
        cv.Optional(CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_L1_SWITCH_OFF_DELAY_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_BOOST_VENTILATION_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_FILTER_WARNING_WEEKS_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_WEEK,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_RF_HIGH_TIME_SHORT_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_RF_HIGH_TIME_LONG_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
        cv.Optional(CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES_NUMBER): number.number_schema(
            ComfoAirTimeDelayNumber,
            unit_of_measurement=UNIT_MINUTE,
            device_class=DEVICE_CLASS_EMPTY,
        ).extend(),
    }
)

CONFIG_SCHEMA = (
  climate.climate_schema(ComfoAirComponent)
  .extend(
    {
      cv.Required(REQUIRED_KEY_NAME): cv.string,
    }
  )
  .extend(uart.UART_DEVICE_SCHEMA)
  .extend(comfoair_sensors_schemas)
)

def to_code(config):
    """Generates code"""
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    yield climate.register_climate(var, config)
    cg.add(var.set_name(config[REQUIRED_KEY_NAME]))
    paren = yield cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_component(paren))
    
    # Handle ventilation level number components with parent and index
    ventilation_numbers = [
        (CONF_SUPPLY_ABSENT_PERCENT, 0),
        (CONF_SUPPLY_LOW_PERCENT, 1),
        (CONF_SUPPLY_MEDIUM_PERCENT, 2),
        (CONF_SUPPLY_HIGH_PERCENT, 3),
        (CONF_EXHAUST_ABSENT_PERCENT, 4),
        (CONF_EXHAUST_LOW_PERCENT, 5),
        (CONF_EXHAUST_MEDIUM_PERCENT, 6),
        (CONF_EXHAUST_HIGH_PERCENT, 7),
    ]
    
    for conf_key, level_index in ventilation_numbers:
        if conf_key in config:
            sens = yield number.new_number(
                config[conf_key],
                min_value=0.0,
                max_value=100.0,
                step=1.0
            )
            cg.add(sens.set_parent(var))
            cg.add(sens.set_level_index(level_index))
            func = getattr(var, "set_" + conf_key)
            cg.add(func(sens))
    
    # Handle time delay number components with parent and index
    time_delay_numbers = [
        (CONF_BATHROOM_SWITCH_ON_DELAY_MINUTES_NUMBER, 0),
        (CONF_BATHROOM_SWITCH_OFF_DELAY_MINUTES_NUMBER, 1),
        (CONF_L1_SWITCH_OFF_DELAY_MINUTES_NUMBER, 2),
        (CONF_BOOST_VENTILATION_MINUTES_NUMBER, 3),
        (CONF_FILTER_WARNING_WEEKS_NUMBER, 4),
        (CONF_RF_HIGH_TIME_SHORT_MINUTES_NUMBER, 5),
        (CONF_RF_HIGH_TIME_LONG_MINUTES_NUMBER, 6),
        (CONF_EXTRACTOR_HOOD_SWITCH_OFF_DELAY_MINUTES_NUMBER, 7),
    ]
    
    for conf_key, delay_index in time_delay_numbers:
        if conf_key in config:
            # Different ranges based on delay type
            if delay_index in [0, 1, 2, 5, 7]:  # Minutes (0-60)
                sens = yield number.new_number(
                    config[conf_key],
                    min_value=0.0,
                    max_value=60.0,
                    step=1.0
                )
            elif delay_index == 3:  # Boost (0-120 minutes)
                sens = yield number.new_number(
                    config[conf_key],
                    min_value=0.0,
                    max_value=120.0,
                    step=1.0
                )
            elif delay_index == 4:  # Filter warning (1-52 weeks)
                sens = yield number.new_number(
                    config[conf_key],
                    min_value=1.0,
                    max_value=52.0,
                    step=1.0
                )
            elif delay_index == 6:  # RF high time long (0-120 minutes)
                sens = yield number.new_number(
                    config[conf_key],
                    min_value=0.0,
                    max_value=120.0,
                    step=1.0
                )
            cg.add(sens.set_parent(var))
            cg.add(sens.set_delay_index(delay_index))
            func = getattr(var, "set_" + conf_key)
            cg.add(func(sens))
    
    # Handle test mode fan speed number components
    if CONF_TEST_SUPPLY_FAN_NUMBER in config:
        sens = yield number.new_number(
            config[CONF_TEST_SUPPLY_FAN_NUMBER],
            min_value=0.0,
            max_value=100.0,
            step=1.0
        )
        cg.add(sens.set_parent(var))
        cg.add(sens.set_is_supply_fan(True))
        cg.add(var.set_test_supply_fan_number(sens))
    
    if CONF_TEST_EXHAUST_FAN_NUMBER in config:
        sens = yield number.new_number(
            config[CONF_TEST_EXHAUST_FAN_NUMBER],
            min_value=0.0,
            max_value=100.0,
            step=1.0
        )
        cg.add(sens.set_parent(var))
        cg.add(sens.set_is_supply_fan(False))
        cg.add(var.set_test_exhaust_fan_number(sens))
    
    # Handle test mode output switches with bitmasks
    test_output_switches = [
        (CONF_TEST_PREHEATING_RELAY_SWITCH, 0x01, False),  # Byte[0] bit 0
        (CONF_TEST_PREHEATING_TRIAC_SWITCH, 0x02, False),   # Byte[0] bit 1
        (CONF_TEST_KITCHEN_HOOD_SWITCH, 0x10, False),       # Byte[0] bit 4
        (CONF_TEST_ERROR_LED_SWITCH, 0x20, False),          # Byte[0] bit 5
        (CONF_TEST_FILTER_LED_SWITCH, 0x01, True),          # Byte[1] bit 0
    ]
    
    for conf_key, mask, is_feedback in test_output_switches:
        if conf_key in config:
            sens = yield switch.new_switch(config[conf_key])
            cg.add(sens.set_parent(var))
            cg.add(sens.set_output_mask(mask, is_feedback))
            func = getattr(var, "set_" + conf_key)
            cg.add(func(sens))
    
    # Handle test mode flap selects
    if CONF_TEST_BYPASS_FLAP_SELECT in config:
        sens = yield select.new_select(config[CONF_TEST_BYPASS_FLAP_SELECT], options=["Closed", "Open", "Stop"])
        cg.add(sens.set_parent(var))
        cg.add(sens.set_is_bypass_flap(True))
        cg.add(var.set_test_bypass_flap_select(sens))
    
    if CONF_TEST_PREHEATING_FLAP_SELECT in config:
        sens = yield select.new_select(config[CONF_TEST_PREHEATING_FLAP_SELECT], options=["Closed", "Open", "Stop"])
        cg.add(sens.set_parent(var))
        cg.add(sens.set_is_bypass_flap(False))
        cg.add(var.set_test_preheating_flap_select(sens))
    
    # Handle test mode switch separately
    if CONF_TEST_MODE_SWITCH in config:
        sens = yield switch.new_switch(config[CONF_TEST_MODE_SWITCH])
        cg.add(sens.set_parent(var))
        cg.add(var.set_test_mode_switch(sens))
    
    # Handle all other components (sensors, binary_sensors, text_sensors, select, buttons, switches)
    for k, values in helper_comfoair.items():
        if k == "number":
            # Already handled above (ventilation levels, time delays, test mode fans)
            continue
        for v in values:
            if not v in config:
                continue
            # Skip test mode entities - they are handled separately above
            if v in [CONF_TEST_MODE_SWITCH, CONF_TEST_SUPPLY_FAN_NUMBER, CONF_TEST_EXHAUST_FAN_NUMBER,
                     CONF_TEST_PREHEATING_RELAY_SWITCH, CONF_TEST_PREHEATING_TRIAC_SWITCH,
                     CONF_TEST_KITCHEN_HOOD_SWITCH, CONF_TEST_ERROR_LED_SWITCH, CONF_TEST_FILTER_LED_SWITCH,
                     CONF_TEST_BYPASS_FLAP_SELECT, CONF_TEST_PREHEATING_FLAP_SELECT]:
                continue
            sens = None
            if k == "sensor":
                sens = yield sensor.new_sensor(config[v])
            elif k == "binary_sensor":
                sens = yield binary_sensor.new_binary_sensor(config[v])
            elif k == "text_sensor":
                sens = yield text_sensor.new_text_sensor(config[v])
            elif k == "select":
                if v == CONF_SIZE_SELECT:
                    sens = yield select.new_select(config[v], options=["Large", "Small"])
                elif v == CONF_RS232_MODE_SELECT:
                    sens = yield select.new_select(config[v], options=["End", "PC_Only", "PC_Master", "PC_Log_Mode"])
                elif v == CONF_FAN_MODE_SELECT:
                    sens = yield select.new_select(config[v], options=["Both", "Supply Only", "Exhaust Only", "Off"])
                else:
                    sens = yield select.new_select(config[v], options=[])
                # Select entities need parent set
                if sens is not None:
                    cg.add(sens.set_parent(var))
            elif k == "button":
                sens = yield button.new_button(config[v])
                # Buttons need parent set
                cg.add(sens.set_parent(var))
            elif k == "switch":
                sens = yield switch.new_switch(config[v])
                # Switches need parent set
                cg.add(sens.set_parent(var))
            if sens is not None:
                func = getattr(var, "set_" + v)
                cg.add(func(sens))
