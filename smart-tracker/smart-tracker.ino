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

#include "UCW_GPS.h"
UCW_API_REST ucw_api =  ucw.api();

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "ucw-smarttracker"

UCW_GPS ucw_gps; //create GPS object
void setup() {  
  // Start the serial connection
  Serial.begin(115200);

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
  
  //startup GPS connection
  ucw_gps.setupGPS();
}

void loop() {
  ucw.sys();
  String data = ucw_gps.readGPS(); // read GPS info
  
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);
  delay(1000);
}
