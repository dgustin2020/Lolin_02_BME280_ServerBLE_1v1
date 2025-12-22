/*
main.cpp
*/
#include  <common_include.h>

#include <SD.h>
#include <my_Out_Pin.h>
#include <my_bme280_driver.h>

#include "..\Blue_gest\Blue_gest.h"
//#include "..\Flash_param\Flash_param.h"
#include "..\Battery_Task\battery_task.h"
#include "..\BME280_Task\bme280_task.h"
#include "..\preferenze_gest\pref_gest.h"
#include "..\BLE_task\ble_server_task.h"

extern void main_loop(void);
extern void main_init(void);
extern void form_base(void);
extern void form_version(void);
extern void form_main_select(void);

extern void form_set_var_long(long* p_var, T_str* const* title);
extern T_str* const T_TITOLO_SET_VAR_SAMPLE[];
extern void form_set_Lclk(T_Lclk* p_Lclk, T_str* const* title);

/****/
static void dummy(void);

/****/
//static FnPtr        Pfn_Mainloop;     // dich. puntatore al programma principale
//static uint8_t      Cnt_Mainloop;

static FnPtr        tbl_fn_Mainloop[] = {// dich. array puntatori alle funzioni
#ifdef BOARD_CYD_01  
  my_lv_gest_task,          //00
  forms_task_run,           //01 getione form
  backlight_task_run,       //02
#else
  dummy,                    //00
  dummy,                    //01
  dummy,                    //02
#endif  
  usb_gest_run,             //03 gestione seriale USB
  battery_task_run,         //04
  blue_task_run,            //05
  #ifdef USE_SOUND_DAC
  dummy,                    //06
  #else
  my_nota_task_run,         //06
  #endif
  bme280_task_run,          //07
  ble_server_task_run,      //08
  dummy,                    //09
  dummy,                    //0a
  dummy,                    //0b
  dummy,                    //0c
  dummy,                    //0d
  dummy,                    //0e
  dummy,                    //0f
  0
};
#define MAINLOOP_MASK  0x0f //da modificare in funzione delle procedure usate
My_Task_Gest main_gest;//crea gestione  main_loop

static T_Lclk loc_tmr = 0;
static T_Lclk loc_tmr_test = 0;
static bool toggle = false;
static bool toggle1 = false;
static Out_Pin led_verde;

//extern void gui_setup(void);

/**** pubbliche ****/

/*
chiamata una sola volta in 'setup' dell'applicazione
*/
void main_init(void)
{

/* init led che indica lo stato della board */
  //board_status_init(PIN_OUT_LED, BOARD_STAT_INIT, Base_Lclk);
/* inizializza gestione loop wifi chiamato nel main loop*/
  //wifi_loop_init();
/*init task NTP */
  //ntp_loop_init();

/*abilit dac su pin dac collegato a ampli bf*/
//dac_output_enable(dacChanel);
//dac_output_voltage(dacChanel, dac_out_value);

  /* init task batteria*/
  battery_task_init();
  
  /*init task rx rs232 via usb*/
  usb_gest_init((T_Lclk*)Base_Lclk);

  /* 
  leggi parametri da flash, 
  deve essere messa dopo init usb perchè se abilitato debug_flash
  viene inviata la lista dei parametri sulla seriale
  */
  usb_232.load_tx_my_string_CR_LF((uint8_t*)"READ FLASH PARAM");

  /************************************ */
  /*** gestione oreferenze  ******/
  /*load parametri base*/
  if (pref_gest_load_all_param_from_space((T_pref_record**)&tbl_base_param_rec, (Preferences*)&impo_base_space)) {
    //Serial.println("MAIN Error loading preferences, defaults loaded");
    usb_232.load_tx_my_string_CR_LF((uint8_t*)"Error loading preferences, defaults loaded");
  } else {
    //Serial.println("MAIN Preferences loaded successfully");
    usb_232.load_tx_my_string_CR_LF((uint8_t*)"Preferences loaded successfully");
  }
  //su usb i parametri base
  pref_gest_write_all_param_to_serial((T_pref_record**)&tbl_base_param_rec,COM_PORT_USB);

  /* load paraetri utente */
  if (pref_gest_load_all_param_from_space((T_pref_record**)&tbl_user_param_rec, (Preferences*)&impo_user_space)) {
    //Serial.println("MAIN Error loading preferences, defaults loaded");
    usb_232.load_tx_my_string_CR_LF((uint8_t*)"Error loading User Preferences, defaults loaded");
  } else {
    //Serial.println("MAIN Preferences loaded successfully");
    usb_232.load_tx_my_string_CR_LF((uint8_t*)"User Preferences loaded successfully");
  }
  //su usb i parametri base
  pref_gest_write_all_param_to_serial((T_pref_record**)&tbl_user_param_rec,COM_PORT_USB);
  /*** END gestione oreferenze  ******/ 
  /*************************************** */

  /* start itr al 1/1000 */
  itr_time_init(onTime_itr, TIMER_1000, PRESCALER_TIMER_1000, TIC_TIMER_1MSEC);
  
  /*
  init genera nota
  a seconda se uso un buzzer diretto o un ampli BF con out mediante dac genrando onda quadra a livello controllato
  oppure se uso ampli bf con out su dac mediante generazione di una sinusoide con fuzione cosine
  Se USE_SOUND_DAC impostato in platformio.ini, uso buzzer diretto o dac onda quadra
  la selszione avvien con flag BUZZER_ON_BOARD 
  */
 #ifdef USE_SOUND_DAC
    sound_nota_init();//nota su buzzer o dac onda quadra
  #else
    my_nota_task_init();//nota si dac con onda sinusoidale
  #endif
  
  /*init gestione blink led*/
  #ifdef DEBUG_OUT_PIN
  if (led_verde.init(OUT_LED_GREEN_PIN,0))
    Serial.println("init led verde OK");
  else
    Serial.println("init led verde error");
  #else
    led_verde.init(OUT_LED_GREEN_PIN,0);
  #endif

  /*init task bluetooth*/
  blue_task_init(BLUE_SET_SLAVE);

  /* init task sensor bme280 */

  #ifndef BME280_SPI 
  bme280_task_init(BME280_I2C_ADDRESS, BME280_DELAY_RATE);
  #else
  bme280_task_init();//  bme280_task_init(0x76, BME280_DELAY_RATE);
  #endif
  
  /* init server BLE x invio parametri BME280 */
  ble_server_task_init();

#ifdef BOARD_CYD_01
  /*start gestione backlight*/
  backlight_task_init();
  /*start driver touchpad e gestione lcd*/
  #if USE_LVGL_TFT_ESPI == 1
  my_lv_gest_setup();
  #else
  /* crea oggetto barra di attesa */
  barra.init(BARRA_ROW,BARRA_COL_START,BARRA_CHAR,BARRA_CNT_MAX,BARRA_WAIT_VAL,(T_Lclk*)&Base_Lclk);
  tft_lib_init_xpt2046();
  #endif
  //open versione
  forms_open(form_version);//all'uscita lancia "form_main_select"
#endif 

  /* inizializza loop del main */
  main_gest.init_and_run(0, MAINLOOP_MASK, tbl_fn_Mainloop, (T_Lclk*)&Base_Lclk);//inizializza main_loop
}

/*
* procedura chiamata ogni millisecondo da 'loop' dell'applicazione principale
*/
void main_loop(void)
{
  /*blink led verde*/
  if (Base_Lclk > loc_tmr){
    loc_tmr += 500;
    led_verde.toggle();
  }

  /* genera main_clock */
  if (Main_Base_Offset.segno)
    Main_Lclk = Base_Lclk + Main_Base_Offset.value;
  else 
    Main_Lclk = Base_Lclk - Main_Base_Offset.value;

  /*gestione ricezione seriale su USB, ogni milliSec x non perdere char */
  usb_gest_rx_task(); 
  
  /* passa alla prossima funzione e relativo run */
  main_gest.next_and_run();

}

/****/
static void dummy(void)
{ 
  return;
}

