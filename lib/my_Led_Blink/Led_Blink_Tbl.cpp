/*
led_blink_tbl.c
*/
#include <my_define.h>
#include <Led_Blink_Tbl.h>

/* led sempre spento*/
const T_Lclk BLINK_OFF[] = {
  0,LED_BLINK_OFF,
  LED_BLINK_END_SEQUENZA,0
};
/* led sempre acceso*/
const T_Lclk BLINK_ON[] = {
  0,LED_BLINK_ON,
  LED_BLINK_END_SEQUENZA,0
};
/* led blink 50%, rate 500mSec*/
const T_Lclk BLINK_500_500[] = {
  500,LED_BLINK_ON,             // on x 500mS
  500,LED_BLINK_OFF,            //off x 500mS
  LED_BLINK_END_SEQUENZA,0
};
/* led blink 50%, rate 500mSec*/
const T_Lclk BLINK_100_300[] = {
  100,LED_BLINK_ON,             // on x 100mS
  300,LED_BLINK_OFF,            //off x 200mS
  100,LED_BLINK_ON,             // on x 100mS
  300,LED_BLINK_OFF,            //off x 200mS
  100,LED_BLINK_ON,             // on x 100mS
  1500,LED_BLINK_OFF,           //off x 900mS
  LED_BLINK_END_SEQUENZA,0
};

