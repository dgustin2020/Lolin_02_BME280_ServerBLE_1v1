/*
lib_clock_long.cpp
*
* il tipo clock viene gesstico com un long che contiene tutti i millisecondi che compongono 'ora.
*
* Bisogna quindi convertire il tipo prima di inviarlo alla visualizzazione

  Forse i calcoli sono pi� rapidi con un arm 32bit
*/
//#include <my_include.h>
#include  <Arduino.h>
#include  <my_define.h>
#include  "my_lib_clock_long.h"

T_Lclk const LCLK_1_SEC = L_SEC;
T_Lclk const LCLK_ON_INTERCIFRE  = L_SEC/2;
/* variabili pubbliche */
uint8_t Lclk_conv[L_CONV_LONG];
Tclkset clock_set;

/* sviluppo */
/****
* incre di un millesimo il clock passato
* Esce:
* true  = overflow day, bisogna incrementare la data
* false = tutto ok
****/
uint32_t Lclk_incre(PT_Lclk p_clk)
{
  *p_clk += 1;
  if (*p_clk > L_CLK_OVERFLOW-1){
    *p_clk = 0;
    return true;
  }
  return false;
}

/****
* somma 2 long_clock passati ed il risultato viene messo nel terzo long_clock
* Esce:
* TREU  = overflow della somma, incrementare la data
* false = tutto ok
****/
uint32_t Lclk_add(PT_Lclk p_add1, PT_Lclk p_add2, PT_Lclk p_res)
{
  *p_res = *p_add1 + *p_add2;
  if (*p_res > L_CLK_OVERFLOW-1){
    *p_res -= L_CLK_OVERFLOW;
    return true;
  }
  return false;
}

/****
* sottrae due long_clock con risultato nel terso
* Esce:
* true  = sottozero, decfemento data
* false = tutto ok
****/
uint32_t Lclk_sub(PT_Lclk p_sub1, PT_Lclk p_sub2, PT_Lclk p_res)
{
  *p_res = *p_sub1 - *p_sub2;
  if (*p_res & L_NEGATIVO){
    //*p_res += (0xFFFFFFFF-L_CLK_OVERFLOW);
    *p_res += L_DAY;
    return true;
  }
  return false;
}

/****
* converte il long_clock passato in 5 char
* giorni; ore; minuti; secondi; centesimi; millesimi
****/                                                     
uint8_t* Lclk_to_byte(PT_Lclk p_clk, uint8_t *p_dove)
{
uint8_t  x;
uint32_t temp;
  temp = *p_clk;
  /* converione day */
  x = -1;
  do{
    temp -= L_DAY;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_DAY;
  *p_dove++ = x; // save ore
  /* converione ore */
  x = -1;
  do{
    temp -= L_ORA;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_ORA;
  *p_dove++ = x; // save ore
  /* converione minuti */
  x = -1;
  do{
    temp -= L_MIN;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_MIN;
  *p_dove++ = x;  // save minuti
  /* converione secondi */
  x = -1;
  do{
    temp -= L_SEC;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_SEC;
  *p_dove++ = x;  // save secondi
  /* converione decimi/centesimi */
  x = -1;
  do{
    temp -= L_CEN;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_CEN;
  *p_dove++ = x;
  /* converione millesimi */
  *p_dove++ = (uint8_t)temp;
  return p_dove;
}

/****
* converte un long_clock in char, senza intercifre
* e senza i giorni, ma solo l'orario
****/
char* Lclk_to_char_no_inter(PT_Lclk p_clk, char *p_dove)
{
uint32_t temp;
uint8_t x;

  if (*p_clk == L_NO_TIME){
    *p_dove++ = '-'; //ore
    *p_dove++ = '-'; //ore
    *p_dove++ = '-'; //min
    *p_dove++ = '-'; //min
    *p_dove++ = '-'; //sec
    *p_dove++ = '-'; //sec
    *p_dove++ = '-'; //dec
    *p_dove++ = '-'; //cen
    *p_dove++ = '-'; //mil
    return p_dove;
  }

  temp = *p_clk;
  /* converione day */
  do{
    temp -= L_DAY;
    //x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_DAY;
  //p_dove = ByteToChar2(x, p_dove);
  /* converione ore */
  x = -1;
  do{
    temp -= L_ORA;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_ORA;
  sprintf(p_dove, "%2d", x);
  p_dove++;
  p_dove++;
  /* converione minuti */
  x = -1;
  do{
    temp -= L_MIN;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_MIN;
  sprintf(p_dove, "%02d", x);
  p_dove++;
  p_dove++;
  /* converione secondi */
  x = -1;
  do{
    temp -= L_SEC;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_SEC;
  sprintf(p_dove, "%02d", x);
  p_dove++;
  p_dove++;
  /* converione decimi/centesimi */
  x = -1;
  do{
    temp -= L_CEN;
    x++;
  }while (!(temp & L_NEGATIVO));
  temp += L_CEN;
  sprintf(p_dove, "%02d", x);
  p_dove++;
  p_dove++;
  /* converione millesimi */
  *p_dove++ = temp + '0';
  return p_dove;
}

/****
* compara 2 tipo long_clock
* Esce:
*  0 = uguali
*  1 = clk_1 > clk_2
*  2 = clk_1 < clk_2
****/
uint32_t Lclk_compare(PT_Lclk p_clk_1, PT_Lclk p_clk_2)
{
uint32_t temp;
  temp = *p_clk_1 - *p_clk_2;
  if (temp == 0)
    return L_UGUALE;
  if (temp & L_NEGATIVO)
    return L_MINORE;
  else
    return L_MAGGIORE;
}

/****
* somma 1 long_clock con un valore passato , il risultato viene messo nel secondo long_clock
* Esce:
* TREU  = overflow della somma, incrementare la data
* false = tutto ok
****/
uint32_t Lclk_add_value(PT_Lclk p_add1, T_Lclk value, PT_Lclk p_res)
{
  *p_res = *p_add1 + value;
  if (*p_res > L_CLK_OVERFLOW-1){
    *p_res -= L_CLK_OVERFLOW;
    return true;
  }
  return false;
}

/****
* sottrae a un long_clock un valore passato , il risultato viene messo nel secondo long_clock
* Esce:
* TREU  = overflow della somma, incrementare la data
* false = tutto ok
****/
uint32_t Lclk_sub_value(PT_Lclk p_add1, T_Lclk value, PT_Lclk p_res)
{
  *p_res = *p_add1 - value;
  if (*p_res & L_NEGATIVO){
    *p_res += L_CLK_OVERFLOW;
    return true;
  }
  return false;
}

/****
* estrare il valore deielle ore dal T_Lclk passato
****/
uint8_t Lclk_to_ore(T_Lclk clk)
{
uint8_t  x;
  /* converione day */
  do{
    clk -= L_DAY;
  }while (!(clk & L_NEGATIVO));
  clk += L_DAY;
  /* converione ore */
  x = -1;
  do{
    clk -= L_ORA;
    x++;
  }while (!(clk & L_NEGATIVO));
  return x;
}

/****
* estrare il valore dei minuti dal T_Lclk passato
****/
uint8_t Lclk_to_min(T_Lclk clk)
{
uint8_t  x;
  /* converione day */
  do{
    clk -= L_DAY;
  }while (!(clk & L_NEGATIVO));
  clk += L_DAY;
  /* converione ore */
  do{
    clk -= L_ORA;
  }while (!(clk & L_NEGATIVO));
  clk += L_ORA;
  /* converione minuti */
  x = -1;
  do{
    clk -= L_MIN;
    x++;
  }while (!(clk & L_NEGATIVO));
  return x;
}

/****
* estrare il valore dei scondi dal T_Lclk passato
****/
uint8_t Lclk_to_sec(T_Lclk clk)
{
uint8_t  x;
  /* converione day */
  do{
    clk -= L_DAY;
  }while (!(clk & L_NEGATIVO));
  clk += L_DAY;
  /* converione ore */
  do{
    clk -= L_ORA;
  }while (!(clk & L_NEGATIVO));
  clk += L_ORA;
  /* converione minuti */
  do{
    clk -= L_MIN;
  }while (!(clk & L_NEGATIVO));
  clk += L_MIN;
  /* converione secondi */
  x = -1;
  do{
    clk -= L_SEC;
    x++;
  }while (!(clk & L_NEGATIVO));
  return x;
}

/****
azzera le frazioni di secondo dal valore Lclk passato
****/
T_Lclk Lclk_clear_frazioni(T_Lclk clk)
{
T_Lclk result;
uint8_t  x;
  /* converione day */
  x = -1;
  do{
    clk -= L_DAY;
    x++;
  }while (!(clk & L_NEGATIVO));
  clk += L_DAY;
  result = x * L_DAY;
  /* converione ore */
  result += (clk / 1000) * 1000;
  return result;
}

/****
azzera le frazioni di secondo ed i secondi dal valore Lclk passato
****/
T_Lclk Lclk_clear_secondi(T_Lclk clk)
{
T_Lclk result;
uint8_t  x;
  /* converione day */
  x = -1;
  do{
    clk -= L_DAY;
    x++;
  }while (!(clk & L_NEGATIVO));
  clk += L_DAY;
  result = x * L_DAY;
  /* converione ore */
  x = -1;
  do{
    clk -= L_ORA;
    x++;
  }while (!(clk & L_NEGATIVO));
  clk += L_ORA;
  result += (x * L_ORA);
  /* converione minuti */
  x = -1;
  do{
    clk -= L_MIN;
    x++;
  }while (!(clk & L_NEGATIVO));
  clk += L_MIN;
  result += (x * L_MIN);
  return result;
}

/****
converte il *T_Lclk passato in byte che contengono Ore-Minutu-Secondi-Decimi-C><entesimi-Millesimi
nell'array passato
serve per l'impostazione 
****/
void Lclk_Lclk_to_clkset(T_Lclk *p_clk_in, Tclkset *dest)
{
uint8_t  x;
T_Lclk clk = *p_clk_in;
  /* converione day */
  do{
    clk -= L_DAY;
  }while (!(clk & L_NEGATIVO));
  clk += L_DAY;
  /* converione ore */
  dest->ore = -1;
  do{
    clk -= L_ORA;
    dest->ore++;
  }while (!(clk & L_NEGATIVO));
  clk += L_ORA;
  /* converione minuti */
  dest->min = -1;
  do{
    clk -= L_MIN;
    dest->min++;
  }while (!(clk & L_NEGATIVO));
  clk += L_MIN;
  /* converione secondi */
  dest->sec = -1;
  do{
    clk -= L_SEC;
    dest->sec++;
  }while (!(clk & L_NEGATIVO));
  clk += L_SEC;
  /* conversione millesimi completi */
  dest->mil = clk;//
}

/****
converte il *Tclk_set passato in T_Lclk
serve per l'impostazione 
****/
int Lclk_clkset_to_Lclk(Tclkset* clk_set, T_Lclk* clk)
{
  *clk = 0;//azzero Lclock
  *clk =   clk_set->ore * L_ORA;
  *clk += (clk_set->min * L_MIN);
  *clk += (clk_set->sec * L_SEC);
  *clk += clk_set->mil;
  return clk_set->enable;
}

/****
* estrae i secondi e i millesimidval tempo passat
* e li mette nei buffer passati
****/
void Lclk_time_to_sec_mil(T_Lclk val, uint32_t* sec, uint32_t* mil)
{
uint32_t t;
  *sec = val / 1000;
  t = *sec * 1000;
  *mil = val - t;
}
