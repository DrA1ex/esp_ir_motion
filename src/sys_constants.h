#pragma once

#define BTN_SILENCE_INTERVAL                    (50u)
#define BTN_HOLD_INTERVAL                       (1000u)
#define BTN_PRESS_WAIT_INTERVAL                 (500u)
#define BTN_RESET_INTERVAL                      (1000u)

#define LED_BLINK_ACTIVE_DURATION               (60ul)
#define LED_BLINK_WAIT_DURATION                 (140ul)
#define LED_BLINK_COOL_DOWN_DURATION            (3000ul)

#define BUZZER_TONE_INTERVAL                    (10u)
#define APP_LOOP_INTERVAL                       (10u)

#define PWM_RESOLUTION                          (8u)
#define PWM_FREQUENCY                           (22000u)

#define CONFIG_STRING_SIZE                      (32u)

#define MOTION_LED_ACTIVE_COLOR                 (Color{64, 0, 0})
#define MOTION_LED_SILENT_COLOR                 (Color{0, 3, 0})

#define MOTION_LED_PANIC_COLORS                 {Color{255, 0, 0}, Color{0, 255, 0}, Color{0, 0, 255}}

#define MOTION_ALARM_MELODY                     {Note{650,  250}, Note{1350, 250}}