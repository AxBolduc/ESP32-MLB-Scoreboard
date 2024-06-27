#ifndef TOUCH_BUTTON_H
#define TOUCH_BUTTON_H

#include <AceButton.h>
using namespace ace_button;

class TouchButtonConfig : public ButtonConfig
{
public:
  TouchButtonConfig(uint8_t pin) : ButtonConfig()
  {
    this->pin = pin;
  }

protected:
  static const long kTouchThreshold = 35;

  int readButton(uint8_t /*pin*/) override
  {
    long total = touchRead(this->pin);
    return (total > kTouchThreshold) ? HIGH : LOW;
  }

private:
  uint8_t pin;
};

#endif