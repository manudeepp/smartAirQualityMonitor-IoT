# Kitchen Air Monitor - IoT

A smart air quality monitoring system using ESP32 and ESP8266 microcontrollers to measure volatile organic compounds (TVOCs), air quality index (AQI), and temperature. The system triggers a fan or other appliance when certain thresholds are met.

## Features
- Real-time air quality monitoring.
- Microcontroller-to-microcontroller wireless communication.
- No cloud dependency; fully local processing.
- Simple, cost-effective hardware setup.

## Components
- **ESP32** ([ESP32 Official Website](https://www.espressif.com/en/products/socs/esp32))
- **ESP8266** ([ESP8266 Official Website](https://www.espressif.com/en/products/socs/esp8266))
- **ENS160 + AHT20 Combo Module**
  ([ENS160 Datasheet](https://www.mouser.com/datasheet/2/1081/SC_001224_DS_1_ENS160_Datasheet_Rev_0_95-2258311.pdf?srsltid=AfmBOopJz7JQgQSGU6_fmkYoJMKtqg8rph8lY0g1R2S8r0DjvQqviONv), [AHT20 Datasheet](https://www.adafruit.com/product/4566))
([ENS160 + AHT20 Combo Module](https://www.amazon.ca/Dioxide-Temperature-Humidity-Replace-Arduino/dp/B0CSYHGMR8/ref=asc_df_B0CSYHGMR8/?tag=googleshopc0c-20&linkCode=df0&hvadid=716025042861&hvpos=&hvnetw=g&hvrand=12265749141506722913&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9001314&hvtargid=pla-2366664139089&psc=1&mcid=3914057e488b336fa4c6511c11c39695&gad_source=1))
- **5V Relay Module** ([General-purpose relay module](https://www.amazon.ca/Relay-Module-Optocoupler-Channel-Trigger/dp/B07WXJ9GTP/ref=sr_1_4?crid=2HCYQUWVBIL8F&dib=eyJ2IjoiMSJ9.YeOKE4dLWItnWDQxygPSgJhy5qXbxwri7QBLYuVaAd9lSI1ESHQ_T9Nyi5GMMSCZPYn_qNYBkceLH8JUF6THlWBE5x4CPBdjbyoOpySpV9Gd0y-reNCehA89VSNrWnNODpoQElr8awHPDfZzQJ1TdIdeRF2cVKtiZCFoaRZOXvvE3bQc6dkHaX62xbXzd23PV2i9D7gqGghBe5LIMK8FgnDJf4cx_wtwJ-U8utQyWLB4AFFMUZFCPUx7bYbk6_2ChQwhZvEP6-VrEdgGZ0uJ24ie1ThMsLXO9dGmgmqVZa4.QzVN9rOw5ft8x3J9Yi31cnoXZd7hQSVRt2YEiXJdmc0&dib_tag=se&keywords=5v%2Bsingle%2Bchannel%2Brelay&qid=1732131884&s=hi&sprefix=5v%2Bsingle%2Bchannel%2Brelay%2Ctools%2C104&sr=1-4&th=1))

## Libraries Required
- [SparkFun ENS160 Library](https://github.com/sparkfun/SparkFun_Indoor_Air_Quality_Sensor-ENS160_Arduino_Library)
- [Adafruit AHTX0 Library](https://github.com/adafruit/Adafruit_AHTX0)
- [ESP-NOW Documentation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_now.html)

## Workflow
1. **Sensor Detection**:  
   Use `sensor_diagnostics.ino` to verify the presence of the I2C module, which includes both the ENS160 and AHT20 sensors.
2. **Sensor Data Testing**:  
   Use `sensor_data_display.ino` to ensure the ENS160 and AHT20 sensors are outputting data correctly.
3. **MAC Address Discovery**:  
   Use `esp8266_mac_address_finder.ino` to find the MAC address of your ESP8266.
4. **Data Communication**:  
   Update the MAC address in `esp32_sender.ino` to match your ESP8266 and upload the code to the ESP32.
5. **Action Handling**:  
   Upload `esp8266_receiver.ino` to the ESP8266 to trigger the relay when conditions are met.

## Setup Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/manudeepp/KitchenAirMonitor-IoT.git
2. Follow the workflow steps in the order mentioned above.
3. Open and upload the .ino files from the Code folder using the Arduino IDE.

## Pin Connections

#### For ESP32 and ENS160 + AHT20 Module:

| Module                 | Pin (ESP32) | Pin (Sensor Module) |
|-------------------------|-------------|----------------------|
| ENS160 + AHT20 Module  | GPIO21      | SDA                  |
| ENS160 + AHT20 Module  | GPIO22      | SCL                  |
| ENS160 + AHT20 Module  | 3.3V        | VCC                  |
| ENS160 + AHT20 Module  | GND         | GND                  |

#### For ESP8266 and Relay Module:

| Module       | Pin (ESP8266) | Pin (Relay Module) |
|--------------|---------------|--------------------|
| Relay Module | GPIO4         | IN                 |
| Relay Module | 3.3V          | VCC                |
| Relay Module | GND           | GND                |

## How It Works

1. **Data Collection**:  
   The ESP32 gathers data from the combined ENS160 and AHT20 sensor module.

2. **Wireless Communication**:  
   Data is sent to ESP8266 using ESP-NOW.

3. **Action**:  
   If thresholds (e.g., AQI ≥ 3, temperature ≥ 30°C) are met, ESP8266 triggers a fan or other connected appliances via the relay module.

## Future Scope
- Extend support to additional air quality sensors to measure Carbon Monoxide or other harmful gases.
- Integrate smartphone notifications using an IoT framework like Blynk or Home Assistant.
- Add advanced machine learning models to predict air quality trends and preemptively trigger ventilation.
- Optimize energy consumption by implementing sleep modes for ESP32 and ESP8266.

---

## License
This project is licensed under the [MIT License](https://opensource.org/licenses/MIT) - feel free to use, modify, and distribute it as long as proper credit is given to the original author and contributors.

