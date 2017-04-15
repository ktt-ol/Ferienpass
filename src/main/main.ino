#include <Adafruit_BME280.h> // Adafruit_BME280_Library (1.0.5) [modified for I2C!], Adafruit_Unified_Sensor (1.0.2)
#include "sensors.h"
#include "display.h"
#include "ESP8266WiFi.h"

/*
  #define BME_SCK 14;  // Serial Clock (SCK)
  #define BME_MISO 12; // Serial Data Out (SDO)
  #define BME_MOSI 13 ;// Serial Data In (SDI)
  #define BME_CS 15;   // Chip Select
*/

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS,BME_MOSI,BME_MISO,BME_SCK); // Software SPI
//Adafruit_BME280 bme(15, 13, 12, 14); // Software SPI
int LDR_Pin = A0;

Sensors sensor(bme);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(20);
  Serial.println("Sensor Test");
}

void blinkLed() {
  digitalWrite(LED_BUILTIN, LOW);   // LED on
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);  // LED off
  delay(1000);
}

void loop() {
	blinkLed();

	// LDR
	float ldr = sensor.getBrightness(LDR_Pin);
	Serial.print("LDR-Brightness: "+ (String)ldr +" !\n");

	// WLAN
	int n = WiFi.scanNetworks();
	Serial.print("Found  "+ (String)n +" WLAN-Networks!\n");

	for (int i = 0; i < n; ++i) {
		if(i % 5 == 0) { // Displaysize?
			delay(5000);
		}
		Serial.print(i + 1);
		Serial.print(": ");
		Serial.print((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
		Serial.print(WiFi.SSID(i));
		Serial.print(" (");
		Serial.print(WiFi.RSSI(i));
		Serial.println(")");
		delay(10);
	}
	
	// BME
	if (!bme.begin())  {
		//Serial.print("No BME280 detected ... Check your wiring or I2C ADDR!\n");
	}
	else {
		Serial.println("BME280 ready.\n");
		Serial.print("Pressure:    ");
		Serial.print(sensor.getPreasure());  
		//Serial.println(sensor.getBrightness(LDR_Pin));
		delay(500);
	 }
               
}
