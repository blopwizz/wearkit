// Libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <Servo.h>

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

// WiFi variables
char ssid[] = "WEARKIT";              // network name
char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const unsigned int localPort = 3333;  // local port to listen for UDP packets
OSCErrorCode error;
char str[256];

// Variables
int vPWM = 50;

////////////////////////// SETUP ///////////////////////////////////////

void setup() {
  // Defining pins
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
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

  pinMode(D5, OUTPUT);
}


///////////////// MOTION FUNCTIONS /////////////////////

void updatePWM(OSCMessage &msg) {
  vPWM = msg.getInt(0);
  Serial.println(vPWM);
}


void runPWM() {
  if (vPWM != 0) {
    Serial.println("in");
    digitalWrite(D5, HIGH);
    delayMicroseconds(vPWM * 10);
  }
  if (vPWM != 100) {
    digitalWrite(D5, LOW);
    delayMicroseconds(1000 - (vPWM * 10));
  }
}

//////////////////// LOOP ////////////////////////////////

void loop() {
  OSCMessage receivedMessage;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      receivedMessage.fill(Udp.read());
    }
    if (!receivedMessage.hasError()) {
      receivedMessage.dispatch("/vPWM", updatePWM);
    } else {
      error = receivedMessage.getError();
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("type: ");
      Serial.println(receivedMessage.getType(0));
    }
  }
  runPWM();
}
