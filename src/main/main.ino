#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> // Adafruit_BME280_Library (1.0.5) [modified for I2C!], Adafruit_Unified_Sensor (1.0.2)
#include "sensors.h"
#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h> // LEDs
#include <ACROBOTIC_SSD1306.h> // Display

#define SEALEVELPRESSURE_HPA (1013.25) //TODO
#define PIN            D3 // LED PINs
#define NUMPIXELS      8  // Number of NeoPixels/LEDs
#define DISPLAYADDR 0x78
#define DISPLAYROWS 8

// LEDs
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// BME
Adafruit_BME280 bme; // I2C  // D1 = SDA, D2 = SCL

int LDR_Pin = A0;
Sensors sensor(bme);

void setup() {
//	pinMode(LED_BUILTIN, OUTPUT);    
	Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library für LEDs
  // Display
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  //oled.drawBitmap(MAINFRAME, 1024); // TODO: 1024px Mainframelogo
  oled.setTextXY(0,0);              // Set cursor position, start of line 0
	delay(20);
}

void showBME() {
    oled.setTextXY(1,0);
    oled.putString("Temp:  ");
    oled.putString((String) sensor.getTemperature());
    oled.putString(" C");

    oled.setTextXY(2,0);
    oled.putString("Druck: ");
    oled.putString((String) sensor.getPressure());
    oled.putString(" hPa");

    oled.setTextXY(3,0);
    oled.putString("Hoehe:");
    //oled.putString((String) sensor.readAltitude(SEALEVELPRESSURE_HPA));
    oled.putString(" m");

    oled.setTextXY(4,0);
    oled.putString("Luftf:  ");
    oled.putString((String) sensor.getHumidity());
    oled.putString(" %");
   
       
    // Serial
		Serial.print("Temperature = ");
		Serial.print(sensor.getTemperature());
		Serial.println(" *C");

		Serial.print("Pressure = ");
		Serial.print(sensor.getPressure());
		Serial.println(" hPa");

		Serial.print("Approx. Altitude = ");
		//Serial.print(sensor.readAltitude(SEALEVELPRESSURE_HPA));
		Serial.println(" m");

		Serial.print("Humidity = ");
		Serial.print(sensor.getHumidity());
		Serial.println(" %");
   
		Serial.println();   
}

void testLeds() {
  for(int i=0;i<NUMPIXELS;i++) {
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // RGB
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(500);
  }
  delay(2000);
  for(int i=0;i<NUMPIXELS;i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
    pixels.show();
    delay(500); 
  }
  delay(2000);  
}

void clearRow(char row) {
  for(char column=0; column<16; column++) {
    oled.setTextXY(row,column); 
    oled.putString(" ");    
  }
}
 
 void clearMultipleRows(char firstRow, char lastRow) {
  for(char row=firstRow; row<lastRow; row++) {
    for(char column=0; column<16; column++) {
      oled.setTextXY(row,column); 
      oled.putString(" ");    
    }
  }
}

void showWlans() {
   // WLAN 
  int n = WiFi.scanNetworks();
  //oled.clearDisplay();
  oled.setTextXY(0,0);
  oled.putString("Found "+ (String)n +" WLANs!\n");

  // Clear WLANs from last scan
  if(n < DISPLAYROWS-1) {
    clearMultipleRows(n+1, DISPLAYROWS);
  }
  
  int row = 1;
  for (int i=0; i<n; ++i) {
    // More WLANs than display rows  TODO: test
    if(i > 0 && i % (DISPLAYROWS-1) == 0) { 
      row  = 1;
      delay(5000);
    }
    clearRow(row);
    oled.setTextXY(row,0);
    //oled.putString((String) i + ")");
    oled.putString((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
    oled.putString((String) WiFi.SSID(i));
    oled.putString("\n");    
    row++;
    delay(20);    
  }
}

void loop() {
  // LDR
  float ldr = sensor.getBrightness(LDR_Pin);
  Serial.print("LDR-Brightness: "+ (String)ldr +" !\n");
  
  int mode = 2; // TODO: Switch via button
  switch(mode) {
    case 1:
      showWlans();
    case 2:
      showBME(); 
    default:
      showBME(); 
  }
	//testLeds();    
	delay(2000);        
}
