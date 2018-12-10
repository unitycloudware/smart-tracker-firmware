# Welcome to the Smart Tracker Firmware Tutorial!

## Quick links

* [UCW Platform][1]
* [UCW Platform Documentation][2]
* [GitHub][3]
* [Issues][4]

## Description

The Smart Tracker Firmware is an example how to implement a monitoring of moving objects and how to process data in UCW.

### Quick start

Before you start to use the firmware you need to do following steps in order to set-up connection to the UCW server:


#### Step 1

In addition to the UCW arduino library, ensure that [Adafruit_GPS library](https://github.com/adafruit/Adafruit_GPS) is installed by taking the following steps

* download the library using the link provided
* rename the uncompressed folder downloaded by removing "-master" from the name
* place this renamed folder in your \Arduino\libraries folder


#### Step 2

In config.h file, change property <b>host</b> for your UCW host. For example when you are running the plugin smart-tracker for UCW Portal 
on your machine, than use the IP address of your machine. In case you are running a firewall, please make sure port 
9602 is allowed on firewall.

```
static UCWConfig cfg = {
  .host = 192.168.0.104,
  .port = 9602,
  .isSecuredConnection = false,
  .useMqtt = false,
  .mqttUser = UCW_MQTT_USER,
  .mqttPassword = UCW_MQTT_PASS,
  .token = "your_token"
};
```

#### Step 3

You need a device token for communication with the UCW portal over REST API. The token can be found in section 
Access Tokens of the UCW project. The scope of the device token has to be READ_WRITE.

```
static UCWConfig cfg = {
  .host = 192.168.0.104,
  .port = 9602,
  .isSecuredConnection = false,
  .useMqtt = false,
  .mqttUser = UCW_MQTT_USER,
  .mqttPassword = UCW_MQTT_PASS,
  .token = "jnfb7600j0tqppi6k4179dm0iaop5u93"
};
```

#### Step 4

Provide ssid and password of the desired WiFi network to connect 
```
#define WIFI_SSID       "your_ssid"
#define WIFI_PASS       "your_password"
``` 

#### Step 5

In arduino file, you need a device ID that identifies your device for particular UCW project and it allows to store data in 
the data stream 'ucw-smarttracker'.

Example: #define DEVICE_ID   "3c483804-74ad-4e1d-b367-0afb6eacd503"

#### Step 6

Control the time interval for GPS sensor to read data. 
```
int interval = 10000; //read after every 10 seconds  
```

## USING UCW ARDUINO LIBRARY FOR WIFI

The steps on how to send data using the UCW library over WiFi can be read [here](https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library/blob/develop/README.md)


[1]: https://unitycloudware.com
[2]: https://docs.unitycloudware.com
[3]: https://github.com/unitycloudware
[4]: https://jira.unitycloudware.com/browse/UCW