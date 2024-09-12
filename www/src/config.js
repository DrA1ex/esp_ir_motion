import {AppConfigBase} from "./lib/index.js";

export const DEFAULT_ADDRESS = "esp_motion.local";


export const PacketType = {
    POWER: 0x00,

    SYS_CONFIG_MDNS_NAME: 0x60,
    SYS_CONFIG_WIFI_MODE: 0x61,
    SYS_CONFIG_WIFI_SSID: 0x62,
    SYS_CONFIG_WIFI_PASSWORD: 0x63,
    SYS_CONFIG_WIFI_WIFI_CONNECTION_TIMEOUT: 0x64,

    SYS_CONFIG_MQTT_ENABLED: 0x70,
    SYS_CONFIG_MQTT_HOST: 0x71,
    SYS_CONFIG_MQTT_PORT: 0x72,
    SYS_CONFIG_MQTT_USER: 0x73,
    SYS_CONFIG_MQTT_PASSWORD: 0x74,
}

const CfgStrLength = 32;

export class Config extends AppConfigBase {
    power;

    sys_config;


    constructor(props) {
        super(props);

        this.lists["wifi_mode"] = [
            {code: 0, name: "AP"},
            {code: 1, name: "STA"},
        ]
    }


    parse(parser) {
        this.power = parser.readBoolean();

        this.sys_config = {
            mdns_name: parser.readFixedString(CfgStrLength),

            wifi_mode: parser.readUint8(),
            wifi_ssid: parser.readFixedString(CfgStrLength),
            wifi_password: parser.readFixedString(CfgStrLength),
            wifi_connection_timeout: parser.readUint32(),

            mqtt: parser.readBoolean(),
            mqtt_host: parser.readFixedString(CfgStrLength),
            mqtt_port: parser.readUint16(),
            mqtt_user: parser.readFixedString(CfgStrLength),
            mqtt_password: parser.readFixedString(CfgStrLength)
        }
    }
}