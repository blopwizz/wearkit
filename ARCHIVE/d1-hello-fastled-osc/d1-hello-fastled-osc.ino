#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <FastLED.h>

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

#define NUM_LEDS 36                                   // how many leds in your strip ?
#define DATA_PIN D4


// Time variables
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

int receivedValue;

CRGB leds[NUM_LEDS];
int delayValue;
int brightness = 0;                                    // how bright the LED is
int fadeAmount = 5;                                    // how many points to fade the LED by
float bpm = 80;

// WiFi variables
const char ssid[] = "WEARKIT";              // network name
const char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const IPAddress outIp(192, 168, 1, 69);     // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 3333;  // local port to listen for UDP packets
OSCErrorCode error;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(115200);

  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i].setRGB(0, 255, 255);                       // set color
  }
  digitalWrite(BUILTIN_LED, 1);
  delayValue = (int)((60000.0 / 102.0) / bpm);



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

void handleBPM(OSCMessage &msg) {
  receivedValue = msg.getInt(0);
  bpm = (float) receivedValue;
  delayValue = (int)((60000.0 / 102.0) / bpm);
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
      receivedMessage.dispatch("/vBPM", handleBPM);
    } else {
      error = receivedMessage.getError();
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("type: ");
      Serial.println(receivedMessage.getType(0));
    }
  }

  if ((currentMillis - previousMillis) > delayValue) {
    brightness = brightness + fadeAmount;               // change brightness for the next loop

    if (brightness <= 0 || brightness >= 255) {         // reverse the direction of the fading when completed
      fadeAmount = -fadeAmount;
    }

    FastLED.setBrightness(int(brightness));
    FastLED.show();
    previousMillis = currentMillis;
  }
}
