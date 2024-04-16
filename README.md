# Smart Hall Monitoring System

This repository contains the code for a smart hall monitoring system implemented using Arduino. The system utilizes various sensors and actuators to monitor and control the temperature of the hall, detect the number of people entering and exiting the hall, and sound alarms when certain conditions are met.

## Components Used
- Arduino board (compatible with ESP8266 or ESP32)
- DHT temperature and humidity sensor
- Passive Infrared (PIR) motion sensors for entry and exit detection
- Buzzer for alarms
- Liquid Crystal Display (LCD) for local display of data
- Wi-Fi module for internet connectivity
- Relay module for controlling appliances (chiller in this case)

## Functionality
The system performs the following functions:
1. **Temperature Monitoring:** The DHT sensor is used to measure the temperature of the hall. If the temperature exceeds a certain threshold, the chiller is turned on to maintain a comfortable temperature.

2. **People Counting:** PIR motion sensors are installed at the entry and exit points of the hall to count the number of people entering and exiting. This information is used to determine the occupancy of the hall.

3. **Alarm System:** When the number of people in the hall reaches predefined thresholds, alarms are sounded using the buzzer to indicate overcrowding.

4. **Data Logging:** Data including temperature and occupancy status is periodically logged and sent to the ThingSpeak platform for visualization and analysis.

## Setup Instructions
1. **Hardware Setup:** Connect the sensors and actuators according to the provided pin configuration in the code.

2. **Wi-Fi Configuration:** Update the SSID and password variables with your Wi-Fi credentials to enable internet connectivity.

3. **ThingSpeak Integration:** Provide your ThingSpeak channel ID and write API key to enable data logging to the ThingSpeak platform.

4. **Upload Code:** Upload the provided Arduino sketch to your Arduino board using the Arduino IDE or any compatible development environment.

5. **Monitor Data:** Once the system is powered on, you can monitor the temperature and occupancy data on the LCD display and on the ThingSpeak platform.

## Note
- Ensure that you have installed the necessary libraries (`DHT`, `ThingSpeak`, `SoftwareSerial`, `Wire`, `LiquidCrystal_I2C`) before uploading the code.
- This code is specifically tailored for ESP8266 or ESP32 boards. Adjustments may be required for compatibility with other Arduino boards.
