#pragma once

#include <cstdint>

#include <lib/base/application.h>
#include <lib/utils/enum.h>

#include "credentials.h"
#include "constants.h"

MAKE_ENUM(PropertyEnum, uint8_t,
          POWER, 0x00,

          SYS_CONFIG_MDNS_NAME, 0x60,
          SYS_CONFIG_WIFI_MODE, 0x61,
          SYS_CONFIG_WIFI_SSID, 0x62,
          SYS_CONFIG_WIFI_PASSWORD, 0x63,
          SYS_CONFIG_WIFI_WIFI_CONNECTION_TIMEOUT, 0x64,

          SYS_CONFIG_MQTT_ENABLE, 0x70,
          SYS_CONFIG_MQTT_HOST, 0x71,
          SYS_CONFIG_MQTT_PORT, 0x72,
          SYS_CONFIG_MQTT_USER, 0x73,
          SYS_CONFIG_MQTT_PASSWORD, 0x74,
)

typedef PropertyMetadata<PropertyEnum> AppPropertyMeta;

typedef char __str[32];

struct __attribute((packed)) SysConfig {
    __str mdns_name = {MSDN_NAME};

    WifiMode wifi_mode = WIFI_MODE;
    __str wifi_ssid = {WIFI_SSID};
    __str wifi_password = {WIFI_PASSWORD};

    uint32_t  wifi_connection_timeout = WIFI_CONNECTION_TIMEOUT;

    bool mqtt_enabled = MQTT_ENABLED;
    __str mqtt_host = {MQTT_HOST};
    uint16_t mqtt_port = MQTT_PORT;
    __str mqtt_user = {MQTT_USER};
    __str mqtt_password = {MQTT_PASSWORD};
};

struct __attribute((packed)) Config {
    bool power = false;

    SysConfig sys_config{};
};
