/*
led_blink_copy.h
*/
#ifndef LED_BLINK_H
#define LED_BLINK_H

#include "Arduino.h"
#include <my_define.h>
#include <my_Out_Pin.h>
#if 0
extern "C" {
typedef void (*callbackFunction)(void);
typedef void (*parameterizedCallbackFunction)(void *);
}
#endif

class Led_Blink{
  public:
    Led_Blink();
    //Led_Blink_copy(uint8_t pin_led = -1, T_Lclk* p_tbl = 0);
    void task_init(uint8_t pin_led, T_Lclk* p_tbl, T_Lclk time);
    bool task_run(T_Lclk time);
    void task_change(T_Lclk* p_tbl, T_Lclk time);
    void task_enable(void);
    void task_disable(void);
    bool task_status(void);
  private:
  Out_Pin        led_blink;//crea led_blink
  T_Lclk         tmr_blink_Lclk;//timer per gestione blink led
  T_Lclk*        p_tbl_blink;
  bool           led_status;
  T_Lclk*        p_rd_data;
  T_Lclk         compare_time;
  bool           flg_task_enable;//true == task abilitato

};
#endif