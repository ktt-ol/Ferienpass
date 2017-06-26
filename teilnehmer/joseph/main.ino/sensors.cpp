#include "sensors.h"

Sensors::Sensors(Adafruit_BME280 bme) {
  Serial.println("Sensor Test");
  if (!bme.begin())  {
    Serial.print("No BME280 detected ... Check your wiring or I2C ADDR!\n");
    while(1);
  }
   else {
    Serial.println("BME280 ready.\n");
  }
	this->bme = bme;
	this->pressure = 0.0F;
	this->temperature = 0.0F;
	this->humidity = 0.0F;
	this->brightness = 0.0F;
}

float Sensors::getPressure() {
 	this->pressure = this->bme.readPressure() / 100.0F;
 	return this->pressure;
 }	

float Sensors::getTemperature() {
	this->temperature = bme.readTemperature();
	return this->temperature;
}

float Sensors::getHumidity() {
	this->humidity = bme.readHumidity();
	return this->humidity;
}

float Sensors::getBrightness(int LDR_Pin) {
	this->brightness = analogRead(LDR_Pin);
	return this->brightness;
}
