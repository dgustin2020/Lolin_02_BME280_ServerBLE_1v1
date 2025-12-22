/*
my_variabili_base.h
*/
#ifndef MY_VARIABILI_BASE_H
#define MY_VARIABILI_BASE_H

#include <my_lib_clock_long.h>

/**** pubbliche ****/
typedef struct {
  T_Lclk value; //valore da aggiungere o togliere a Base_Lclk per aver Main_clock synceo
  bool   segno; // true == aggiungi; false == togli
}T_Main_Base_offset;
#define METTI_OFFSET    true
#define TOGLI_OFFSET    false
extern T_Lclk Main_Lclk;
extern T_Main_Base_offset Main_Base_Offset;
extern T_Lclk Main_Lclk_Offset;
extern T_Lclk Temp_Lclk;

/* varaibili ad uso Lcd +  Touch */
extern  char lingua;
#define ITA 0
#define ENG 1
#define TED 2
#define SPA 3
#define L   4+1

/**** buffer generici ****/
#if USE_LVGL_TFT_ESPI == 1
#else
extern char temp_displ_char[];
#endif
//extern uint temp_displ_int[];
//extern long temp_displ_long[];


/* 
* impostazioni funzionamento 
* da salvare in scheda SD
* se scheda non presente, caricare valori default
*/
typedef struct{
uint16_t  key;                //Key se diverso da 0xaa55, valori non impostati, caricare valorii default
T_Lclk    backlight_delay;    //delay backlght on
bool      sound_on_key;       //genera suono su azionamento tasti
bool      enable_sound;       //suoni abilitati
uint16_t  lingua;             //lingua
}T_impo_utente;
extern T_impo_utente          impostazioni_utente;
extern T_impo_utente const    impostazioni_utente_default;
/* define x impo utente */
/*Key*/
#define KEY_UTENTE_OK         0xAA55
#define KEY_UTENTE_DEFAULT    0x55AA
/*backlight_delay*/
#define BACKLIGHT_DELAY_MIN   (1 * L_SEC)   //1 sec
#define BACKLIGHT_DELAY_MAX   (30 * L_SEC)  //30 sec
#define BACKLIGHT_DELAY_ON    (0xFFFF)      //sempre acceso
#define BACKLIGHT_DELAY_DEFAULT   (3 * L_SEC)
/*sound_on_key*/
#define SOUND_ON_KEY_ON   true
#define SOUND_ON_KEY_OFF  false 
#define SOUND_ON_KEY_DEFAULT      SOUND_ON_KEY_ON
/*enable_sound*/
#define SOND_ENABLE_ON    true
#define SOND_ENABLE_OFF   false
#define SOND_ENABLE_DEFAULT       SOND_ENABLE_ON
/*lingua*/
#define ITA               0
#define ENG               1
#define TED               2
#define SPA               3
#define L                 4+1
#define LINGUA_DEFAULT            ITA


/*procedure*/
extern uint16_t variabili_calcola_checksum(uint8_t* p_from, uint16_t n_byte);

#endif //MY_VARIABILI_BASE_H
