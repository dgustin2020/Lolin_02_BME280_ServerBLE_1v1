/*
cmd_s_sensor_gest.cpp

usata per leggere o programmare i parametri del sensore BMP280
invio stringa:
$,s,R,0 -> xx,r / w, CR LF	
0 -> xx : è il parametro operativo fa leggere o impostare, da 0 a xx (vedi tabella)
r / w   : indica se parametro da leggere o da scrivere

Risposta 
Tutto ok => "$,s,C,0,x,1"  dove (x --< 'r' o 'w'):
Errore   => "$,s,C,0,x,0"  dove (x --< 'r' o 'w'):

Scrivi prametro
"$,p,R,xx,w,val,CR LF"	dove 'xx' è il parametro operativo e 'val' è il valore da programmare
in risposta abbiamo la stringa $,p,C,xx,W, Cr LF
Leggi parametro
"$,p,R,xx,r,CR LF"	dove 'xx' è il parametro operativo da leggere
in risposta abbiamo $,p,R,xx,r,val
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
#include "..\BME280_Task\bme280_task.h"

#define SUB_CMD_BME280_TASK_ENABLE_DISABLE    0                                      //0
#define SUB_CMD_BME280_TASK_AUTOEMIT_DELAY    SUB_CMD_BME280_TASK_ENABLE_DISABLE + 1 //1
#define SUB_CMD_BME280_TASK_READ_VALUE        SUB_CMD_BME280_TASK_AUTOEMIT_DELAY + 1 //2
#define SUB_CMD_BME280_TASK_MAX_VAL           SUB_CMD_BME280_TASK_READ_VALUE + 1     //3

/* variabili */
static char read_write_cmd = 0;

/* procedure */
static char* set_common_start_answear(long* p_parametro);

/**/
char cmd_s(char com) //_sens_task
{
  long  parametro;
  int   n_char;
  char* p_buff;
  long  long_buff;
  char* p_str = 0;
  proto_from++;//skip separatore
  if ( (*proto_from != P_REQUEST_TYPE) && (*proto_from != P_REQUEST_TYPE_min) )   //test se comando richiesta
    return PROTO_ERR_TYPE_REQ; //no richiesta, esco con errore  
  proto_from++;//skip request type
  proto_from++; //skip separatore
  //trovata richiesta, leggi parametro
  n_char = buff_char_to_long(proto_from, (long*)&parametro);
  proto_from = proto_from + n_char;//goto dopo char cifre
  if (!n_char){
    return PROTO_ERR_SUB_CMD;//nessun parametro trovato, segnalo errore
  }
  if (parametro >= SUB_CMD_BME280_TASK_MAX_VAL){//test se parametro nella list
    return PROTO_ERR_SUB_CMD;//errore fuori range, segnalo errore
  }
  proto_from++; //skip separatore
  
  //leggi tipo comando: write / read
  read_write_cmd = *proto_from++;//save read or write
  if ( (read_write_cmd != P_WRITE_COMMAND) && (read_write_cmd != P_READ_COMMAND) ){//test se lettura o scrittura
    return PROTO_ERR_SUB_SUB_CMD; //non read/write command, esco con errore
  }
  proto_from++; //skip separatore

  //preparo per risposta, che è comune per entrambi i tipo di comando
  set_common_start_answear(&parametro);//metto parte iniziale risposta

  /* 
  ho il parametro e il tipo di comando
  se tipo read, leggo da buffer
  se tipo write, lo metto in buffer 
  */
  if (read_write_cmd == P_READ_COMMAND)
  {  //tipo read
    #ifdef DEBUG_BME280
    Serial.printf("parametro= %d\r\n", parametro);
    #endif
    // a seconda del parmetro ricevuto, invio risposta
    switch (parametro)
    {
      /**/
      case SUB_CMD_BME280_TASK_ENABLE_DISABLE://richiesta stato enable/disable task
        *proto_to++ = (bme280_task_enable_disable_get() + '0');//metti valore attuale
        break;
      /**/
      case SUB_CMD_BME280_TASK_AUTOEMIT_DELAY:
        p_buff = (char*)malloc(20 * sizeof(char));
        sprintf(p_buff,"%d", bme280_task_autoemit_delay_get());
        proto_to = my_string_copy_terminate(p_buff, proto_to);
        free(p_buff);      
        break;
      /**/
      case SUB_CMD_BME280_TASK_READ_VALUE:
        p_buff = (char*)malloc(200 * sizeof(char));
        sprintf(p_buff,"T=%.1f H=%.1f P=%.1f A=%.1f",
          bme280_task_read_temp(),
          bme280_task_read_humy(),
          bme280_task_read_press(),
          bme280_task_read_alti()
        );
        proto_to = my_string_copy_terminate(p_buff, proto_to);
        free(p_buff);      
        break;
      /**/
      default://alri parametri
      p_str = (char*)malloc(100 * sizeof(char));
        sprintf(p_str,"%s",(char*)"ERR no valid parameter");
        break;//end default
    }
  }
  else 
  {//tipo write
    switch (parametro)
    {
      /**/
      case SUB_CMD_BME280_TASK_ENABLE_DISABLE:
        if ( (*proto_from == '0') || (*proto_from == '1') )//test se valore in range
        {//parametro nel range, memorizza
          bme280_task_enable_disable_set(*proto_from - '0');//valore in range set task enable
       //   *proto_to++ = P_SEPARA;
          *proto_to++ = COMMAND_OK;
          *proto_to++ = P_SEPARA;
          *proto_to++ = *proto_from; 
          *proto_to++ = P_SEPARA;
        }
        else 
        {//valore non in range
          p_str = (char*)malloc(100 * sizeof(char));
          sprintf(p_str,"%s",(char*)"ERR value out of range");
        }
        break;
      /**/
      case SUB_CMD_BME280_TASK_AUTOEMIT_DELAY:
          n_char = buff_char_to_long(proto_from,(long*)&long_buff);
          if (n_char)//test se ho trovato valore
          {//trovato valore, test se in range
            if ( (long_buff >= AUTOEMIT_DELAY_MIN) && (long_buff <= AUTOEMIT_DELAY_MAX) )
            {//in range, salvo valore
              bme280_task_autoemit_delay_set((int)long_buff);//valore in range, salvo in buffer
              *proto_to++ = COMMAND_OK;
              *proto_to++ = P_SEPARA;
              p_buff = (char*)malloc(20 * sizeof(char));
              sprintf(p_buff,"%d", bme280_task_autoemit_delay_get());
              proto_to = my_string_copy_terminate(p_buff, proto_to);
              free(p_buff);    
              *proto_to++ = P_SEPARA;
            }
            else
            {//fuori range, segnalo
              p_str = (char*)malloc(100 * sizeof(char));
              sprintf(p_str,"%s",(char*)"ERR value out of range");
            }
          }
          else
          {//non trovato valore, segnalo errore
            p_str = (char*)malloc(100 * sizeof(char));
            sprintf(p_str,"%s",(char*)"ERR no valid parameter");
          }
        break;
      /**/
      case SUB_CMD_BME280_TASK_READ_VALUE:
        *proto_to++ = COMMAND_OK;
        *proto_to++ = P_SEPARA;
        p_buff = (char*)malloc(200 * sizeof(char));
        sprintf(p_buff,"T=%.1f H=%.1f P=%.1f A=%.1f",
          bme280_task_read_temp(),
          bme280_task_read_humy(),
          bme280_task_read_press(),
          bme280_task_read_alti()
        );
        proto_to = my_string_copy_terminate(p_buff, proto_to);
        free(p_buff);       
        *proto_to++ = P_SEPARA;
        break;
      /**/
      default://parametron non disponibile, segnalare
        p_str = (char*)malloc(100 * sizeof(char));
        sprintf(p_str,"%s",(char*)"ERR no valid parameter");
        break;//end default
    }//end switch parametro

    /* metti comando riuscito o no*/
    if (p_str)
    {
      *proto_to++ = COMMAND_ERR;
      *proto_to++ = P_SEPARA;
      proto_to = my_string_copy_terminate(p_str, proto_to);
      free(p_str);
    *proto_to++ = P_SEPARA;
    }
  }//end tipo write
  return PROTO_NO_ERR;
}

/**/
static char* set_common_start_answear(long* p_parametro)
{
  proto_to = proto_metti_cmd(proto_cmd, proto_str);       /* metti inizio stringa */
  *proto_to++ = P_CONFIRM_TYPE;                   /* metti type */
  *proto_to++ = P_SEPARA;
  proto_to = proto_to + sprintf(proto_to, "%i",*p_parametro);;
  *proto_to++ = P_SEPARA;
  *proto_to++ = read_write_cmd;
  *proto_to++ = P_SEPARA;
  return proto_to;  
}
