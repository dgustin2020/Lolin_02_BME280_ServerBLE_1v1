/*
cmd_i_info_gest.cpp
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

#include "..\local_default.h"
/** stringa ->
$,i,R, CR LF
**/

/****/
char cmd_i(char com)//_device_info
{
  proto_from++;//skip separatore
  if (*proto_from++ == P_REQUEST_TYPE){   //load & skip sepa
    //richiesta, invio info
    proto_to = genera_device_info_short(proto_cmd, P_CONFIRM_TYPE, proto_str);
    proto_from++; //skip sepa
    return true;
  }
  return false;
}
