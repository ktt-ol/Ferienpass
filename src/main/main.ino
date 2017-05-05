#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_BME280.h> // Adafruit_BME280_Library (1.0.5) [modified for I2C!], Adafruit_Unified_Sensor (1.0.2)
#include "sensors.h"
#include "display.h"
#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h>

// Display
#include <ACROBOTIC_SSD1306.h>


/*
  #define BME_SCK 14;  // Serial Clock (SCK)
  #define BME_MISO 12; // Serial Data Out (SDO)
  #define BME_MOSI 13 ;// Serial Data In (SDI)
  #define BME_CS 15;   // Chip Select
*/

#define SEALEVELPRESSURE_HPA (1013.25)
#define PIN            D3 // LED PINs
#define NUMPIXELS      8  // Number of NeoPixels/LEDs
#define DISPLAYADDR 0x78

// LEDs
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_BME280 bme; // I2C  // D1 = SDA, D2 = SCL
//Adafruit_BME280 bme(BME_CS,BME_MOSI,BME_MISO,BME_SCK); // Software SPI 
//Adafruit_BME280 bme(15, 13, 12, 14); // Software SPI 
//Adafruit_BME280 bme(D1, D2, D3, D4); // Software SPI 

int LDR_Pin = A0;
Sensors sensor(bme);

void setup() {
//	pinMode(LED_BUILTIN, OUTPUT);    
	Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library für LEDs
  // Display
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.setTextXY(0,0);              // Set cursor position, start of line 0
  // Test
  for (int i=0; i<9; i++) {
    oled.putString((String) i);
    oled.setTextXY(i,0); 
  }
	delay(20);
}

/*
void blinkLed() {
  digitalWrite(LED_BUILTIN, LOW);   // LED on
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);  // LED off
  delay(1000);
}
*/

void printValues() {
		Serial.print("Temperature = ");
		Serial.print(sensor.getTemperature());
		Serial.println(" *C");

		Serial.print("Pressure = ");

		Serial.print(sensor.getPressure());
		Serial.println(" hPa");

		//Serial.print("Approx. Altitude = ");
		//Serial.print(sensor.readAltitude(SEALEVELPRESSURE_HPA));
		//Serial.println(" m");

		Serial.print("Humidity = ");
		Serial.print(sensor.getHumidity());
		Serial.println(" %");

		Serial.println();
}

void testLeds() {
  for(int i=0;i<NUMPIXELS;i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(500); // Delay for a period of time (in milliseconds).
  }
  delay(2000);
  for(int i=0;i<NUMPIXELS;i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(500); // Delay for a period of time (in milliseconds).
  }
  delay(2000);
  
}
void loop() {
	testLeds();

	// LDR
	float ldr = sensor.getBrightness(LDR_Pin);
	Serial.print("LDR-Brightness: "+ (String)ldr +" !\n");

	// WLAN
	/*
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
	*/
	// BME
		
	printValues();    
	delay(1000);
	 
        
}
