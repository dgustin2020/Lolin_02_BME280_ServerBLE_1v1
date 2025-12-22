/*******************************************************************
    application.cpp
 *******************************************************************/
#include  <common_include.h>

#include "..\src\Blue_gest\Blue_gest.h"

extern void main_loop(void);
extern void main_init(void);

static T_Lclk  loc_Lclk = 0;

//#include <TinyGPS.h>       // http://arduiniana.org/libraries/TinyGPS/
//#include <SoftwareSerial.h>
// Create an instance of the HardwareSerial class for Serial 2
// HardwareSerial gpsSerial(2);
// SoftwareSerial SerialGPS = SoftwareSerial(2, 3);  // receive on pin 10
// EspSoftwareSerial::UART mySerial;


/****/
void setup() {

  /////mySerial.begin(38400, SWSERIAL_8N1, RX_PIN, TX_PIN, false);//crea seriale software
  Serial.begin(115200);
  GPIO_IS_VALID_GPIO(2);//test

  /* start main loop, con init dei vari task e gestione display*/
  main_init();//goto inizializza vari task
}

static uint16_t t;
/** */
void loop() {
  if (itr_time_scaduto()){//test se scaduto mSec x gestione procedure ad ogni mSec
    Base_Lclk++;//incre base dei tempi locale
    itr_time_scaduto_clear();//clear flag
    main_loop();/* gestione varie procedure task generici a tempo */
  }
  //test & read canale bluetooth
  blue_rx_char();
}

