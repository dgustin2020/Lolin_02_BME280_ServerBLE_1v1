/*
my_define.h
definizioni primarie, da mettere in

C:\Users\diegog\.platformio\packages\toolchain-xtensa-esp32\xtensa-esp32-elf\sys-include\sys\_stdint.h

per non avere problemi con le estensioni della libreria my_TFT_eSPI (button.h e button.cpp)
credo non serva se messa in platformio.ini nella sezione libdep
*/

/********************************************************************************************/
/**** Mie modifiche Start********************************************************************/
/********************************************************************************************/
#ifndef _MY_DEFINE_H
#define _MY_DEFINE_h

/*se usato il driver estern XPT2046 come touchscreen */
//#define TOUCH_XPT2046 //messo in platformio.ini per essere piu generico
/*se usato il sistema di toucscreen di TFT_ESPI */
//#define TOUCH_TFT_ESPI

#include <Arduino.h>

/* definizione tipi mie variabili usate per compatibilita con vecchi crono **/
    typedef uint32_t    T_Lclk;
    typedef uint32_t*   pT_Lclk;
    typedef void(*FnPtr)(void);     // dich. chiamata procedura indicizzata.
    typedef char(*char_FnPtr)(void);
    typedef void(*FnPtr_char)(char val);
    typedef char(*char_FnPtr_char)(char val);
    typedef char** P_Tstr;
    typedef char*  T_str[];
    #define TRUE    true
    #define FALSE   false
    #define MIO_NULL 	0
	#define CR          0x0d
	#define LF          0x0a

/********************************************************************************************/
/**** Mie modifiche Fine*********************************************************************/
/********************************************************************************************/

/****/
#define CLOCK_CPU              APB_CLK_FREQ // 80000000 //80MHz

/**** timer ****/
//#define TIMER_0               0
//#define TIMET_1               1
#define TIMER_1000              2 //n° timer x generazione itr ogni 1 mSec
#define TIMER_SOUND             3 //n° timer x generare suono


/**** mie definizioni ****/
/* stringhe */
typedef char** P_Tstr;
typedef char*  T_str[];

/* nota tasti */
//#define nota_key    sound_nota_start
#define NOTA_ESCAPE NOTA_KEY_OK

#define COM_PORT_USB        0
#define COM_PORT_BLUE       1

/**************************************************************************
 * definizione dei piedini della scheda che dipendono dal modello
 * messi in questo file, perchè se creo un file a doc ho problemi di compilazione
 * in qualche sorgente non viene trovto il file
***************************************************************************/
/**** Define messi in platformio.ini ****/
//#define BOARD_LOLIN_02 //set tipo di scheda usata
//#define BOARD_CYD //set tipo di scheda usata
//#define BOARD_CYD32R2BT //set tipo di scheda usata

#ifdef BOARD_LOLIN_02
#include <Lolin32_pin_define.h>
#endif
#ifdef BOARD_CYD_01
#include "CYD_pin_define.h"
#endif
#ifdef BOARD_CYD32R2BT
#include "CYD32R2BT_pin_define.h"
#endif

/***************************************************************** */
/***************************************************************** */

#define KEY_DATA_FLASH_OK       0x5A5A  //  1 x uint16
#define KEY_DATA_FLASH_DEFAULT  0x5AFE  //  1 x uint16
#define FIRMWARE_LEN             32     //  4 x uint32_t
#define N_SERIE_LEN               8     //  2 x unit32_t
#define BLUE_NAME_LEN           100     // 25 x uint32_t
#define BLUE_PIN_LEN             24     //  6 x uint32_t
#define WIFI_NAME_LEN           100     // 25 x uint32_t
#define WIFI_PSW_LEN             48     // 12 x uint32_t 
#define NTP_SERVER_LEN          100     // 25 x uint32_t
#define NTP_GMT_OFFSET_LEN        4     //  1 x uint32_t   
#define NTP_LEGAL_OFFSET_LEN      4     //  1 x uint32_t
#define XTAL_SEL_DEFAULT        500     //  1 x uint16_t
#define XTAL_BASE_DEFAULT       TIC_TIMER_1MSEC  //
#define XTAL_BASE_1_DEFAULT     TIC_TIMER_1MSEC  //
#define XTAL_CHECK_SUM_DEAFULT  (uint16_t)(XTAL_BASE_1_DEFAULT + XTAL_BASE_DEFAULT + XTAL_SEL_DEFAULT)
#define CHECKSUM_DEFAULT            0xFFFF

/*********************************************************************** */
#define SENS_TASK_ENABLE_VAL_DEFAULT       1 //task abilitato
#define SENS_AUTOEMIT_DELAY_VAL_DEFAULT    0 // non autoemit

#endif
