#include <Wire.h>
#include "SparkFun_ENS160.h"   // SparkFun ENS160 library
#include <Adafruit_AHTX0.h>    // Adafruit AHT2X library

// ENS160 setup
SparkFun_ENS160 myENS;

// AHT2X setup
Adafruit_AHTX0 aht;

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  // Initialize ENS160
  Serial.println("Initializing ENS160...");
  if (!myENS.begin()) {
    Serial.println("Could not communicate with the ENS160, check wiring.");
    while (1);
  }
  Serial.println("ENS160 initialized successfully.");

  // Reset ENS160 settings.
  if (myENS.setOperatingMode(SFE_ENS160_RESET))
    Serial.println("ENS160 settings reset. Ready.");
  
  delay(100);

  // Set ENS160 to standard operation
  myENS.setOperatingMode(SFE_ENS160_STANDARD);

  // Display initial status
  int ensStatus = myENS.getFlags();
  Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
  Serial.println(ensStatus);

  // Initialize AHT2X
  Serial.println("Initializing AHT2X...");
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring.");
    while (1);
  }
  Serial.println("AHT sensor found.");
}

void loop()
{
  // ENS160 Measurements
  if (myENS.checkDataStatus()) {
    int aqi = myENS.getAQI();
    float tvoc = myENS.getTVOC();
    float eco2 = myENS.getECO2();

    Serial.println("ENS160 Sensor Data:");
    Serial.print("  AQI: ");
    Serial.println(aqi);
    Serial.print("  TVOC: ");
    Serial.print(tvoc);
    Serial.println(" ppb");
    Serial.print("  eCO2: ");
    Serial.print(eco2);
    Serial.println(" ppm");
  }

  // AHT2X Measurements
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  Serial.println("AHT2X Sensor Data:");
  Serial.print("  Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");
  Serial.print("  Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  Serial.println("------------------------------------");

  // Delay between readings
  delay(5000);
}
