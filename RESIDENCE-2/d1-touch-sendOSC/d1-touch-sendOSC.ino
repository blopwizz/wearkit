// Touch sensor base on GSR diy sensor
// Libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// ************** USER CONSTANTS ********************
#define N_SMOOTHING 5                                        // number of values for smoothing
#define DELAY 10
// WiFi parameters
char ssid[] = "WEARKIT";                             // network name
char pass[] = "twerkkkkkk";                          // network password
WiFiUDP Udp;                                         // A UDP instance to let us send and receive packets over UDP
const unsigned int localPort = 3333;                 // local port to listen for UDP packets
const IPAddress outIp(255, 255, 255, 255);           // remote IP of your computer
const unsigned int outPort = 3336;                   // remote port to receive OSC

// ************** USER VARIABLES ********************

// ************** PROCESS CONSTANTS *****************

// ************** PROCESS VARIABLES *****************

// Time variables
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

int vSensor = 0;
int vvSensor[N_SMOOTHING];
int iReading = 0;
int vSum = 0;
int vResistivity = 0;

// ************* SETUP ****************************
void setup() {
  digitalWrite(BUILTIN_LED, 1);
  connectWifi();
  digitalWrite(BUILTIN_LED, 0);
  for (int k = 0; k < N_SMOOTHING; k++) {
    vvSensor[k] = 0;
  }
}

// ************ LOOP *****************************
void loop() {
  currentMillis = millis();
  readSensor();                                   // updates are launched when msg are read
  printSensor();
  sendOSC();
  delay(DELAY);                                  // delay for stability
}



