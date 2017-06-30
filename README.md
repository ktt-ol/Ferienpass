# Ferienpass

## Submodules
We use the [kicad-ESP8266][kicadESP8266Lib] Libary form [jdunmire][ESP8266LibAuthor].

To clone the whole Repository including the submodules simpy use the `--recusive` Option or run the Commands `git submodule init && git submodule update` inside this repository adter cloning.

[kicadESP8266Lib]: https://github.com/jdunmire/kicad-ESP8266
[ESP8266LibAuthor]: https://github.com/jdunmire

## Arduino Library
Install the following librarys via the Arduino IDE:
- Adafruit BME280 Library
- Adafruit GFX LIbrary
- Adafruit NeoPixel
- Adafruit Unified Sensor
- ACROBOTIC SSD1306

Modify three lib-files (first two for I2C):
- Adafruit_BME280.h: Line 32: Change `BME280_ADDRESS` from `0x77` to `0x76`
- Adafruit_BME280.cpp: Line 50: Change `Wire.begin();` to `Wire.begin(5, 4); // Pin D1, D2 of ESP`
- Adafruit_SSD1306.h: Line 73/74: use line 73 `#define SSD1306_128_64`  instead of line 74 `#define SSD1306_128_32`
