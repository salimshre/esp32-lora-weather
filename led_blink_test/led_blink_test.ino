// Blink LED on GPIO2 (D2) for ESP32-WROOM

const int ledPin = 4;  // D2 pin

void setup() {
  pinMode(ledPin, OUTPUT);  // Set pin as output
}

void loop() {
  digitalWrite(ledPin, HIGH);  // Turn LED on
  delay(500);                  // Wait 500 ms
  digitalWrite(ledPin, LOW);   // Turn LED off
  delay(500);                  // Wait 500 ms
}
