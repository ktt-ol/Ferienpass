#include "lib/Adafruit_BME280.h"
#include <Wire.h>


Adafruit_BME280 bme; // I2C

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

    Serial.begin(115200);
   delay(10);
   Serial.println("Sensor Test");
   
   if (!bme.begin())  {
    Serial.print("Ooops, no BME280 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  else {
    Serial.println("BME280 ready.");
  }
}

void blinkLed() {
  digitalWrite(LED_BUILTIN, LOW);   // LED on 
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH);  // LED off
  delay(2000);    
}
void loop() {
  blinkLed();
  //float pressure = bme.readPressure();
  Serial.print("Pressure:    ");
  //Serial.print(pressure);  
               
}
