#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// WiFi variables
char ssid[] = "WEARKIT";              // network name
char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const unsigned int localPort = 3333;  // local port to listen for UDP packets

void setup_wifi() {
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
