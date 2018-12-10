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

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "ucw-smarttracker"

//variables for saving longitude and latitude values
float Latitude, Longitude;

//cordinates direction
char lon_dir, lat_dir;

//timer
uint32_t timer = millis();
int interval = 10000; //read after every 10 seconds

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
  
  //startup GPS connection
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // Uncomment to ask for firmware version
  #if !defined(ARDUINO_ARCH_ESP32)
  GPSSerial.println(PMTK_Q_RELEASE);
  #endif

  // comment this if you need antenna update
  //GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update

  delay(1000);
}

void loop() {
  ucw.sys();
  
  // read GPS info
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())){
      ;
    } // this also sets the newNMEAreceived() flag to false
      //return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();

  // approximately every 10 seconds or so, print out the current stats
  if (millis() - timer > interval) {
    timer = millis(); // reset the timer
    Latitude = GPS.latitude;
    Longitude = GPS.longitude;

    if (GPS.lon == 'E'){
      lon_dir = 1;  
    } else {
      lon_dir = -1;
    }
    if (GPS.lat == 'N'){
      lat_dir = 1;  
    } else {
      lat_dir = -1;
    }
  }

  int lat_degree = (int)Latitude/100;
  float lat_min = (Latitude/100) - (float)lat_degree;
  float Lat = (lat_dir) * (lat_degree + (lat_min * 100)/60);
 
  int lon_degree = (int)Longitude/100;
  float lon_min = (Longitude/100) - (float)lon_degree;
  float Lon = (lon_dir) * (lon_degree + (lon_min * 100)/60);
  
  String data = "{\"latitude\": %lat,\"longitude\": %long}";
  data.replace("%lat", String(Lat));
  data.replace("%long", String(Lon));
  
  delay(1000);
}
