/*
proto_wifi_gest.cpp
procedura chiamata dl gestore comandi
con comando 'w'
*/
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>
#include <my_itr_time.h>
#include <usb_gest.h>
#include <my_string.h>
#include <my_lib_clock_long.h>
#include <proto.h>
#include <proto_common.h>

#include "..\version\version.h"
#include "..\local_default.h"

#if 0
#ifdef WIFI_ENABLE
#include "WiFi/wifi_gest.h"
#include "WiFi/ntp_gest.h"
#endif
#include "proto_common.h"
#include "proto.h"
#endif

/****/
char proto_wifi_info(void)
{
#ifdef WIFI_ENABLE 
  proto_from++;//skip separatore
  if (*proto_from++ == P_REQUEST_TYPE){   //load & skip sepa
    //richiesta, invio info
    proto_to = genera_wifi_info(proto_cmd, P_CONFIRM_TYPE, proto_str);
    proto_from++; //skip sepa
    return true;
  }
  return false;
#else
  return false;
#endif
}  

/*
*/
char cmd_w(char com)//_wifi_gest
{
  return true;
}