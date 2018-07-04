// Libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

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

// sMotion : state of motion
#define FOLLOWING_TARGET 1
#define REACHED_TARGET 0

// sWave : state of wave pattern
#define NOT_RUNNING 0
#define GOING_UP 1
#define GOING_DOWN 2

// sDefault : state of changing default
#define GOING_TO_DEFAULT 1
#define NOT_RUNNING 0

// Event states
#define PLAY 1
#define STOP 0

///////////////////////////////////////////////////////////////////////////////

// Time variables
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// WiFi variables
char ssid[] = "WEARKIT";              // network name
char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const unsigned int localPort = 3333;  // local port to listen for UDP packets

////////////////////////// SETUP ///////////////////////////////////////

void setup() {
  // Defining pins
  pinMode(BUILTIN_LED, OUTPUT);
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

//////////////////// LOOP ////////////////////////////////

void loop() {
  currentMillis = millis();
}

