/*
mydrvbatt.cpp
*/

#include "my_Batt_Drv.h"

my_Batt_Drv::my_Batt_Drv()  //costruttore
{
  _pin_voltage = -1;
  _pin_charge  = -1;
  _batt_charge = false;
  _rate = 1;//nessun partitore
}

/*
 pin_voltage == pin dove collegato partitore batteria
 pin_charge  == pin dove collegato sindicatore di ricarica
 rate        == rapporto del partitore usato. Di solito 2 usando due resistenze di valore uguale
*/
void my_Batt_Drv::init(int pin_voltage, int pin_charge, int rate)
{
 // Serial.print("pin_volage ");
 // Serial.println(pin_voltage);
  _pin_voltage = pin_voltage;
  _pin_charge = pin_charge;
  _batt_charge = false;
  pinMode(_pin_voltage,ANALOG);
  pinMode(_pin_charge,INPUT_PULLUP);
  _rate = rate;
}

/* 
legge indicatore di stato del ic carica batteria
esce con 
false == non in carica
true  == in carica
*/
bool my_Batt_Drv::status_charge()//true = ON / false = OFF
{
  if (digitalRead(_pin_charge)){
    _batt_charge = false;//pin high, charge off
  }
  else {
    _batt_charge = true; //pin low, charge on
  }
  return _batt_charge;
}

/* 
esce con la tensione in milliVolt del pin collegto alla batterie
e moltiplicato per il valore impostato del rate del partitore d'ingresso
return -1, errore di lettura
*/
int my_Batt_Drv::voltage()
{
  if (_pin_voltage != -1){
    return analogReadMilliVolts(_pin_voltage) * _rate;
  }
  return -1;
}