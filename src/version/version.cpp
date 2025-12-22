/*
version.cpp
*/
#include <common_include.h>

#ifdef BOARD_CYD_01
#if 0
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>

#if USE_LVGL_TFT_ESPI == 1
#else
#include <my_tft_lib.h>//
#endif

#include <form.h>
#include <my_itr_time.h>
#ifdef USE_SOUND_DAC
  #include <sound.h>
#else
  #include <my_nota_dac_task.h>
#endif
#include  <..\src\Flash_param\Flash_param.h>
#include "version.h"
#include "lvgl.h"
#include <my_lvgl_gest.h>
#endif

extern void form_version(void);


/***** gestione form ******/
static void f_open(void);
#define F_OPEN            0
static void f_wait(void);
#define F_WAIT            F_OPEN + 1
static void f_close(void);
#define F_CLOSE           F_WAIT + 1
static const FnPtr Afn_FormLoop[] = {// dich. array puntatori alle funzioni
  f_open,                //F_OPEN --> FORM_OPEN del metodo con "case"
  f_wait,                //F_WAIT
  f_close,               //F_CLOSE
  0
};
/*
a display versione firmware con data / dopo 4 secondi si chide e torna al form precedente
 */
void form_version(void)
{ forms_goto(Afn_FormLoop); }

/*varaibili*/
static T_Lclk loc_tmr;
#define DELAY_VERSION_FORM  10*L_SEC

/* sviluppo */

/**/
static void f_open(void)
{
  //clear screen
  lv_obj_clean(lv_scr_act());

  /*crea label x riga*/
  label_riga_1 =   my_lv_label_str_direct((char*)" ", RIGA_1, 0);
//  lv_obj_set_style_text_font(label_riga_1,&my_lv_font_TextureNarrow_32,0);
  label_riga_2 =   my_lv_label_str_direct((char*)" ", RIGA_2, 0);
//  lv_obj_set_style_text_font(label_riga_2,&my_lv_font_TextureNarrow_32,0);
  label_riga_3 =   my_lv_label_str_direct((char*)" ", RIGA_3, 0);
//  lv_obj_set_style_text_font(label_riga_3,&my_lv_font_TextureNarrow_32,0);
  label_riga_4 =   my_lv_label_str_direct((char*)" ", RIGA_4, 0);
//  lv_obj_set_style_text_font(label_riga_4,&my_lv_font_TextureNarrow_32,0);

  /*metti test righe*/
  char* p_mem = (char*)malloc(sizeof(char) * 100);//crea buffer dove generare stribg da visualizzare
  /*riga 1*/
  sprintf(p_mem,"%s %s",NOME_PROGRAMMA, VERSIONE_PGM);
  lv_label_set_text(label_riga_1, p_mem );
  /*riga 2*/
  sprintf(p_mem,"Serial #: %s",flash_parametri.n_serie);
  lv_label_set_text(label_riga_2, p_mem );
  /*riga 3*/
  sprintf(p_mem,"%s %s",DATA_COMPILAZIONE,ORA_COMPILAZIONE);
  lv_label_set_text(label_riga_3, p_mem );
  /*riga 4*/
  lv_label_set_text(label_riga_4, (char*)"012345678901234567890" );

  free(p_mem);//delete buffer

  /* metti tastiera */  
  my_lv_draw_keypad_from_tbl((T_my_keypad*)&KEYPAD_ESC);

  /*set timer visualizzazione*/
  loc_tmr = Base_Lclk + DELAY_VERSION_FORM; 
  form_stat = F_WAIT;
}

/**/
static void f_wait(void)
{
  if (Base_Lclk > loc_tmr){//test se scaduto timer
    form_stat = F_CLOSE;//timer scaduto, close form
  }
  if (lv_key){  //test se premuto qualche tasto per verifica
    switch (lv_key){//gestione tasto premuto
      case KEY_ESCAPE:
        form_stat = F_CLOSE;
        break;
      default:
        Serial.printf("VERSION Error: key non gestito=%d\r", lv_key);
        break;
    }
    lv_key = 0;
  }
}

extern  void form_main_select(void);
/*
non chiudo il form version che rimane sullo sfonro
alluscita dal form, chiamo la main menu
*/
static void f_close(void)
{
  forms_open(form_main_select);
}


#endif //BOARD_CYD_01