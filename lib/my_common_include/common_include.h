/*
common_include.h
*/
//#ifndef _COMMON_INCLUDE_H_
//#define _COMMON_INCLUDE_H_

#include <Arduino.h>
#include <my_define.h>
#include <my_lib_clock_long.h>

#include <variabili_base.h>
#include <my_itr_time.h>

#include <My_Task_Gest.h>
#include <usb_gest.h>
#include <my_string.h>
#include <my_serial_driver.h>
#include <my_Out_Pin.h>
#include <My_Enable_Debug.h>
#include <proto.h>
#include <proto_common.h>
#include <my_Batt_Drv.h>

#ifdef USE_SOUND_DAC
  #include <sound.h>
#else
  #include <my_nota_dac_task.h>
#endif

#include  <../../src/local_default.h>
#include <Preferences.h>
#include  <../../src/preferenze_gest/pref_gest.h>
#include  <../../src/preferenze_gest/pref_gest_rec.h>

#ifdef BOARD_CYD_01
/* include relativi all'utilizzo del display LCD */
#include <my_backlight_gest.h>
#include <form.h>
#if USE_LVGL_TFT_ESPI == 1
#include <lvgl.h>
#include <my_lvgl_gest.h>
#else
#include <my_tft_lib.h>//
//#include <my_bar.h>
//#include <my_Button.h>
//#include <menu.h>
//#include <res_key_common.h>
#endif
#endif //BOARD_CYD_01

//#endif // _COMMON_INCLUDE_H_