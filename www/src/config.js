import {AppConfigBase} from "./lib/index.js";

export const DEFAULT_ADDRESS = "esp_motion.local";


export const PacketType = {
    POWER: 0x00,

    SILENCE: 0x10,
    SILENCE_RESET: 0x11,

    MOTION_CONFIG_BUZZER_ENABLED: 0x20,
    MOTION_CONFIG_BUZZER_PIN: 0x21,
    MOTION_CONFIG_LED_ENABLED: 0x22,
    MOTION_CONFIG_LED_R_PIN: 0x23,
    MOTION_CONFIG_LED_G_PIN: 0x24,
    MOTION_CONFIG_LED_B_PIN: 0x25,
    MOTION_CONFIG_BUTTON_ENABLED: 0x26,
    MOTION_CONFIG_BUTTON_PIN: 0x27,
    MOTION_CONFIG_MOTION_PIN: 0x28,

    SYS_CONFIG_MDNS_NAME: 0x60,
    SYS_CONFIG_WIFI_MODE: 0x61,
    SYS_CONFIG_WIFI_SSID: 0x62,
    SYS_CONFIG_WIFI_PASSWORD: 0x63,
    SYS_CONFIG_WIFI_CONNECTION_TIMEOUT: 0x64,
    SYS_CONFIG_MQTT_ENABLED: 0x65,
    SYS_CONFIG_MQTT_HOST: 0x66,
    SYS_CONFIG_MQTT_PORT: 0x67,
    SYS_CONFIG_MQTT_USER: 0x68,
    SYS_CONFIG_MQTT_PASSWORD: 0x69,

    SYS_CONFIG_BUZZ_TIME: 0x6A,
    SYS_CONFIG_SILENT_TIME: 0x6B,
    SYS_CONFIG_IDLE_FLASH_TIME: 0x6C,
    SYS_CONFIG_PANIC_COLOR_TIME: 0x6D,

    MOTION_STATE: 0x80,
    MOTION_SILENCE_TIME_LEFT: 0x81,

    GET_STATE: 0xa0,
}

const CfgStrLength = 32;

export class Config extends AppConfigBase {
    power;
    motion_config;
    sys_config;

    status;

    constructor(props) {
        super(props);

        this.lists["wifi_mode"] = [
            {code: 0, name: "AP"},
            {code: 1, name: "STA"},
        ]
    }

    async load(ws) {
        const [_, statusPacket] = await Promise.all([
            super.load(ws),
            ws.request(PacketType.GET_STATE),
        ]);

        this.status = this.#parseStatus(statusPacket.parser());
    }

    parse(parser) {
        this.power = parser.readBoolean();
        this.motion_config = this.#parseMotionConfig(parser);
        this.sys_config = this.#parseSysConfig(parser);
    }

    #parseMotionConfig(parser) {
        return {
            motion_pin: parser.readUint8(),

            buzzer_enabled: parser.readBoolean(),
            buzzer_pin: parser.readUint8(),

            led_enabled: parser.readBoolean(),
            led_r_pin: parser.readUint8(),
            led_g_pin: parser.readUint8(),
            led_b_pin: parser.readUint8(),

            button_enabled: parser.readBoolean(),
            button_pin: parser.readUint8(),
        };
    }

    #parseSysConfig(parser) {
        return {
            mdns_name: parser.readFixedString(32),
            wifi_mode: parser.readUint8(),
            wifi_ssid: parser.readFixedString(32),
            wifi_password: parser.readFixedString(32),
            wifi_connection_timeout: parser.readUint32(),

            mqtt_enabled: parser.readBoolean(),
            mqtt_host: parser.readFixedString(32),
            mqtt_port: parser.readUint16(),
            mqtt_user: parser.readFixedString(32),
            mqtt_password: parser.readFixedString(32),

            buzz_time: parser.readUint16(),
            silent_time: parser.readUint32(),
            idle_flash_time: parser.readUint16(),
            panic_color_time: parser.readUint16(),
        };
    }

    #parseStatus(parser) {
        return {
            state: parser.readUint8(),
            silence_left: parser.readUint32()
        };
    }
}