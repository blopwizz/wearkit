#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <Servo.h>

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

char ssid[] = "WEARKIT";              // network name
char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const unsigned int localPort = 3333;  // local port to listen for UDP packets
OSCErrorCode error;

int testMessage;
char str[256];

Servo myservo;  // create servo object to control a servo
int vPosServo = 0; // position of the servo
int vDelay = 5; // delay in ms between each rotation

void setup() {
  // Attach Pins
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 1);
  myservo.attach(D4);  // attaches the servo on GIO2 to the servo object
  
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
}

void updatePosServo(OSCMessage &msg) {
  if (msg.isInt(0)) {
    int receivedInt = msg.getInt(0);
    vPosServo = receivedInt;
  }
  else{}
}

void updateDelay(OSCMessage &msg) {
  if (msg.isInt(0)) {
    int receivedInt = msg.getInt(0);
    vDelay = receivedInt;
  }
  else{}
}

void loop() {
  OSCMessage receivedMessage;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      receivedMessage.fill(Udp.read());
    }
    if (!receivedMessage.hasError()) {
      receivedMessage.dispatch("/vPosServo", updatePosServo);
      receivedMessage.dispatch("/vDelay", updateDelay); 

      Serial.print("vPosServo: ");
      Serial.print(vPosServo);
      Serial.print("vDelay: ");
      Serial.println(vDelay);

      myservo.write(vPosServo);
      delay(vDelay);
    } else {
      error = receivedMessage.getError();
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("type: ");
      Serial.println(receivedMessage.getType(0));
    }
  }
}
