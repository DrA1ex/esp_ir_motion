#pragma once

#include <map>
#include <lib/utils/meta.h>

#include "type.h"

inline std::map<PropertyEnum, AppPropertyMeta> PropertyMetaMap = {
        define_meta_entry(PropertyEnum::POWER, &Config::power),

        define_meta_entry(PropertyEnum::MOTION_CONFIG_MOTION_PIN, &Config::motion_config, &MotionConfig::motion_pin),

        define_meta_entry(PropertyEnum::MOTION_CONFIG_BUZZER_ENABLED, &Config::motion_config, &MotionConfig::buzzer_enabled),
        define_meta_entry(PropertyEnum::MOTION_CONFIG_BUZZER_PIN, &Config::motion_config, &MotionConfig::buzzer_pin),

        define_meta_entry(PropertyEnum::MOTION_CONFIG_LED_ENABLED, &Config::motion_config, &MotionConfig::led_enabled),
        define_meta_entry(PropertyEnum::MOTION_CONFIG_LED_R_PIN, &Config::motion_config, &MotionConfig::led_r_pin),
        define_meta_entry(PropertyEnum::MOTION_CONFIG_LED_G_PIN, &Config::motion_config, &MotionConfig::led_g_pin),
        define_meta_entry(PropertyEnum::MOTION_CONFIG_LED_B_PIN, &Config::motion_config, &MotionConfig::led_b_pin),

        define_meta_entry(PropertyEnum::MOTION_CONFIG_BUTTON_ENABLED, &Config::motion_config, &MotionConfig::button_enabled),
        define_meta_entry(PropertyEnum::MOTION_CONFIG_BUTTON_PIN, &Config::motion_config, &MotionConfig::button_pin),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MDNS_NAME, &Config::sys_config, &SysConfig::mdns_name),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_MODE, &Config::sys_config, &SysConfig::wifi_mode),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_SSID, &Config::sys_config, &SysConfig::wifi_ssid),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_PASSWORD, &Config::sys_config, &SysConfig::wifi_password),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_CONNECTION_TIMEOUT, &Config::sys_config, &SysConfig::wifi_connection_timeout),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_ENABLED, &Config::sys_config, &SysConfig::mqtt_enabled),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_HOST, &Config::sys_config, &SysConfig::mqtt_host),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_PORT, &Config::sys_config, &SysConfig::mqtt_port),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_USER, &Config::sys_config, &SysConfig::mqtt_user),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_PASSWORD, &Config::sys_config, &SysConfig::mqtt_password),

        define_meta_entry(PropertyEnum::SYS_CONFIG_BUZZ_TIME, &Config::sys_config, &SysConfig::buzz_time),
        define_meta_entry(PropertyEnum::SYS_CONFIG_SILENT_TIME, &Config::sys_config, &SysConfig::silent_time),
        define_meta_entry(PropertyEnum::SYS_CONFIG_IDLE_FLASH_TIME, &Config::sys_config, &SysConfig::idle_flash_time),
        define_meta_entry(PropertyEnum::SYS_CONFIG_PANIC_COLOR_TIME, &Config::sys_config, &SysConfig::panic_color_time),
};