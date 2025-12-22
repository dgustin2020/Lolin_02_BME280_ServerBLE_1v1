/*
my_string.cpp
*/
//#include <my_include.h>
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>
#include "my_string.h"

/* sviluppo */
/****
* a seonda della lingua selezionata
* esce con il pointer alla giustra riga tradotta
****/
T_str* text_single(T_str* const* array)
{
char t = lingua & 0x03; //limita in caso di problemi
  while (t>0){
    array++;
    t--;
  }
  return *array;
}

/****
* copia una stringa in un altra
* da *from a *to
* ATTENZIONE: la stringa da copiare deve terminare con MIO_NULL
* altrimenti parte per la tengente
****/
char* my_string_copy_terminate(char* from, char* to)
{
  while(*from > MIO_NULL){
    *to++ = *from++;
  }
  return to;
}