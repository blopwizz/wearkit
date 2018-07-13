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

void setup() {
  Serial.begin(115200);
  Serial.println("setup is done");
}

void loop() {
  Serial.println("running loop 1");
  printOther();
}
