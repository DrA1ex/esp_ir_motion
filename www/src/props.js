import {PacketType} from "./config.js";

/**
 * @type {PropertiesConfig}
 */
export const Properties = [{
    key: "general", section: "General", props: [
        {key: "power", title: "Power", type: "trigger", kind: "Boolean", cmd: PacketType.POWER}
    ],
}, {
    key: "motion_config", section: "Motion", collapse: "true", props: [
        {key: "motion_config.buzzer_enabled", title: "Buzzer Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.MOTION_CONFIG_BUZZER_ENABLED},
        {key: "motion_config.buzzer_pin", title: "Buzzer Pin", type: "int", kind: "Uint8", cmd: PacketType.MOTION_CONFIG_BUZZER_PIN, min: 0, max: 32},

        {key: "motion_config.led_enabled", title: "LED Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.MOTION_CONFIG_LED_ENABLED},
        {key: "motion_config.led_r_pin", title: "LED Red Pin", type: "int", kind: "Uint8", cmd: PacketType.MOTION_CONFIG_LED_R_PIN, min: 0, max: 32},
        {key: "motion_config.led_g_pin", title: "LED Green Pin", type: "int", kind: "Uint8", cmd: PacketType.MOTION_CONFIG_LED_G_PIN, min: 0, max: 32},
        {key: "motion_config.led_b_pin", title: "LED Blue Pin", type: "int", kind: "Uint8", cmd: PacketType.MOTION_CONFIG_LED_B_PIN, min: 0, max: 32},

        {key: "motion_config.button_enabled", title: "Button Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.MOTION_CONFIG_BUTTON_ENABLED},
        {key: "motion_config.button_pin", title: "Button Pin", type: "int", kind: "Uint8", cmd: PacketType.MOTION_CONFIG_BUTTON_PIN, min: 0, max: 32},
    ]
}, {
    key: "sys_config", section: "Settings", collapse: "true", props: [
        {key: "sys_config.mdns_name", title: "mDNS Name", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MDNS_NAME, maxLength: 32},

        {type: "title", label: "WiFi"},
        {key: "sys_config.wifi_mode", title: "WiFi Mode", type: "select", kind: "Uint8", cmd: PacketType.SYS_CONFIG_WIFI_MODE, list: "wifi_mode"},
        {key: "sys_config.wifi_ssid", title: "SSID", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_WIFI_SSID, maxLength: 32},

        {type: "title", label: "WiFi Extra"},
        {key: "sys_config.wifi_password", title: "Password", type: "password", kind: "FixedString", cmd: PacketType.SYS_CONFIG_WIFI_PASSWORD, maxLength: 32},
        {key: "sys_config.wifi_connection_timeout", title: "Connection Timeout", type: "int", kind: "Uint32", cmd: PacketType.SYS_CONFIG_WIFI_CONNECTION_TIMEOUT},

        {type: "title", label: "MQTT"},
        {key: "sys_config.mqtt_enabled", title: "MQTT Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.SYS_CONFIG_MQTT_ENABLED},
        {key: "sys_config.mqtt_host", title: "MQTT Host", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_HOST, maxLength: 32},
        {key: "sys_config.mqtt_port", title: "MQTT Port", type: "int", kind: "Uint16", cmd: PacketType.SYS_CONFIG_MQTT_PORT, min: 1, limit: 65535},
        {key: "sys_config.mqtt_user", title: "MQTT User", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_USER, maxLength: 32},
        {key: "sys_config.mqtt_password", title: "MQTT Password", type: "password", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_PASSWORD, maxLength: 32},

        {type: "title", label: "Alert Timing"},
        {key: "sys_config.buzz_time", title: "Buzz Time", type: "int", kind: "Uint16", cmd: PacketType.SYS_CONFIG_BUZZ_TIME},
        {key: "sys_config.silent_time", title: "Silent Time", type: "int", kind: "Uint32", cmd: PacketType.SYS_CONFIG_SILENT_TIME},
        {key: "sys_config.idle_flash_time", title: "Idle Flash Time", type: "int", kind: "Uint16", cmd: PacketType.SYS_CONFIG_IDLE_FLASH_TIME},
        {key: "sys_config.panic_color_time", title: "Panic Color Time", type: "int", kind: "Uint16", cmd: PacketType.SYS_CONFIG_PANIC_COLOR_TIME},

        {type: "title", label: "Actions", extra: {m_top: true}},
        {key: "apply_sys_config", type: "button", label: "Apply Settings"}
    ]
}];