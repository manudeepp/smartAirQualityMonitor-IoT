#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure for received data
typedef struct {
  float avgAQI;
  float avgTVOC;
  float avgTemp;
  bool trigger;
} SensorData;

SensorData receivedData;

#define RELAY_PIN 5 // GPIO5 / D1


// Callback function for receiving data
void onReceive(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Print received data
  Serial.println("Data Received:");
  Serial.print("Average AQI: "); Serial.println(receivedData.avgAQI);
  Serial.print("Average TVOC: "); Serial.print(receivedData.avgTVOC); Serial.println(" ppb");
  Serial.print("Average Temperature: "); Serial.print(receivedData.avgTemp); Serial.println(" degrees C");
  Serial.print("Trigger: "); Serial.println(receivedData.trigger ? "HIGH" : "LOW");

  // Simulate relay control
  if (receivedData.trigger) {
    Serial.println("Relay ON");
    Serial.println("-------------------------------------------");
    digitalWrite(RELAY_PIN, HIGH); // Turn the relay ON
  } else {
    Serial.println("Relay OFF");
    digitalWrite(RELAY_PIN, LOW); // Turn the relay OFF
    Serial.println("-------------------------------------------");
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receiving callback
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onReceive);

  Serial.println("ESP8266 ready to receive data!");
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is OFF initially
}

void loop() {
  // Do nothing, waiting for data
}
