#pragma once

#include <cstdint>

#include <lib/base/application.h>
#include <lib/network/wifi.h>
#include <lib/utils/enum.h>

#include "credentials.h"
#include "constants.h"

MAKE_ENUM(PropertyEnum, uint8_t,
          POWER, 0x00,

          MOTION_CONFIG_BUZZER_ENABLED, 0x20,
          MOTION_CONFIG_BUZZER_PIN, 0x21,
          MOTION_CONFIG_LED_ENABLED, 0x22,
          MOTION_CONFIG_LED_R_PIN, 0x23,
          MOTION_CONFIG_LED_G_PIN, 0x24,
          MOTION_CONFIG_LED_B_PIN, 0x25,
          MOTION_CONFIG_BUTTON_ENABLED, 0x26,
          MOTION_CONFIG_BUTTON_PIN, 0x27,

          SYS_CONFIG_MDNS_NAME, 0x60,
          SYS_CONFIG_WIFI_MODE, 0x61,
          SYS_CONFIG_WIFI_SSID, 0x62,
          SYS_CONFIG_WIFI_PASSWORD, 0x63,
          SYS_CONFIG_WIFI_CONNECTION_TIMEOUT, 0x64,

          SYS_CONFIG_MQTT_ENABLED, 0x65,
          SYS_CONFIG_MQTT_HOST, 0x66,
          SYS_CONFIG_MQTT_PORT, 0x67,
          SYS_CONFIG_MQTT_USER, 0x68,
          SYS_CONFIG_MQTT_PASSWORD, 0x69,

          SYS_CONFIG_BUZZ_TIME, 0x6A,
          SYS_CONFIG_SILENT_TIME, 0x6B,
          SYS_CONFIG_IDLE_FLASH_TIME, 0x6C,
          SYS_CONFIG_PANIC_COLOR_TIME, 0x6D,
)

typedef PropertyMetadata<PropertyEnum> AppPropertyMeta;

typedef char __str[32];

struct __attribute((packed)) MotionConfig {
    uint8_t motion_pin = MOTION_PIN;

    bool buzzer_enabled = BUZZER_ENABLED;
    uint8_t buzzer_pin = BUZZER_PIN;

    bool led_enabled = LED_ENABLED;
    uint8_t led_r_pin = LED_R_PIN;
    uint8_t led_g_pin = LED_G_PIN;
    uint8_t led_b_pin = LED_B_PIN;

    bool button_enabled = BUTTON_ENABLED;
    uint8_t button_pin = BUTTON_PIN;
};

struct __attribute((packed)) SysConfig {
    __str mdns_name = {MSDN_NAME};

    WifiMode wifi_mode = WIFI_MODE;
    __str wifi_ssid = {WIFI_SSID};
    __str wifi_password = {WIFI_PASSWORD};

    uint32_t wifi_connection_timeout = WIFI_CONNECTION_TIMEOUT;

    bool mqtt_enabled = MQTT_ENABLED;
    __str mqtt_host = {MQTT_HOST};
    uint16_t mqtt_port = MQTT_PORT;
    __str mqtt_user = {MQTT_USER};
    __str mqtt_password = {MQTT_PASSWORD};

    uint16_t buzz_time = BUZZ_TIME;
    uint32_t silent_time = SILENT_TIME;
    uint16_t idle_flash_time = IDLE_FLASH_TIME;
    uint16_t panic_color_time = PANIC_COLOR_TIME;
};

struct __attribute((packed)) Config {
    bool power = true;

    MotionConfig motion_config{};
    SysConfig sys_config{};
};
