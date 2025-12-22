/*
version.h
*/
#ifndef VERSION_H
#define VERSION_H
#include "..\data_ora_compila.h"

/**** VALORE VERSIONE ****/
#define VERSIONE_MAIN     " 1"
#define VERSIONE_SUB      "01"    // se nuova versione --> format gara e load default
#define VERSION_BETA      "01"   // se solo nuova lettere --> Cancella Gara 
/**** Nome Programma ****/
#define NOME_PROGRAMMA          "BME280E"
#define NOME_PROGRAMMA_BLUE     "BME280E"
#define NOME_PROGRAMMA_BLUE_SLAVE   NOME_PROGRAMMA_BLUE "_S"
#define NOME_PROGRAMMA_BLUE_MASTER  NOME_PROGRAMMA_BLUE "_M"
#define NOME_PROGRAMMA_BLE_SERVER   NOME_PROGRAMMA_BLUE "_SER"
#define NOME_PROGRAMMA_BLE_CLIENT   NOME_PROGRAMMA_BLUE "_CLI"

/**** Versione firmware ****/
#ifdef VERSION_BETA
  #define VERSIONE_PGM  VERSIONE_MAIN "v" VERSIONE_SUB "b" VERSION_BETA
#else
  #define VERSIONE_PGM  VERSIONE_MAIN "v" VERSIONE_SUB "   "
#endif

#define NOME_PROGRAMMA_SHORT    NOME_PROGRAMMA
#define NOME_PROGRAMMA_LONG     NOME_PROGRAMMA "," VERSIONE_PGM
/**** Data / ora compilazione ****/
#define DATA_COMPILAZIONE DATA_COMPILA
#define ORA_COMPILAZIONE  ORA_COMPILA

#define NOME_PGM_SHORT_RS232    NOME_PROGRAMMA
#define NOME_PGM_LONG_RS232     NOME_PROGRAMMA "," VERSIONE_PGM
#define COMPILATO_TIME_RS232    DATA_COMPILA "," ORA_COMPILA

#endif //end version.h