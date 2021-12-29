/*********
  Split Flap ESP Master
*********/
#include "secrets.h"
#include <avr/pgmspace.h>
#include "otaWeb_html.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include "AsyncJson.h"
#include <ArduinoJson.h>
#include <ezTime.h>
#include <FastLED.h>

#define UNITSAMOUNT 10 // !IMPORTANT! Amount of connected units, change this if you have a different amount of units connected

//#define serial //uncomment for serial debug messages, no serial messages if this whole line is a comment!

#define BAUDRATE 115200
#define ANSWERSIZE 1 //Size of units request answer
#define FLAPAMOUNT 45 //Amount of Flaps in each unit
#define MINSPEED 1 //min Speed
#define MAXSPEED 12 //max Speed
#define ESPLED 1 //Blue LED on ESP01

//Variables to save values from HTML form
String alignment = "center";
String flapSpeed;
String devicemode;
String flaptext;

String alignmentSV;
String flapSpeedSV;
String devicemodeSV;
String flaptextSV;

long countdownSV;

WiFiClient espClient;
PubSubClient client(espClient);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

Timezone LocalTimezone;

void setup() {
  // Serial port for debugging purposes
  #ifdef serial
    Serial.begin(BAUDRATE);
    Serial.println("master start");
  #endif
  setupTime();    //initializes time functions
  initWiFi();     //initializes WiFi
  setupi2c();     // initialize i2c
  initFS();       //initializes filesystem
  loadFSValues(); //loads initial values from filesystem
  build_registration(); 
  otaSetup();     // initialize OTA
  mqttSetup();    // setup mqtt
  webSetup(); // setup webserver

  #ifdef serial
    Serial.println("master ready");
  #endif
}


void loop() {
  mqttLoopOps();
  ArduinoOTA.handle();
  EVERY_N_SECONDS( 60 ) {
    checkIn();
  }
  
  EVERY_N_SECONDS( 1 ) {
    updateFlapData();
  }
  checkForRestart();
}
