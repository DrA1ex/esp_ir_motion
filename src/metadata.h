#pragma once

#include <map>
#include <lib/utils/meta.h>

#include "type.h"

inline std::map<PropertyEnum, AppPropertyMeta> PropertyMetaMap = {
        define_meta_entry(PropertyEnum::POWER, &Config::power),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MDNS_NAME, &Config::sys_config, &SysConfig::mdns_name),

        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_MODE, &Config::sys_config, &SysConfig::wifi_mode),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_SSID, &Config::sys_config, &SysConfig::wifi_ssid),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_PASSWORD, &Config::sys_config, &SysConfig::wifi_password),
        define_meta_entry(PropertyEnum::SYS_CONFIG_WIFI_WIFI_CONNECTION_TIMEOUT, &Config::sys_config, &SysConfig::wifi_connection_timeout),

        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_ENABLE, &Config::sys_config, &SysConfig::mqtt_enabled),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_HOST, &Config::sys_config, &SysConfig::mqtt_host),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_PORT, &Config::sys_config, &SysConfig::mqtt_port),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_USER, &Config::sys_config, &SysConfig::mqtt_user),
        define_meta_entry(PropertyEnum::SYS_CONFIG_MQTT_PASSWORD, &Config::sys_config, &SysConfig::mqtt_password),
};