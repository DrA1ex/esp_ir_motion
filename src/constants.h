#pragma once

#define WIFI_MODE                               (WifiMode::AP)
#define WIFI_CONNECTION_TIMEOUT                 (120000u)

#define RESTART_DELAY                           (500u)
#define STATE_NOTIFICATION_INTERVAL             (10000u)

#define BUZZ_TIME                               (3000ul)
#define SILENT_TIME                             (300000ul)
#define IDLE_FLASH_TIME                         (5000ul)
#define PANIC_COLOR_TIME                        (250ul)

#define MQTT_ENABLED                            (false)

#define MOTION_PIN                              (6u)

#define BUZZER_ENABLED                          (true)
#define BUZZER_PIN                              (3u)

#define LED_ENABLED                             (1u)
#define LED_R_PIN                               (2u)
#define LED_G_PIN                               (1u)
#define LED_B_PIN                               (0u)

#define BUTTON_ENABLED                          (false)
#define BUTTON_PIN                              (7u)


#define MQTT_PREFIX                             MDNS_NAME
#define MQTT_TOPIC_POWER                        MQTT_PREFIX "/power"
#define MQTT_TOPIC_SILENT                       MQTT_PREFIX "/silent"

#define MQTT_OUT_PREFIX                         MQTT_PREFIX "/out"
#define MQTT_OUT_TOPIC_POWER                    MQTT_OUT_PREFIX "/power"
#define MQTT_OUT_TOPIC_SILENT                   MQTT_OUT_PREFIX "/silent"
#define MQTT_OUT_TOPIC_ALARM                    MQTT_OUT_PREFIX "/alarm"