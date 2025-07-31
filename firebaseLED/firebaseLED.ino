#include <WiFi.h>
#include <Firebase_ESP_Client.h>

/* 1. Wi-Fi credentials */
#define WIFI_SSID "esp32"
#define WIFI_PASSWORD "esp32test123"

/* 2. Firebase project credentials */
#define API_KEY "Sf0uDBl3Wn0ZuOKYvDQN9gL47eN4fiW0dVnu4Cka"
#define DATABASE_URL "https://esp32-2bd75-default-rtdb.firebaseio.com/"
#define USER_EMAIL "salim9shrestha@gmail.com"
#define USER_PASSWORD "salim@5678"

/* 3. Firebase objects */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int ledPin = 4;  // D4 = GPIO4
unsigned long lastUpdate = 0;
bool firebaseReady = false;

void setup() {
  Serial.begin(115200);

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("🔌 Starting ESP32...");

  // Connect to Wi-Fi
  Serial.print("📡 Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n✅ Wi-Fi connected");
  Serial.print("🧠 IP address: ");
  Serial.println(WiFi.localIP());

  // Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  config.timeout.serverResponse = 10000; // 10 seconds

  // Begin Firebase
  Firebase.begin(&config, &auth);
  Serial.println("🔁 Initializing Firebase...");
}

void loop() {
  // Check Firebase readiness
  if (!firebaseReady && Firebase.ready()) {
    firebaseReady = true;
    Serial.println("✅ Firebase authenticated and ready!");
    Serial.println("🚀 ESP32 is connected to Firebase Realtime Database!");
  }

  // If Firebase is not ready
  if (!Firebase.ready()) {
    Serial.println("⛔ Firebase not ready or connection lost!");
    delay(1000); // avoid spamming logs
    return;
  }

  // Poll Firebase every 500ms
  if (firebaseReady && millis() - lastUpdate > 500) {
    lastUpdate = millis();

    Serial.println("🔄 Checking Firebase for /led/state...");

    if (Firebase.RTDB.getInt(&fbdo, "/led/state")) {
      int ledState = fbdo.intData();
      digitalWrite(ledPin, ledState ? HIGH : LOW);
      Serial.println("💡 LED state from Firebase: " + String(ledState ? "ON" : "OFF"));
    } else {
      Serial.print("❌ Firebase error: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
