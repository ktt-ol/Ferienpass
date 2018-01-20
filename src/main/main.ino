#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> // Adafruit_BME280_Library (1.0.5) [modified for I2C!], Adafruit_Unified_Sensor (1.0.2)
#include "sensors.h"
#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h> // LEDs
#include <ACROBOTIC_SSD1306.h> // Display

#define BUTTONPIN      D4
#define LEDPIN         D3 
#define BEEPPIN        10 // SD3
#define NUMPIXELS      25  // Number of NeoPixels/LEDs
#define DISPLAYADDR 0x78
#define DISPLAYROWS 8

// LEDs
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

// BME
Adafruit_BME280 bme; // I2C  // D1 = SDA, D2 = SCL

//int LDR_Pin = A0;
int buttonState = 0;
int mode = 0;
Sensors sensor(bme);

// Micro
const int VAL_MEAN = 507;
const int analogPin = 0;
//const int OUTPUT_PIN = 13;    
int sensorVal = 0;
int amp = 0; 
// End micro

void setup() {
	Serial.begin(9600);
  Serial.println("Starting ESP...");
  pixels.begin(); // This initializes the NeoPixel library für LEDs
  
  // Display
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen  
  //oled.drawBitmap(MAINFRAME, 1024); // TODO: 1024px Mainframelogo
  oled.setTextXY(0,0); 
       
  pinMode(BUTTONPIN, INPUT);        
  pinMode(LEDPIN, OUTPUT);  
  pinMode(BEEPPIN, OUTPUT);  

  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), isr, FALLING);
  
  beep(1, 128);
  delay(20);
}

void isr() {
  //beep(4, 128);
  Serial.println("INTERURPT!");
  oled.clearDisplay();
  mode = ++mode % 3;
}

void testLeds() {
  pixels.setPixelColor(24, pixels.Color(255,0,0)); // Rot
  for(int i=0;i<3;i++) {
    pixels.setPixelColor(i, pixels.Color(225,45,45));  // Rot
    delay(50);
  }
  pixels.setPixelColor(3, pixels.Color(75,75,255));  // Blau
  delay(1);
  pixels.setPixelColor(4, pixels.Color(225,45,45));  // Rot
  delay(1);
  pixels.setPixelColor(5, pixels.Color(225,45,45));  // Rot
  delay(1);
  pixels.setPixelColor(6, pixels.Color(75,75,255));  // Blau
  delay(1);
  pixels.setPixelColor(7, pixels.Color(225,45,45));  // Rot
  delay(1);
  for(int i=8;i<16;i++) {
    pixels.setPixelColor(i, pixels.Color(200,250,10)); // Gruen
    delay(1);
  }
  for(int i=16;i<24;i++) {
    pixels.setPixelColor(i, pixels.Color(75,75,255));  // Blau        
    pixels.setBrightness(10); // Helligkeit
    pixels.show(); // Sends the updated pixel color to the hardware.
    delay(1);
  }
  delay(100);
  for(int i=0;i<NUMPIXELS;i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
    pixels.setBrightness(10);
    pixels.show();
    delay(1); 
  }
}

void showBME() {
    oled.setTextXY(0,0);  
    oled.putString("= Umweltwerte =");
    
    oled.setTextXY(2,0);
    oled.putString("T: ");
    oled.putString((String) sensor.getTemperature());
    oled.putString(" C");

    oled.setTextXY(3,0);
    oled.putString("P: ");
    oled.putString((String) sensor.getPressure());
    oled.putString(" hPa");

    oled.setTextXY(4,0);
    oled.putString("H: ");
    oled.putString((String) sensor.getHumidity());
    oled.putString(" %");
   
       
    // Serial
    Serial.print("Temperature = ");
    Serial.print(sensor.getTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(sensor.getPressure());
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(sensor.getHumidity());
    Serial.println(" %");
     
    Serial.println();   
}

void beep(int count, int frequency) {
   for(int i=0; i<count; i++) {
    analogWrite(BEEPPIN, frequency);
    delay(1000);
    analogWrite(BEEPPIN, 0);
    delay(1000);
   }
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

  // Clear WLANs from last scan
  if(n < DISPLAYROWS-1) {
    clearMultipleRows(n+1, DISPLAYROWS);
  }
  
  int row = 1;
  for (int i=0; i<n; ++i) {
    if(mode != 1){
      oled.clearDisplay(); 
      break;
    }
    //oled.clearDisplay();
    oled.setTextXY(0,0);
    oled.putString("Found "+ (String)n +" WLANs!");

    // More WLANs than display rows
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

void startMic() {
  sensorVal = analogRead(analogPin);
  int maxAmp = 50;
  int allAmps = 0;
  for(int i=0; i<maxAmp; i++) {
    allAmps += abs(sensorVal-VAL_MEAN);   
    delay(1); 
  }
  amp = allAmps / maxAmp;

  // Serial.print(F("sensed: ")); Serial.println(sensorVal);
  // Serial.print(F("amp: ")); Serial.println(amp);
  
  clearRow(2);
  oled.setTextXY(2,0);
  oled.putString("amp: "+(String)amp);
  delay(50);
}

void loop() {
  /* LDR  for next release
  float ldr = sensor.getBrightness(LDR_Pin);
  Serial.print("LDR-Brightness: "+ (String)ldr +" !\n");
  */
  
  Serial.println("Buttonstate: " + (String)buttonState);
  Serial.println("Mode: " + (String)mode);

  switch(mode) {
    case 0:
      showBME();           
      break;
    case 1:      
      showWlans();
      break;
    case 2:
      startMic();
      break;
    default:
      showBME(); 
      break;
  }
  testLeds(); 
  //beep(4, 128);	
  delay(100);        
}
