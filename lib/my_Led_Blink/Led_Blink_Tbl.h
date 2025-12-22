/*
led_blink_tbl.h
*/
#ifndef LED_BLINK_TBL_H
#define LED_BLINK_TBL_H
#include <my_define.h>

#define LED_BLINK_ON              FALSE
#define LED_BLINK_OFF             TRUE
#define LED_BLINK_END_SEQUENZA    0

extern const T_Lclk BLINK_OFF[];
extern const T_Lclk BLINK_ON[];
extern const T_Lclk BLINK_500_500[];
extern const T_Lclk BLINK_100_300[];

#endif