#pragma once

#include <lib/base/metadata.h>
#include <lib/utils/metadata.h>

#include "misc/motion.h"
#include "type.h"

DECLARE_META_TYPE(AppMetaProperty, PropertyEnum)

DECLARE_META(MotionConfigMeta, AppMetaProperty,
             MEMBER(Parameter<uint8_t>, motion_pin),
             MEMBER(Parameter<bool>, buzzer_enabled),
             MEMBER(Parameter<uint8_t>, buzzer_pin),
             MEMBER(Parameter<bool>, led_enabled),
             MEMBER(Parameter<uint8_t>, led_r_pin),
             MEMBER(Parameter<uint8_t>, led_g_pin),
             MEMBER(Parameter<uint8_t>, led_b_pin),
             MEMBER(Parameter<bool>, button_enabled),
             MEMBER(Parameter<uint8_t>, button_pin)
)

DECLARE_META(SysConfigMeta, AppMetaProperty,
             MEMBER(FixedString, mdns_name),
             MEMBER(Parameter<uint8_t>, wifi_mode),
             MEMBER(FixedString, wifi_ssid),
             MEMBER(FixedString, wifi_password),
             MEMBER(Parameter<uint32_t>, wifi_connection_timeout),
             MEMBER(Parameter<bool>, mqtt_enabled),
             MEMBER(FixedString, mqtt_host),
             MEMBER(Parameter<uint16_t>, mqtt_port),
             MEMBER(FixedString, mqtt_user),
             MEMBER(FixedString, mqtt_password),
             MEMBER(Parameter<uint16_t>, buzz_time),
             MEMBER(Parameter<uint32_t>, silent_time),
             MEMBER(Parameter<uint16_t>, idle_flash_time),
             MEMBER(Parameter<uint16_t>, panic_color_time)
)

DECLARE_META(DataConfigMeta, AppMetaProperty,
             MEMBER(ComplexParameter<Config>, config),
)

DECLARE_META(GeneratedConfigMeta, AppMetaProperty,
             MEMBER(GeneratedParameter<AppState>, app_state),
             MEMBER(GeneratedParameter<uint8_t>, sensor_state),
             MEMBER(GeneratedParameter<unsigned long>, sensor_silence_time_left),

             MEMBER(GeneratedParameter<uint8_t>, sensor_state_alarm),
             MEMBER(GeneratedParameter<uint8_t>, sensor_state_pause),
)

DECLARE_META(ConfigMetadata, AppMetaProperty,
             MEMBER(Parameter<bool>, power),
             SUB_TYPE(MotionConfigMeta, motion_config),
             SUB_TYPE(SysConfigMeta, sys_config),
             SUB_TYPE(DataConfigMeta, data),
             SUB_TYPE(GeneratedConfigMeta, generated)
)

inline ConfigMetadata build_metadata(Config &config, MotionControl &motion_control) {
    return {
            .power  = {
                    PropertyEnum::POWER,
                    MQTT_TOPIC_POWER, MQTT_OUT_TOPIC_POWER,
                    &config.power
            },
            .motion_config = {
                    .motion_pin = {
                            PropertyEnum::MOTION_CONFIG_MOTION_PIN,
                            &config.motion_config.motion_pin
                    },
                    .buzzer_enabled = {
                            PropertyEnum::MOTION_CONFIG_BUZZER_ENABLED,
                            &config.motion_config.buzzer_enabled
                    },
                    .buzzer_pin = {
                            PropertyEnum::MOTION_CONFIG_BUZZER_PIN,
                            &config.motion_config.buzzer_pin
                    },
                    .led_enabled = {
                            PropertyEnum::MOTION_CONFIG_LED_ENABLED,
                            &config.motion_config.led_enabled
                    },
                    .led_r_pin = {
                            PropertyEnum::MOTION_CONFIG_LED_R_PIN,
                            &config.motion_config.led_r_pin
                    },
                    .led_g_pin = {
                            PropertyEnum::MOTION_CONFIG_LED_G_PIN,
                            &config.motion_config.led_g_pin
                    },
                    .led_b_pin = {
                            PropertyEnum::MOTION_CONFIG_LED_B_PIN,
                            &config.motion_config.led_b_pin
                    },
                    .button_enabled = {
                            PropertyEnum::MOTION_CONFIG_BUTTON_ENABLED,
                            &config.motion_config.button_enabled
                    },
                    .button_pin = {
                            PropertyEnum::MOTION_CONFIG_BUTTON_PIN,
                            &config.motion_config.button_pin
                    },
            },
            .sys_config ={
                    .mdns_name ={
                            PropertyEnum::SYS_CONFIG_MDNS_NAME,
                            {config.sys_config.mdns_name, CONFIG_STRING_SIZE}
                    },
                    .wifi_mode = {
                            PropertyEnum::SYS_CONFIG_WIFI_MODE,
                            (uint8_t *) &config.sys_config.wifi_mode
                    },
                    .wifi_ssid = {
                            PropertyEnum::SYS_CONFIG_WIFI_SSID,
                            {config.sys_config.wifi_ssid, CONFIG_STRING_SIZE}
                    },
                    .wifi_password = {
                            PropertyEnum::SYS_CONFIG_WIFI_PASSWORD,
                            {config.sys_config.wifi_password, CONFIG_STRING_SIZE}
                    },
                    .wifi_connection_timeout = {
                            PropertyEnum::SYS_CONFIG_WIFI_CONNECTION_TIMEOUT,
                            &config.sys_config.wifi_connection_timeout
                    },
                    .mqtt_enabled = {
                            PropertyEnum::SYS_CONFIG_MQTT_ENABLED,
                            &config.sys_config.mqtt_enabled
                    },
                    .mqtt_host = {
                            PropertyEnum::SYS_CONFIG_MQTT_HOST,
                            {config.sys_config.mqtt_host, CONFIG_STRING_SIZE}
                    },
                    .mqtt_port = {
                            PropertyEnum::SYS_CONFIG_MQTT_PORT,
                            &config.sys_config.mqtt_port
                    },
                    .mqtt_user = {
                            PropertyEnum::SYS_CONFIG_MQTT_USER,
                            {config.sys_config.mqtt_user, CONFIG_STRING_SIZE}
                    },
                    .mqtt_password = {
                            PropertyEnum::SYS_CONFIG_MQTT_PASSWORD,
                            {config.sys_config.mqtt_password, CONFIG_STRING_SIZE}
                    },
                    .buzz_time = {
                            PropertyEnum::SYS_CONFIG_BUZZ_TIME,
                            &config.sys_config.buzz_time
                    },
                    .silent_time = {
                            PropertyEnum::SYS_CONFIG_SILENT_TIME,
                            &config.sys_config.silent_time
                    },
                    .idle_flash_time = {
                            PropertyEnum::SYS_CONFIG_IDLE_FLASH_TIME,
                            &config.sys_config.idle_flash_time
                    },
                    .panic_color_time = {
                            PropertyEnum::SYS_CONFIG_PANIC_COLOR_TIME,
                            &config.sys_config.panic_color_time
                    },
            },

            .data {
                    .config = ComplexParameter(&config),
            },

            .generated {
                    .app_state = {{[&] {
                        return AppState{
                                .state = motion_control.state(),
                                .silence_time_left = motion_control.silence_time_left(),
                        };
                    }}},

                    .sensor_state = {{([&] {
                        return (uint8_t) motion_control.state();
                    })}},

                    .sensor_silence_time_left = {{([&] {
                        return motion_control.silence_time_left();
                    })}},

                    .sensor_state_alarm = {
                            MQTT_OUT_TOPIC_ALARM,
                            {([&]() -> uint8_t {
                                return motion_control.state() == MotionState::PANIC ? 1 : 0;
                            })}
                    },
                    .sensor_state_pause = {
                            MQTT_OUT_TOPIC_PAUSE,
                            {([&]() -> uint8_t {
                                return motion_control.state() == MotionState::SILENT ? 1 : 0;
                            })}
                    },
            }
    };
}
