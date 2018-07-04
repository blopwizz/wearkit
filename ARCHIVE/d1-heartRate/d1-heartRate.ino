#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>

//I2C Library
#include <Wire.h>

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

// Connection variables
char ssid[] = "WEARKIT";              // network name
char pass[] = "twerkkkkkk";           // network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP

//destination IP
IPAddress outIp(192, 168, 1, 195);
const unsigned int outPort = 9990;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

void setup() {
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("!Connecting to ");
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
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif
}
/*
  void setup() {
    Serial.begin(115200);
    Serial.println("heart rate sensor:");
    Wire.begin();
  }*/

void loop() {
  Wire.requestFrom(0xA0 >> 1, 1);    // request 1 bytes from slave device
  Serial.print(".");
  while (Wire.available()) {         // slave may send less than requested
    unsigned char c = Wire.read();   // receive heart rate value (a byte)
    Serial.println(c, DEC);         // print heart rate value
    Serial.print("Wi re: ");
    Serial.println(Wire.available());

    //Bundle containning the 6 raw values from MPU6050
    OSCBundle bundle;
    bundle.add("/vHeartRate").add(c);
 
    bundle.send(Udp);
    Udp.endPacket();
    bundle.empty();
  }
  delay(100);
}
