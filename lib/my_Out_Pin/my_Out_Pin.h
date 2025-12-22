//---------------
//---------------

#ifndef OUT_PIN_H
#define OUT_PIN_H

#include "Arduino.h"
#if 0
extern "C" {
typedef void (*callbackFunction)(void);
typedef void (*parameterizedCallbackFunction)(void *);
}
#endif

class Out_Pin
{
  public:
      // ----- Constructor -----
    Out_Pin();
    Out_Pin(int8_t pin, int8_t state = LOW);
    //procedure
    bool init(int8_t pin, int8_t state);
    void on();
    void off();
    bool state();
    void toggle();
    void set(int8_t state);

  private:
    //varaiabile
    int8_t  _pin;
    int8_t  _state;
};

#endif