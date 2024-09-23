# esp-ir-motion

Smart IR Motion Sensor based on ESP32/ESP8266

## UI

<img width="400" alt="image" src="https://github.com/user-attachments/assets/74e611aa-806a-4f24-8b23-0de276ac219f">


## Features
- Web/Mobile Application (PWA)
- Optional sound and visual (LED) alerting
- Integration with any Smart Home Assistant (such as Alise) via MQTT broker
- MQTT Protocol

## Connection

<img width="500" alt="image" src="https://github.com/user-attachments/assets/268893e3-76d1-4b24-b517-b7de0e2e3fee">


## Installation

### Manual Build / OTA


1. Install [Platform.IO](https://platformio.org/install):
2. (Optional) Modify the `credentials.h` file and, if necessary, customize `constants.h`. You can change these parameters later through the Web UI.
3. Upload filesystem and firmware

**Note:** This repository contains a submodule. Please use the `--recursive` option when cloning.

```bash
git clone --recursive https://github.com/DrA1ex/esp_ir_motion.git
cd esp_ir_motion

# Make script executable
chmod +x ./upload_fs.sh

# Specify the platform: esp32-c3 or esp8266
PLATFORM=esp32-c3

# Set the environment: debug, release, or ota
ENV=release

# For OTA: set your ESP's address
ADDRESS=esp_motion.local

# Additional envs if OTA enabled
if [ "$ENV" = "ota" ]; then OTA=1 else OTA=0 ADDRESS= fi

pio run -t upload -e $PLATFORM-$ENV --upload-port "$ADDRESS"
./upload_fs.sh --upload-port "$ADDRESS"
```


## MQTT Protocol

| Topic In *       			| Topic Out *          			| Type        | Values		         | Comments                              |
|---------------------------|-------------------------------|-------------|----------------------|---------------------------------------|
| `MQTT_TOPIC_POWER`		| `MQTT_OUT_TOPIC_POWER` 		| `uint8_t`   | 0..1      	     	 | Power _state: ON (1) / OFF (0)         |

\* Actual topic values decalred in `constants.h`


## Misc

### Configuring a Secure WebSocket Proxy with Nginx

If you're hosting a Web UI that uses SSL, you'll need to set up a Secure WebSocket (`wss://...`) server instead of the non-secure `ws://` provided by your ESP. Browsers require secure socket connections for WebSocket functionality, so this configuration is essential.

To achieve this, you can use Nginx as a proxy to create an SSL-enabled WebSocket connection.

#### Step 0: Install Nginx

```sh
apt install nginx
```

#### Step 1: Create the Nginx Configuration

Create a file at `/etc/nginx/conf.d/ws.locations` and add the following content:

```nginx
location /w_esp_ir_motion/ws {
    proxy_pass http://<YOUR-ESP-IP-HERE_1>/ws; # Replace with your actual service address
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection keep-alive;
    proxy_set_header Host $host;
}

# You can create proxy for multiple hosts
location /w_esp_ir_motion_2/ws {
    proxy_pass http://<YOUR-ESP-IP-HERE_2>/ws; # Replace with your actual service address
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection keep-alive;
    proxy_set_header Host $host;
}
```

#### Step 2: Reload the Nginx Configuration

After saving the configuration file, reload Nginx to apply the changes:

```sh
nginx -s reload
```

**Note**
Make sure to replace `<YOUR-ESP-IP-HERE_1>` and `<YOUR-ESP-IP-HERE_2>` with the actual IP addresses of your ESP devices.

#### Step 3: Check result

Open WebUi in browser https://dra1ex.github.io/esp_ir_motion/?host=ADDRESS/w_esp_ir_motion
