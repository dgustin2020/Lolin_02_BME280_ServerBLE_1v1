/*
my_batt_drv.h
*/
#ifndef MY_BATT_DRV_H
#define MY_BATT_DRV_H

#include <Arduino.h>

extern "C" {
//typedef void (*callbackFunction)(void);
//typedef void (*parameterizedCallbackFunction)(void *);
//}



/**************** */
class my_Batt_Drv
{
  public:
      // ----- Constructor -----
    my_Batt_Drv();
    my_Batt_Drv(int pin_voltage, int pin_charge, int  rate);
    //procedure
    void init(int pin_voltage, int pin_charge, int rate);
    bool status_charge();//true = ON / false = OFF
    int  voltage();// in milliVolt

  private:
    //varaiabile
    int     _pin_voltage;
    int     _batt_voltage;
    int     _pin_charge;
    bool    _batt_charge;
    int     _rate;

};

} //end C
#endif