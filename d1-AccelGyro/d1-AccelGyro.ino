#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>

//I2C and MPU6050 Libraries
#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;

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

//the Arduino's IP
IPAddress ip(128, 32, 122, 252);
//destination IP
IPAddress outIp(255, 255, 255, 255);
const unsigned int outPort = 1111;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
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


  //Initialize MPU6050
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  // Check settings
  checkSettings();
}

void checkSettings()
{
  Serial.println();

  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Clock Source:      ");
  switch (mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }

  Serial.print(" * Gyroscope:         ");
  switch (mpu.getScale())
  {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  }

  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());

  Serial.println();
}

int correction(int value) {
  if (value > 65535 / 2) {
    value -= 65535;
  }
  return value;
}

///////////////////////////////////// LOOP ///////////////////////////////////////

void loop() {
  // Raw acceleration and Gyro from MPU6050
  Vector rawAccel = mpu.readRawAccel();
  Vector rawGyro = mpu.readRawGyro();

  rawAccel.XAxis = correction(rawAccel.XAxis);
  rawAccel.YAxis = correction(rawAccel.YAxis); 
  rawAccel.ZAxis = correction(rawAccel.ZAxis);
  rawGyro.XAxis = correction(rawGyro.XAxis);
  rawGyro.YAxis = correction(rawGyro.YAxis);
  rawGyro.XAxis = correction(rawGyro.ZAxis);
/*
  Serial.print(rawGyro.XAxis);
  Serial.print(rawGyro.YAxis);
  Serial.print(rawGyro.ZAxis);

  Serial.print(rawAccel.XAxis);
  Serial.print(rawAccel.YAxis);
  Serial.println(rawAccel.ZAxis);
*/
  //Bundle containning the 6 raw values from MPU6050
  OSCBundle bundle;
  bundle.add("/vAccelX").add(rawAccel.XAxis);
  bundle.add("/vAccelY").add(rawAccel.YAxis);
  bundle.add("/vAccelZ").add(rawAccel.ZAxis);
  bundle.add("/vGyroX").add(rawGyro.XAxis);
  bundle.add("/vGyroY").add(rawGyro.YAxis);
  bundle.add("/vGyroZ").add(rawGyro.ZAxis);

  Udp.beginPacket(outIp, outPort);
  bundle.send(Udp);
  Udp.endPacket();
  bundle.empty();
  delay(100);
}
