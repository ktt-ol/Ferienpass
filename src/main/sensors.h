#include <Adafruit_BME280.h>

class Sensors {
	private:
		Adafruit_BME280 bme;
		float pressure;
		float temperature;
		float humidity;
		float brightness;

	public:
		Sensors(Adafruit_BME280 bme);
		float getPressure();
		float getTemperature();
		float getHumidity();
		float getBrightness(int LDR_Pin);
};
