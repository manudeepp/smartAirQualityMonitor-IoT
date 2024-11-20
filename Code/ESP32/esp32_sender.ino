#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "SparkFun_ENS160.h"
#include <Adafruit_AHTX0.h>

// ENS160 setup
SparkFun_ENS160 myENS;

// AHT2X setup
Adafruit_AHTX0 aht;

int ensStatus; 

// Variables for averaging
float totalAQI = 0;
float totalTemp = 0;
float totalTVOC = 0;
int readingCount = 0;

// ESP-NOW configuration
uint8_t receiverAddress[] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11}; // Replace with your ESP8266 MAC address

// Structure for sending data
typedef struct {
  float avgAQI;
  float avgTVOC;
  float avgTemp;
  bool trigger;
} SensorData;

SensorData dataToSend;

// Callback function for sending data
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent successfully" : "Message send failed");
}

void setup()
{
	Wire.begin();

	Serial.begin(115200);
  
  Serial.println("Initializing ENS 160...");

	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check wiring.");
		while(1);
	}

  

	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
	  Serial.println("ENS 160 is ready to transmit the data!");

	delay(100);

	// Device needs to be set to idle to apply any settings.
	// myENS.setOperatingMode(SFE_ENS160_IDLE);

	// Set to standard operation
	// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

	// There are four values here: 
	// 0 - Operating ok: Standard Operation
	// 1 - Warm-up: occurs for 3 minutes after power-on.
	// 2 - Initial Start-up: Occurs for the first hour of operation.
  //												and only once in sensor's lifetime.
	// 3 - No Valid Output
	ensStatus = myENS.getFlags();
	Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
	Serial.println(ensStatus);

  // Initialize AHT2X
  Serial.println("Initializing AHT2X...");
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring.");
    while (1);
  }
  Serial.println("AHT2X is ready to transmit the data!");

  // Initialize Wi-Fi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receiver
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0; // Default Wi-Fi channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP32 ready to send sensor data!");
	
}

void loop()
{
	if (myENS.checkDataStatus()) 
  {
    Serial.print("Reading Count: ");
    Serial.println(readingCount);
    
    ensStatus = myENS.getFlags();
    Serial.print("Gas Sensor Status Flag: ");
    Serial.println(ensStatus);

    float tvoc = myENS.getTVOC();
    int aqi = myENS.getAQI();
    totalAQI += aqi;
    totalTVOC += tvoc;

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    float temperature = temp.temperature;
    totalTemp += temperature;

    Serial.print("AQI");      
    Serial.print("\t");     
    Serial.print("TVOC");   
    Serial.print("\t");
    Serial.println("Temperature"); 

    // Output for Serial Plotter
    Serial.print(aqi);      // AQI value
    Serial.print("\t");     
    Serial.print(tvoc);   // TVOC value
    Serial.print("\t");
    Serial.println(temperature); // Temperature Value

    Serial.println("##############################################");
    readingCount++;
  }

  if (readingCount == 8) {
    float avgAQI = totalAQI / readingCount;
    dataToSend.avgAQI = avgAQI;
    float avgTVOC = totalTVOC / readingCount;
    dataToSend.avgTVOC = avgTVOC;
    float avgTemp = totalTemp / readingCount;
    dataToSend.avgTemp = avgTemp;
    bool trigger = (avgAQI >= 2.5 && avgTemp >= 28);
    dataToSend.trigger = trigger;
    
    Serial.println("-------------------------------------------");
    Serial.print("Average AQI: ");
    Serial.println(avgAQI);
    Serial.print("Average TVOC: ");
    Serial.println(avgTVOC);
    Serial.print("Average Temperature: ");
    Serial.println(avgTemp);
    Serial.print("Trigger: ");
    Serial.println(trigger);
    Serial.println("-------------------------------------------");

    // Send the temperature data
    Serial.println("Sending Data to the receiver....");

    esp_now_send(receiverAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));

    // Reset counters
    totalAQI = 0;
    totalTVOC = 0;
    totalTemp = 0;
    readingCount = 0;
  }

	delay(15000);
}
