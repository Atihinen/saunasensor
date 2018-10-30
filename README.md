# saunasensor

Wemos based saunasensor to monitor sauna temperature in own node network

![Network diagram]((https://raw.githubusercontent.com/Atihinen/saunasensor/master/docs/network.png)

# Sauna temperature reader

## Requirements

### Software

* Arduino IDE
* [OneWire library](http://playground.arduino.cc/Learning/OneWire)
* ArduinoJson library (6.5.0-beta)

## Hardware

* Wemos D1 mini
* DS18B20
* jumper cables x 3
* resistor 4.75k ohm (or resistor module)

![Temperature reader diagram](https://raw.githubusercontent.com/Atihinen/saunasensor/master/docs/sauna_reader_bb.png)

# Sauna temperature display

## Requirements

### Software

* Arduino IDE
* ArduinoJson library (6.5.0-beta)
* [LiquidCrystal_I2C library](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)

### Hardware

* Wemos D1 mini
* LCD display with I2C pack
* jumper cables x 4

![Display diagram](https://raw.githubusercontent.com/Atihinen/saunasensor/master/docs/sauna_display_bb.png)

# Setup

Clone this repository

in both Arduino sketches create secrets.h with content

```
//check that ssid is not conflicting with any existing ssid names
//sauna temperature reader will create it's own wifi network using these
//sauna temperature display will connect using these
const char* ssid = "your_ssid";
const char* password = "your_ssid_password";
```

Make the needed connections and upload to boards.