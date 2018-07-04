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
unsigned long interval = 500;

int ledState = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println(" init");
}

void loop() {
  currentMillis = millis();
  if ((currentMillis - previousMillis) > interval) {
    ledState = (ledState + 1) % 2;
    digitalWrite(BUILTIN_LED, ledState);
    previousMillis = currentMillis;
  }

  Serial.println(ledState);
  delay(10);
}
