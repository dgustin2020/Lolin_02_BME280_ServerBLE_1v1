/*
my_default.h
*/
#ifndef MASTER_DEFAULT_H
#define MASTER_DEFAULT_H

//#include <../../../../src/version\version.h>
#include "version/version.h"
#define NAME_FIRMWARE_DEF     NOME_PROGRAMMA_SHORT 
#define SERIAL_NUMBER_DEF     "1234"

#define BLUE_NAME_SLAVE_DEF  NOME_PROGRAMMA_BLUE "-S"
#define BLUE_PIN_SLAVE_DEF    "1234"
#define BLUE_NAME_MASTER_DEF  NOME_PROGRAMMA_BLUE "-M"
#define BLUE_PIN_MASTER_DEF   "1234"

#define WIFI_NAME_DEF         "FASTWEB-xva7kA"
#define WIFI_PSW_DEF          "J56fEbe36E"

#define NTP_SERVER_DEF        "pool.ntp.org"
#define NTP_GMT_OFFSET_DEF    3600   // fuso orario in sec
#define NTP_LEGAL_OFFSET_DEF  3600   // ora legale in sec

#endif
