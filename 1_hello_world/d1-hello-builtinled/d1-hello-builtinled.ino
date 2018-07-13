unsigned long previous_millis = 0; // previous loop timestamp
unsigned long current_millis = 0;  // current loop timestamp
unsigned long interval = 500;     // in ms

int led_state = 0;

void setup() {
  setup_config();
  Serial.print(
}

void loop() {
  current_millis = millis();
  if ((current_millis - previous_millis) > interval) {
    led_state = (led_state + 1) % 2;
    digitalWrite(BUILTIN_LED, led_state);
    previous_millis = current_millis;
  }
  delay(10);
}
