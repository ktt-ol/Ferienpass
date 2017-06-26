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

// Values for microphone
const int THRESHOLD = 20;
const int VAL_MEAN = 507;
// trigger characteristics
const int CLAPS_TO_TRIGGER = 2; // number of claps
const int DEBOUNCE_TIME = 150; // ms to ignore double-clapping
const int SILENCE_T = 40; // ms between claps, otherwise reset
const int MELODY_INTERVAL = 800; // ms max time for the claps
const int PAUSE_TIME = 2000; // wait after switch, or noise
// pin-layout
const int analogPin = 0;
//const int OUTPUT_PIN = 13;    
int output_state = LOW;
uint32_t pause_t = 0;
uint32_t debounce_t = 0;
uint32_t start_time = 0; // timestamp of first clap-start
uint8_t claps = 0;
int sensorVal = 0;
int amp = 0; 
// End values for microphone

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

void bunteLEDsLong() {
  for(int i=0;i<25;i++) {
    pixels.setPixelColor(i, pixels.Color(random(10, 255),random(10, 255),random(10, 255)));        
    pixels.setBrightness(10); // Helligkeit
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(100);
   } 
   delay(500);
   for(int i=0;i<25;i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));        
    pixels.setBrightness(10); // Helligkeit
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(100);
  } 
}

void bunteLEDs() {
  for(int i=0;i<25;i++) {
    pixels.setPixelColor(i, pixels.Color(random(10, 255),random(10, 255),random(10, 255)));        
    pixels.setBrightness(10); // Helligkeit
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(100);
  }
}

void ampLEDs(int amp)  {
  amp = amp / 20;
  if(amp > 255) {
    amp = 255;
  }
  for(int i=0;i<amp;i++) {
    pixels.setPixelColor(i, pixels.Color(0,255,0));        
    pixels.setBrightness(10); // Helligkeit
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(10);
  } 
    for(int i=amp;i<NUMPIXELS;i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));        
    pixels.setBrightness(10); // Helligkeit
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(10);
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


void micReset() {
  // time has expired
  start_time = 0;
  claps = 0;
  debounce_t = 0;
  Serial.println(F("reset"));
}

void startMic() {
  sensorVal = analogRead(analogPin);
  amp = abs(sensorVal-VAL_MEAN);
  ampLEDs(amp);

  // Serial.print(F("sensed: ")); Serial.println(sensorVal);
  // Serial.print(F("amp: ")); Serial.println(amp);

  // at the end of an interval check the number of claps
  if (millis()-start_time > MELODY_INTERVAL & claps != 0) {
    if (claps == CLAPS_TO_TRIGGER & millis()-pause_t > PAUSE_TIME) {
      output_state = !output_state;
      pause_t = millis();
      //digitalWrite(OUTPUT_PIN,output_state);
      Serial.print(F("Triggered "));
      Serial.println(output_state);
    }
    if (claps>CLAPS_TO_TRIGGER) { Serial.print(F("- too many")); }
    micReset();
  }

  // noise/clap detected
  if (amp > THRESHOLD &\
      millis()-debounce_t > DEBOUNCE_TIME &\
      millis()-pause_t > PAUSE_TIME) {
    // there has to be silence between claps
    if (millis()-debounce_t < SILENCE_T+DEBOUNCE_TIME) {
      claps = 0;
      debounce_t = millis();
      Serial.print(F("reset - silence interrupted"));
      Serial.print(F(" amp "));Serial.println(amp);
      clearRow(2);
      oled.setTextXY(2,0);
      oled.putString("amp: "+(String)amp);
    } else {
      claps++;
      debounce_t = millis();
      Serial.print(F("claps "));Serial.print(claps);
      Serial.print(F(" amp "));Serial.println(amp);
      clearRow(2);
      oled.setTextXY(2,0);
      oled.putString("amp: "+(String)amp);
      if (claps == 1) { start_time = debounce_t; }
    }
  }
  delay(1);
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
      bunteLEDsLong();
      break;
    case 1:  
      bunteLEDs();
      showWlans();
      break;
    case 2:
      startMic();
      break;
    default:
      showBME(); 
      bunteLEDs();
      break;
  }
   
  //beep(4, 128);
	
	delay(500);        
}
