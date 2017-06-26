#include <NeoPixelBus.h>

class Display {
  private:
    int brightness;

  public:
    void setColor(int red, int green, int blue);
    void setBrightness (int brightness);
    void setValue (int value, int type);
};

