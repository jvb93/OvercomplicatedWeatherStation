# Sender and Bridge

These two devices, combined, form the IoT component of this project. The sender is intended to collect data and broadcast out over a LoRa radio protocol to the bridge. The bridge picks up these signals and forwards the data along to the API. The sender lives outside, out of range of my home's WiFi network, hence why the LoRa radio was chosen. The bridge is just passively awaiting signals from the sender.

## General Instructions

- assemble hardware
- compile and flash software
- place sender somewhere to collect accurate data (shade works best)

## Device Instructions

### The Sender

Required Hardware:

- TTGO T-Beam ESP32 LoRa device
- 18650 rechargeable LiOn battery
- BMP280 temperature/humidity/pressure sensor
- Optional solar panel to keep battery topped up

Required Software:

- LoRaSend.ino

### The Bridge

Required Hardware:

- TTGO ESP32 LoRa device

Required Software:

- LoRaReceive.ino
- Secrets.h with the following #defines:
  - WiFiSSID "your wifi network name"
  - WiFiPass "your wifi password"
  - ApiUrl "url of node.js api"
