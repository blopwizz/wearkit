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

// Wave states
#define NOT_RUNNING 0
#define GOING_UP 1
#define GOING_DOWN 2

// Servo states
#define FOLLOWING_TARGET 1
#define REACHED_TARGET 0

// Event states
#define PLAY 1
#define STOP 0

///////////////////////////////////////////////////////////////////////////////
// Pin Servo (D4 <=> 2)
int pServo = 2;

///////////////////////////////////////////////////////////////////////////////

// Time variables
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// WiFi variables
char ssid[] = "WEARKIT";              // network name
char pass[] = "twerkkkkkk";           // network password
WiFiUDP Udp;                          // UDP instance
const unsigned int localPort = 3333;  // local port to listen for UDP packets
OSCErrorCode error;
char str[256];

// Variables
Servo myservo;  // create servo object to control a servo
int vAmplitude = 15;
int vPosServo = 0;       // position of the servo
int vDefaultPosServo = 0; // target position of the servo
int vDelay = 5;          // delay in ms between each rotation
int vStep = 2;           // steps that the motor makes at every loop

// States
int sServo = 0; // state of servo
int sWave = 0;

// Events
int eWave = 0;

// Event memory
int epWave = 0;

////////////////////////// SETUP ///////////////////////////////////////

void setup() {
  // Defining pins
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 1);
  myservo.attach(pServo);  // attaches the servo on GIO2 to the servo object
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
  followTarget(0);

}

/////////////// OSC UPDATE FUNCTIONS //////////////////

void bangWave(OSCMessage &msg) {
  sWave = GOING_UP;
  Serial.println("bang wave");
}

void updateAmplitude(OSCMessage &msg) {
  vAmplitude = msg.getInt(0);
}
}

void updateDefaultPosServo(OSCMessage &msg) {
  vDefaultPosServo = msg.getInt(0);
  followTarget(vDefaultPosServo);
}

void updateDelay(OSCMessage &msg) {
  if (msg.isInt(0)) {
    vDelay = msg.getInt(0);
  }
}

void updateStep(OSCMessage &msg) {
  if (msg.isInt(0)) {
    int receivedInt = msg.getInt(0);
    vStep = receivedInt;
  }
  else {}
}


// **************** UTILITIES FUNCTIONS *************


void saveEventState() {
  epWave = eWave;
}


///////////////// MOTION FUNCTIONS /////////////////////

void updateWaveState() {
  if (sWave == GOING_UP) {
    sServo = followTarget(vAmplitude);
    if (sServo == REACHED_TARGET) {
      sWave = GOING_DOWN;
    }
  }

  if (sWave == GOING_DOWN) {
    sServo = followTarget(0);
    if (sServo == REACHED_TARGET) {
      sWave = NOT_RUNNING;
    }
  }
}

int followTarget(int target) {
  if (abs(vPosServo - target) > vStep) {
    if (vPosServo > target) {
      vPosServo -= vStep;
    }
    else {
      vPosServo += vStep;
    }
    myservo.write(vPosServo);
    delay(vDelay);
    return FOLLOWING_TARGET;
  }
  else {
    return REACHED_TARGET;
  }
}



//////////////////// LOOP ////////////////////////////////

void loop() {
  currentMillis = millis();
  OSCMessage receivedMessage;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      receivedMessage.fill(Udp.read());
    }
    if (!receivedMessage.hasError()) {
      receivedMessage.dispatch("/vAmplitude", updateAmplitude);
      receivedMessage.dispatch("/vDefaultPosServo", updatePosServo);
      receivedMessage.dispatch("/vDelay", updateDelay);
      receivedMessage.dispatch("/vStep", updateStep);
      receivedMessage.dispatch("/eWave", bangWave);
    } else {
      error = receivedMessage.getError();
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("type: ");
      Serial.println(receivedMessage.getType(0));
    }
  }

  updateWaveState();
  saveEventState();
}

