/*
form_sound_volume_set.cpp

permette di modificare il volume dell'ampli generando un tono ritmico
con Up e DOWN si modifica
con ESCAPE si esce
con ENTER  si imposta il valore come volume standard

per regolare il volume bisogna modificare
*/
#include  <common_include.h>
#ifdef BOARD_CYD_01
#include  <my_lvgl_layout_keypad.h>

extern void form_sound_volume_set(void);

//**** titolo
static const T_str M_TITOLO_0 = {(char*)("Regola Volume")};
static const T_str M_TITOLO_1 = {(char*)("Set Volume")};
static const T_str M_TITOLO_2 = {(char*)("Set Volume")};
static const T_str M_TITOLO_3 = {(char*)("Set Volume")};
static T_str* const T_TITOLO_FORM[] = {
  (T_str* const)M_TITOLO_0,
  (T_str* const)M_TITOLO_1,
  (T_str* const)M_TITOLO_2,
  (T_str* const)M_TITOLO_3,
};
//**** titolo
static const T_str M_BODY_0 = {(char*)("Livello:")};
static const T_str M_BODY_1 = {(char*)("Level:")};
static const T_str M_BODY_2 = {(char*)("Level:")};
static const T_str M_BODY_3 = {(char*)("Level:")};
static T_str* const T_BODY[] = {
  (T_str* const)M_BODY_0,
  (T_str* const)M_BODY_1,
  (T_str* const)M_BODY_2,
  (T_str* const)M_BODY_3,
};


/***** gestione form ******/
static void f_open(void);
#define F_OPEN              0
static void f_reopen(void);
#define F_REOPEN            F_OPEN + 1
static void f_wait(void);
#define F_WAIT              F_REOPEN + 1
static void f_close(void);
#define F_CLOSE             F_WAIT + 1
/**/
static const FnPtr Afn_FormLoop[] = {// dich. array puntatori alle funzioni
  f_open,                //F_OPEN --> FORM_OPEN del metodo con "case"
  f_reopen,              //F:REOPEN
  f_wait,                //F_WAIT
  f_close,               //F_CLOSE
  0
};
static T_Lclk   loc_Lclk;
#define DELTA_OUT_SOUND   (1 * L_SEC)
static char*    p_mem;
#define POS_X_LABEL_BODY    10

/***************************************************************************************** */
/**** pubbliche ****/
/*
form loop 
*/
void form_sound_volume_set(void)
{  
  forms_goto(Afn_FormLoop); 
}


/**** private ****/
/*
clear form / metti titolo / metti tasti / prepara label per scrivere valore volume
*/
static void f_open(void)
{
  // cancella form
  lv_obj_clean(lv_scr_act());
  // metti titolo --> crea label
  my_lv_label_str_multi(T_TITOLO_FORM, LV_V_TITLE, LV_H_TITLE);

  // crea label per visualizzare valore level max sound
  my_lv_label_str_multi(T_BODY, RIGA_3, POS_X_LABEL_BODY);
  label_riga_4 = my_lv_label_str_direct((char*)" ", RIGA_3, POS_X_LABEL_BODY+150);
  
  /*metti tastiera*/
  my_lv_draw_keypad_from_tbl((T_my_keypad*)&KEYPAD_UP_DOWN_ESC_ENTER);
  
  //abilta backlight per sempre
  backlight_task_disable_set_on();

  //crea buffer dove convertire valore sound
  p_mem = (char*)malloc(sizeof(char) * 100);

  //set timer x out sound a tempo
  loc_Lclk = Base_Lclk + DELTA_OUT_SOUND;

  //cambia form_stat
  forms_stat_set(F_REOPEN);
}

/*
metti valore volume su label relativa
*/
static void f_reopen(void)
{
  sprintf(p_mem,"%d",sound_get_level_nota());
  lv_label_set_text(label_riga_4, p_mem);
  
  //cambia form_stat
  forms_stat_set(F_WAIT);
}

/*
gastione tasti
con UP e DOWN, torna in F_REOPEN per ridisegnare label relative
con ESCAPE esce senza fare nulla
con ENTER salva valore volume ed esce
*/
static void f_wait(void)
{
  //gestione nota
  if (Base_Lclk > loc_Lclk){
    loc_Lclk += DELTA_OUT_SOUND;
    sound_nota_start(NOTA_SCANDI);  
  }
  uint8_t val;
  //gestione tasti
  if (lv_key){
    switch (lv_key){
      case KEY_ESCAPE:
        Serial.println("Press ESCAPE");
        forms_stat_set(F_CLOSE);
        break;
      case KEY_ENTER:
        Serial.println("Press ENTER");
        forms_stat_set(F_CLOSE);
        break;
      case KEY_UP:
        Serial.println("Press UP");
        val = sound_get_level_nota();
        if (val < SOUND_NOTA_LEVEL_MAX){
          val++;
          soud_set_level_nota(val);
        }
        forms_stat_set(F_REOPEN);
        break;
      case KEY_DOWN:
        Serial.println("Press DOWN");
        val = sound_get_level_nota();
        if (val > SOUND_NOTA_LEVEL_MIN){
          val--;
          soud_set_level_nota(val);
        }
        forms_stat_set(F_REOPEN);
        break;
      default:
        Serial.println("Press ERROR");
        forms_stat_set(F_CLOSE);
      }
    lv_key = 0;
  }
}

/*
chiude il form
*/
static void f_close(void)
{
  free(p_mem);
  forms_close();
}


#endif //BOARD_CYD_01