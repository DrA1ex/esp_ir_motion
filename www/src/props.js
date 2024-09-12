import {PacketType} from "./config.js";

/**
 * @type {PropertiesConfig}
 */
export const Properties = [{
    key: "general", section: "General", props: [
        {key: "power", title: "Power", type: "trigger", kind: "Boolean", cmd: PacketType.POWER}
    ],
}, {
    key: "settings", section: "Settings", collapse: "true", props: [
        {key: "sys_config.mdns_name", title: "mDNS name", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MDNS_NAME, maxLength: 32},

        {type: "title", label: "WiFi"},
        {key: "sys_config.wifi_mode", title: "Mode", type: "select", kind: "Uint8", cmd: PacketType.SYS_CONFIG_WIFI_MODE, list: "wifi_mode"},
        {key: "sys_config.wifi_ssid", title: "SSID", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_WIFI_SSID, maxLength: 32},
        {key: "sys_config.wifi_password", title: "Password", type: "password", kind: "FixedString", cmd: PacketType.SYS_CONFIG_WIFI_PASSWORD, maxLength: 32},

        {type: "title", label: "WiFi Extra"},
        {key: "sys_config.wifi_connection_timeout", title: "Connection Timeout", type: "int", kind: "Uint32", cmd: PacketType.SYS_CONFIG_WIFI_WIFI_CONNECTION_TIMEOUT},


        {type: "title", label: "MQTT"},
        {key: "sys_config.mqtt", title: "Enabled", type: "trigger", kind: "Boolean", cmd: PacketType.SYS_CONFIG_MQTT_ENABLED},
        {key: "sys_config.mqtt_host", title: "Host", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_HOST, maxLength: 32},
        {key: "sys_config.mqtt_port", title: "Port", type: "int", kind: "Uint16", cmd: PacketType.SYS_CONFIG_MQTT_PORT, min: 1, limit: 65535},
        {key: "sys_config.mqtt_user", title: "User", type: "text", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_USER, maxLength: 32},
        {key: "sys_config.mqtt_password", title: "Password", type: "password", kind: "FixedString", cmd: PacketType.SYS_CONFIG_MQTT_PASSWORD, maxLength: 32},

        {type: "title", label: "Actions", extra: {m_top: true}},
        {key: "apply_sys_config", type: "button", label: "Apply Settings"}
    ]
}];