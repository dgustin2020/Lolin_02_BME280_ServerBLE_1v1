/*
outled.cpp

gestione pin in
C:\Users\diegog\.platformio\packages\framework-arduinoespressif32\cores\esp32\esp32-hal-gpio.c
*/
#include <Arduino.h>
#include "my_Out_Pin.h"

Out_Pin::Out_Pin()
{
    _pin = -1;
    _state = LOW;
}

bool Out_Pin::init(int8_t pin, int8_t state)
{
    if (GPIO_IS_VALID_OUTPUT_GPIO(pin)){
        _pin = pin;
        _state = state;
        pinMode(_pin,OUTPUT);
        digitalWrite(_pin, _state);
        return true;
    }
    else
        return false;
}

void Out_Pin::on()
{
    if (GPIO_IS_VALID_OUTPUT_GPIO(_pin)){
        _state = HIGH;
        digitalWrite(_pin, _state);
    }
}

void Out_Pin::off()
{
    if (GPIO_IS_VALID_OUTPUT_GPIO(_pin)){
        _state = LOW;
        digitalWrite(_pin, _state);
    }
}

bool Out_Pin::state()
{
    return _state;
}

void Out_Pin::toggle()
{
    if (GPIO_IS_VALID_OUTPUT_GPIO(_pin)){
        if (_state){
        _state = LOW;
        }
        else {
            _state = HIGH;
        }
        digitalWrite(_pin, _state);
    }
}

void Out_Pin::set(int8_t state)
{
    if (GPIO_IS_VALID_OUTPUT_GPIO(_pin)){
        _state = state;
        digitalWrite(_pin, _state);
    }
}