# Overcomplicated Weather Station

![CI](https://github.com/jvb93/OvercomplicatedWeatherStation/workflows/CI/badge.svg)

[See it in action](http://jvbuntu2.asuscomm.com:8080/)

## What is it?

This is an overly-complicated solution to a problem that doesn't really exist - the need to know the weather on a minute-by-minute basis.

Actually, it's an attempt to learn some interesting IoT concepts and gather a LOT of data for my data science experiments. I built a system of devices which collects ambient weather data and stores for future use.

Please see the individual subfolders in this repository for detailed information on each component

## What's involved?

### Diagram

!['diagram'](/images/diagram.png)

### The Sender

The sender is a TTGO T-Beam - an ESP32-based MCU which contains a ton of really interesting stuff like:

- LoRa long range packet radio comms
- 18650 rechargable battery support, power management
- WiFi, Bluetooth, GPS
- Dual core CPU

Additional parts:

- BMP280 Temperature, Humidity, and Pressure sensor
- Small solar panel

The sender's job is to collect weather data and blast it over LoRa to anyone who's listening.

### The Bridge

The Bridge's job is to receive LoRa packets from the sender and forward the data along to the API. The sender lives outdoors, out of range of my home WiFi - so a bridge is necessary to listen for long range data, connect to WiFi, and post to the appropriate endpoints.

Again, this is a TTGO ESP32-based MCU, but it doesn't have as many cool features as the sender. It just has the bare minimum to receive data and process.

### The API

The API is an Express server on top of Node.js. This runs on a Docker host on my LAN. It is the broker between the bridge (and other clients) and my Postgres database, also running in a container.

### The Frontend

The Frontend is written in Vue.js using Chart.js for data visualization and Bulma for css
