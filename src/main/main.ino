#include <Adafruit_BME280.h> // Adafruit_BME280_Library (1.0.5) [modified for I2C!], Adafruit_Unified_Sensor (1.0.2)
#include "sensors.h"
#include "display.h"
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
  if (!bme.begin())  {
    Serial.print("No BME280 detected ... Check your wiring or I2C ADDR!\n");
  }
  else {
    Serial.println("BME280 ready.\n");
    Serial.print("Pressure:    ");
    Serial.print(sensor.getPreasure());
    //Serial.println(sensor.getBrightness(LDR_Pin));
    delay(500);
  }

}
