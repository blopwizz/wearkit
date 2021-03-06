// Libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// Wemos D1 Mini Pro pins
#define BUILTIN_LED 2
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

// Time variables
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// WiFi variables
const char ssid[] = "WEARKIT";              // network name
const char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const IPAddress outIp(192,168,1,69);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 3333;  // local port to listen for UDP packets
OSCErrorCode error;

int receivedValue;

void setup() {
  digitalWrite(BUILTIN_LED, 1);
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
  digitalWrite(BUILTIN_LED, 0);
}

void handleTest(OSCMessage &msg) {
  receivedValue = msg.getInt(0);
  Serial.println("received Value: " + receivedValue);
}


void loop() {
  currentMillis = millis();
  OSCMessage receivedMessage;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      receivedMessage.fill(Udp.read());
    }
    if (!receivedMessage.hasError()) {
      receivedMessage.dispatch("/vTest", handleTest);
    } else {
      error = receivedMessage.getError();
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("type: ");
      Serial.println(receivedMessage.getType(0));
    }
  }
}

