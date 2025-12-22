/*
cmd_a_device_info_task
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
#include "..\preferenze_gest\pref_gest.h"
/*
device_info_task
*/
char cmd_a(char com) //_device_info_pgm_serial
{
  proto_from++;//skip separatore
  if  ( (*proto_from == P_REQUEST_TYPE) || (*proto_from == P_REQUEST_TYPE_min) ){   //load & skip sepa
    proto_from++;//skip request type
    proto_to = genera_device_info_task(proto_cmd, P_CONFIRM_TYPE, proto_str);    //richiesta, invio info
    proto_from++; //skip sepa
    return PROTO_NO_ERR;
  }
  return PROTO_ERR_TYPE_REQ;
}


/* 
 * genera la stringa con il tipo e il numereo seriale del crono 
*/
char* genera_device_info_short(char cmd, char type, char* p_to)
{
char*   p_from;
/* metti inizio stringa */
  p_to = proto_metti_cmd(cmd, p_to);
/* metti type */
  *p_to++ = type;
  *p_to++ = P_SEPARA;
/* metti nome strumento */
  if (base_param.dev_name[0] == 0)
    //p_from = (char*)NAME_FIRMWARE_DEF;
    p_from = (char*)NOME_PGM_LONG_RS232;
  else
    p_from = (char*)base_param.dev_name;
  while (*p_from != MIO_NULL){
    *p_to++ = *p_from++;
  }
  *p_to++ = P_SEPARA;
  //metti n_serie
  if (base_param.dev_serie[0] == 0)
    p_from = (char*)SERIAL_NUMBER_DEF;
  else
    p_from = (char*)base_param.dev_serie;
  while (*p_from != MIO_NULL){
    *p_to++ = *p_from++;
  }
  return p_to;//esci con ptr al posto giusto
}
/****
genera la stringa con il tipo e il numereo seriale del crono e la versione
****/
char* genera_device_info_task(char cmd, char type, char* p_to)
{
  p_to = genera_device_info_short(cmd, type, p_to);
  *p_to++ = P_SEPARA;
  /* metti info attivazione task */
  p_to = my_string_copy_terminate((char*)"TASK_INFO", p_to);
  /* metti flag vari task */
  *p_to++ = P_SEPARA;
  p_to = my_string_copy_terminate((char*)"TaskWiFi ", p_to);
  #ifdef WIFI_ENABLE
    if (wifi_task_get_state())
      *p_to++ = '1';
    else
      *p_to++ = '0';
    #else
      *p_to++ = 'x';
  #endif
  *p_to++ = P_SEPARA;
  p_to = my_string_copy_terminate((char*)"TaskNTP ", p_to);
  #ifdef WIFI_ENABLE
    if (ntp_task_get_state())
      *p_to++ = '1';
    else
      *p_to++ = '0';
    #else
      *p_to++ = 'x';
  #endif
  *p_to++ = P_SEPARA;
  p_to = my_string_copy_terminate((char*)"TaskBMP ", p_to);
  #ifdef BMP230_ENABLE
    if (bmp280_task_get_state())
      *p_to++ = '1';
    else
      *p_to++ = '0';
    #else
      *p_to++ = 'x';
  #endif
  *p_to++ = P_SEPARA;
  //p_to--;
  //p_to--;
  return p_to;
}

/*
genera una stringa con info wifi
*/
char* genera_wifi_info(char cmd, char type, char* p_to)
{
char*   p_from;
  /* metti inizio stringa */
  p_to = proto_metti_cmd(cmd, p_to);
  /* metti type */
  *p_to++ = type;
  *p_to++ = P_SEPARA;
  /* qui metti dati relativi al wifi */
#ifdef WIFI_ENABLE
  if (WiFi.status() == WL_CONNECTED){
    *p_to = '1';
    *p_to = P_SEPARA;
    p_to = my_string_copy_terminate((char*)"WiFi connect to ", p_to);
    p_to = my_string_copy_terminate((char*)base_param.wifi_name, p_to);
  }
else {
    *p_to = '0';
    *p_to = P_SEPARA;
    p_to = my_string_copy_terminate((char*)"WiFi not connect", p_to);
  }
#else
  *p_to = 'x';
  *p_to = P_SEPARA;
  p_to = my_string_copy_terminate((char*)"No WiFi", p_to);
#endif
//  p_to = wifi_conn_info(p_to);
  return p_to;
}

