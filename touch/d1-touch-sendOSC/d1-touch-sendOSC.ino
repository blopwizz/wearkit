// Touch sensor base on GSR diy sensor
// Libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// ************** USER CONSTANTS ********************
#define N_SMOOTHING 20                                         // number of values for smoothing
#define DELAY 3
// WiFi parameters
char ssid[] = "WEARKIT";                             // network name
char pass[] = "twerkkkkkk";                          // network password
WiFiUDP Udp;                                         // A UDP instance to let us send and receive packets over UDP
const unsigned int localPort = 3333;                 // local port to listen for UDP packets
const IPAddress outIp(192, 168, 1, 69);           // remote IP of your computer
const unsigned int outPort = 3337;                   // remote port to receive OSC

// ************** USER VARIABLES ********************

// ************** PROCESS CONSTANTS *****************

// ************** PROCESS VARIABLES *****************

// Time variables
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long interval = 500;

int vSensor = 0;
int vvSensor[N_SMOOTHING];
int iReading = 0;
int vSum = 0;
int vResistivity = 0;
int ledState = 0;

// ************* SETUP ****************************
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  connectWifi();
  for (int k = 0; k < N_SMOOTHING; k++) {
    vvSensor[k] = 0;
  }
}

// ************ LOOP *****************************
void loop() {
  currentMillis = millis();
  if ((currentMillis - previousMillis) > interval) {
    ledState = (ledState + 1) % 2;
    digitalWrite(BUILTIN_LED, ledState);
    previousMillis = currentMillis;
  }
  readSensor();                                   // updates are launched when msg are read
  printSensor();
  sendOSC();
  delay(DELAY);                                  // delay for stability
}



