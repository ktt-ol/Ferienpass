#include "sensors.h"

Sensors::Sensors(Adafruit_BME280 bme) {
	this->bme = bme;
	this->preasure = 0.0F;
	this->temperature = 0.0F;
	this->humidity = 0.0F;
	this->brightness = 0.0F;
}

float Sensors::getPreasure() {
	this->preasure = bme.readPressure();
	return this->preasure;
 }	

float Sensors::getTemperature() {
	this->temperature = bme.readTemperature() / 100.0F;
	return this->temperature;
}

float Sensors::getHumidity() {
	this->humidity = bme.readHumidity() / 100.0F;
	return this->humidity;
}

float Sensors::getBrightness(int LDR_Pin) {
	this->brightness = analogRead(LDR_Pin);
	return this->brightness;
}
