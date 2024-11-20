#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  Serial.println();
  Serial.println("Printing MAC Address every 10 seconds...");
}

void loop() {
  // Print the MAC address
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  // Wait for 10 seconds
  delay(10000);
}
