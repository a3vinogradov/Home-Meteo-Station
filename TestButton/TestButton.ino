void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(16, INPUT);     // Initialize the OutLedPin pin as an output
}

void loop() {
  digitalWrite(LED_BUILTIN, digitalRead(16));
}