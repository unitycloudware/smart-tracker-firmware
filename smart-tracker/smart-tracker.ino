/*
  Smart Tracker Firmware - Smart Tracker for monitoring of moving objects
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

UCW_API_REST ucw_api =  ucw.api();

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "ucw-dhtlogger"

void setup() {  
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw.connect();

  // Wait for a connection
  while (ucw.status() != UCW_NET_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println(" Connected!");
  ucw.printNetworkInfo();
  
  dht.begin();
}

void loop() {
  ucw.sys();
  
  delay(1000);
}