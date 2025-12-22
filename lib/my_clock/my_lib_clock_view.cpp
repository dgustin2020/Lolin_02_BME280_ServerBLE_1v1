/*
lib_clock_view.cpp
*/
//#include <my_include.h>

#ifndef BOARD_LOLIN_02

#include <Arduino.h>
#include <my_define.h>

#if USE_LVGL_TFT_ESPI == 1
#else
#include <TFT_eSPI.h>
#include <my_tft_lib.h>//
#endif

#include "my_lib_clock_long.h"
#include "my_lib_clock_view.h"



#define POS_ORE         0 //0-1
#define POS_INT_ORE_MIN 2
#define POS_MIN         3 //3-4
#define POS_INT_MIN_SEC 5
#define POS_SEC         6 //6-7
#define POS_INT_SEC_MIL 8
#define POS_MIL         9 //9-10-11
#define POS_END_STR     12
#define POS_END_RESOL_SEC POS_MIL + 1
#define POS_END_RESOL_DEC POS_MIL + 1
#define POS_END_RESOL_CEN POS_MIL + 2
#define POS_END_RESOL_MIL POS_MIL + 3
#define LV_CHAR_SEPA_ORE_MIN ':'
#define LV_CHAR_SEPA_SEC_MIL  '.'

/*
carica in buuf un clkset con risoluzion, cancellazione zeri, blink cifre
resol = risoluzione decimale 
no_zeri 1==cancella zeri iniziali
chi_blank = indica gruppo di cifre che lapeggia
*/
void view_clock_clkset_to_buff(Tclkset *p_clkset, char* buff, char resol, bool no_zeri, char chi_blank)
{
  char * p_char;
  //crea stringa da Tclkset
  sprintf(&buff[POS_ORE],"%02d",p_clkset->ore);
  sprintf(&buff[POS_MIN],"%02d",p_clkset->min);
  sprintf(&buff[POS_SEC],"%02d",p_clkset->sec);
  sprintf(&buff[POS_MIL],"%03d",p_clkset->mil);
  buff[POS_INT_ORE_MIN] = LV_CHAR_SEPA_ORE_MIN;
  buff[POS_INT_MIN_SEC] = LV_CHAR_SEPA_ORE_MIN;
  buff[POS_INT_SEC_MIL] = LV_CHAR_SEPA_SEC_MIL;
  buff[POS_END_STR] = 0;//end of string
  //clear zeri, se abilitato
  if (no_zeri){
    //metto blank le prime cifre con valore 0
    p_char = &buff[POS_ORE];
    while (p_char <= &buff[POS_SEC + 1]){
      if ( (*p_char == '0') || (*p_char == LV_CHAR_SEPA_ORE_MIN) ){
        *p_char = ' ';
        p_char++;
      }
      else
        break;
    }
  }
  //metti blank il blocco selezionato, Serve pergenerare lampeggio
  switch (chi_blank){
    case SET_ORE:
      p_char = &buff[POS_ORE];
      *p_char++ = '_';
      *p_char   = '_';
      break;
    case SET_MIN:
      p_char = &buff[POS_MIN];
      *p_char++ = '_';
      *p_char   = '_';
      break;
    case SET_SEC:
      p_char = &buff[POS_SEC];
      *p_char++ = '_';
      *p_char   = '_';
      break;
    case SET_MIL:
      p_char = &buff[POS_MIL];
      *p_char++ = '_';
      *p_char++ = '_';
      *p_char   = '_';
      break;
    default:
      break;
  }
  //metti fine stringa in relazione alla risoluzione richiesta
  switch (resol){
    case RESOL_MIL:
      buff[POS_END_RESOL_MIL] = 0;//fine stringga
      break;
    case RESOL_CEN:    
      buff[POS_END_RESOL_CEN] = 0;
      break;
    case RESOL_DEC:    
      buff[POS_END_RESOL_DEC] = 0;
      break;
    default:
    case RESOL_SEC:    
      buff[POS_INT_SEC_MIL] = 0;
      break;
  } 
}

/****/
void view_clock_Lclk_to_buff(T_Lclk* p_Lclk, char* buff, char resol, bool no_zeri, char chi_blank)
{
  Tclkset loc_clkset;
  Lclk_Lclk_to_clkset(p_Lclk, (Tclkset*)&loc_clkset);//covert in clkset
  view_clock_clkset_to_buff((Tclkset*)&loc_clkset, buff, resol, no_zeri, chi_blank);
}


#if USE_LVGL_TFT_ESPI == 1
#else
/****/
int16_t view_clkset(Tclkset* p_clkset, int row, int col, char resol, bool no_zeri, char chi_blank){
  return view_clkset_pixel(p_clkset, RIGA_TO_PX[row], col, resol, no_zeri, chi_blank);
}

/****/
int16_t view_clkset_pixel(Tclkset* p_clkset, int row_px, int col, char resol, bool no_zeri, char chi_blank)
{
  char buff[15];
  char* p_char;

  // crea stringa hh:mm:ss.dcm con zeri 
  sprintf(&buff[POS_ORE],"%02d",p_clkset->ore);
  sprintf(&buff[POS_MIN],"%02d",p_clkset->min);
  sprintf(&buff[POS_SEC],"%02d",p_clkset->sec);
  sprintf(&buff[POS_MIL],"%03d",p_clkset->mil);
  buff[POS_INT_ORE_MIN] = CHAR_SEPA_ORE_MIN;
  buff[POS_INT_MIN_SEC] = CHAR_SEPA_ORE_MIN;
  buff[POS_INT_SEC_MIL] = CHAR_SEPA_SEC_MIL;

  if (no_zeri){
    //metto blank le prime cifre con valore 0
    p_char = &buff[POS_ORE];
    while (p_char <= &buff[POS_SEC + 1]){
      if ( (*p_char == '0') || (*p_char == CHAR_SEPA_ORE_MIN) ){
        *p_char = ' ';
        p_char++;
      }
      else
        break;
    }
  }

  switch (chi_blank){
    case SET_ORE:
      p_char = &buff[POS_ORE];
      *p_char++ = ' ';
      *p_char =' ';
      break;
    case SET_MIN:
      p_char = &buff[POS_MIN];
      *p_char++ = ' ';
      *p_char =' ';
      break;
    case SET_SEC:
      p_char = &buff[POS_SEC];
      *p_char++ = ' ';
      *p_char =' ';
      break;
    case SET_MIL:
      p_char = &buff[POS_MIL];
      *p_char++ = ' ';
      *p_char++ = ' ';
      *p_char =' ';
      break;
    default:
      break;
  }

  //metti fine stringa in relazione alla risoluzione richiesta
  switch (resol){
    case RESOL_MIL:
      buff[POS_END_RESOL_MIL] = 0;//fine stringga
      break;
    case RESOL_CEN:    
      buff[POS_END_RESOL_CEN] = 0;
      break;
    case RESOL_DEC:    
      buff[POS_END_RESOL_DEC] = 0;
      break;
    default:
    case RESOL_SEC:    
      buff[POS_INT_SEC_MIL] = 0;
      break;
  } 
  return displ_str_direct_pixel((char*)buff, row_px, col);
}

/****/
int16_t view_Lclk(T_Lclk* p_Lclk, int row, int col, char resol, bool no_zeri, char chi_blank)
{
  Tclkset loc_clkset;
  Lclk_Lclk_to_clkset(p_Lclk, &loc_clkset);//covert in clkset
  return view_clkset(&loc_clkset, row, col, resol, no_zeri, chi_blank);//view clkset
}

/****/
int16_t view_Lclk_pixel(T_Lclk* p_Lclk, int row_px, int col, char resol, bool no_zeri, char chi_blank)
{
  Tclkset loc_clkset;
  Lclk_Lclk_to_clkset(p_Lclk, &loc_clkset);//covert in clkset
  return view_clkset_pixel(&loc_clkset, row_px, col, resol, no_zeri, chi_blank);//view clkset
}
#endif

#endif  //end BOARD_LOLIN_02

