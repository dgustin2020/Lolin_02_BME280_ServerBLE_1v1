/*
proto.cpp
*/
#include <my_define.h>
#include <usb_gest.h>
#include <my_string.h>
#include "proto.h"
#include "proto_common.h"
#include <../../src/Proto_Serial/proto_serial_command.h>
#include <../../src/Blue_gest/blue_gest.h>
/*********************************************** */
/*erver per abilitare proto su bluetooth*/
//#define NO_BLUE_232 //disabilita protocollo verso driver bluetooth
/*********************************************** */

/* procedure x comandi ricevuti*/
static void  tx_stringa(char dove);

/* variabili */
char   proto_cmd;
char*  proto_from = 0;
char*  proto_to = 0;
char   proto_str[200];//buffer dove generare le stringhe in formato proto
char   proto_tx[200];//buffer dove generare le stringhe da inviare come Informazione
char   loc_char_buff[50];


static char_FnPtr_char         P_Funct;      // dich. puntatore al programma principale
static const char_FnPtr_char   A_Funct[] = { // dich. array puntatori alle funzioni
  cmd_a,                          //  'a' _device_info_task
  cmd_b,                          //  'b' _batt_info
  0,//cmd_c,                      //  'c' 
  0,                              //  'd' 
  0,                              //  'e' 
  0,                              //  'f
  0,                              //  'g
  0,                              //  'h
  0,//cmd_i,                      //  'i _device_info
  0,                              //  'j
  0,                              //  'k
  0,                              //  'l
  0,                              //  'm
  0,                              //  'n
  0,                              //  'o
  cmd_p,                          //  'p _param_gest
  0,                              //  'q
  0,                              //  'r
  cmd_s,                          //  's _sens_task
  0,                              //  't
  cmd_u,                          //  'u 
  0,                              //  'v
  0,//cmd_w,                      //  'w _wifi_gest
  0,                              //  'x
  0,                              //  'z
  0
};

static char*  p_loc_char_buff;

/*****************************************************************************************/
/*****************************************************************************************/


/*********** RICEZIONE STRINGHE ***********************/




/********************************************
procedura da chiamare dal main
analizza la stringa ricevuta e passata
a seconda dei comandi, fa cose
com_port = 0 ---> rs232
com_port = 1 ---> wireless
Se tutto OK, esce con true
****/
bool proto_gest(char* p_dati, char com_port)
{
char invia_str = 0;;
char t;
  proto_from = p_dati;
  //loc_cpm = com_port;
  proto_to = proto_str;
  if (*proto_from++ != P_PREAMBOLO){   //load & skip sepa
    proto_to = my_string_copy_terminate((char*)"Er Start: ",proto_to);//scrivi "errore"
    proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
    proto_set_end_of_string(proto_to);
    tx_stringa(com_port);
    return FALSE;                 //preambolo errato, rispondo con errore
  }
  /* load comando */
  proto_from++;//skip separatore
  proto_cmd = *proto_from++;
  /* se minuscolo converti in minuscolo*/
  if (proto_cmd < 'a')
    proto_cmd += ('a' - 'A');//converti in minuscolo
  if ((proto_cmd >= (char)P_CMD_MIN) && (proto_cmd <= (char)P_CMD_MAX)){
    P_Funct = A_Funct[(proto_cmd - (char)P_CMD_MIN)];	// load address procedura indicizzata
    if (P_Funct != 0){//test se esiste funzione
      uint8_t result_Funct = P_Funct(com_port);    
      if (result_Funct == PROTO_NO_ERR){  /* test se devo inviare una stringa */
        proto_set_end_of_string(proto_to);
        tx_stringa(com_port);
        return TRUE;
      }
      else {
        switch (result_Funct){
          case PROTO_ERR_TYPE_REQ:
            proto_to = my_string_copy_terminate((char*)"Er Type: ",proto_to);//scrivi "errore"
            break;          
          case PROTO_ERR_SUB_CMD:
            proto_to = my_string_copy_terminate((char*)"Er Sub_CMD: ",proto_to);//scrivi "errore"
            break;          
          case PROTO_ERR_SUB_SUB_CMD:
            proto_to = my_string_copy_terminate((char*)"Er Sub_Sub_CMD: ",proto_to);//scrivi "errore"
            break;          
          case PROTO_ERR_HARDWARE:
            proto_to = my_string_copy_terminate((char*)"Er Hardware: ",proto_to);//scrivi "errore"
            break;          
          default:
            proto_to = my_string_copy_terminate((char*)"Er no gest.: ",proto_to);//scrivi "errore"
            break;
        }
        proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
        proto_set_end_of_string(proto_to);
        tx_stringa(com_port);
        return FALSE;
      }
    }
    else {//funzione non esiste
      proto_to = my_string_copy_terminate((char*)"Er CMD no procedure: ",proto_to);//scrivi "errore"
      proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
      proto_set_end_of_string(proto_to);
      tx_stringa(com_port);
      return FALSE;
    }
  }
  else {
    proto_to = my_string_copy_terminate((char*)"Er CMD No in range: ",proto_to);//scrivi "errore"
    proto_to = my_string_copy_terminate(p_dati, proto_to);//metti stringa ricevuta
    proto_set_end_of_string(proto_to);
    tx_stringa(com_port);
    return FALSE;
  }
}

/***********************************
'z'
* trasmissione dati 
* invia la stringa passato con il protocollo correto
*/
void proto_tx_info(char cmd, char* txt, char com_port)
{
  char *p_to;
  char *p_str;
  p_to = proto_str; 
  //p_to = proto_tx; 
  p_str = p_to;
  *p_to++ = P_PREAMBOLO;
  *p_to++ = P_SEPARA;
  *p_to++ = cmd;
  *p_to++ = P_SEPARA;
  *p_to++ = P_INDICATION_TYPE;
  *p_to++ = P_SEPARA;
  p_to = my_string_copy_terminate(txt, p_to);
  proto_set_end_of_string(p_to);
  tx_stringa(com_port);
}

/*
* mette il comando con il char preambolo
*/
char* proto_metti_cmd(char cmd, char* p_to)
{
  *p_to++ = P_PREAMBOLO;
  *p_to++ = P_SEPARA;
  *p_to++ = cmd;
  *p_to++ = P_SEPARA;
  return p_to;
}
/*
 * mette conferma al comando ricevuto
*/
char* proto_string_cmd_cfm_ok(char cmd, char* p_to)
{
  p_to = proto_metti_cmd(cmd, p_to);
  *p_to++ = P_CONFIRM_TYPE;
  *p_to++ = P_SEPARA;
  return p_to;
}

/*
metti cr lf
*/
char* proto_set_end_of_string(char* p_to)
{
  p_to += sprintf(p_to,P_END_STRING_CR_LF);
  *p_to = MIO_NULL;
  return p_to;
}

/**************************************************************************************
***************************************************************************************/
/*
invia la stringa proto_str al driver usb/blue passato come parametro
*/
static void tx_stringa(char dove)
{
  #ifdef NO_BLUE_232
  usb_232.load_tx_my_string((uint8_t*)&proto_str);      // --> rs232
  #else //driver bluetooth abilitato
  if (!dove){
    usb_232.load_tx_my_string((uint8_t*)&proto_str);      // --> rs232
  }
    else {
    blue_232.load_tx_my_string((uint8_t*)&proto_str);     // --> blue
  }
  #endif
  proto_to = proto_str;

}


/*
legge i char del buffer passato (*p_from) e crea un long fin tanto che trova char cifre
comprei tra 0 e 9
esce con il numero di char letti
Se esce con 0, non ho trovato char cifre da convertire
il valore long viene messo nel buffer passato (*p_result)
*/
int buff_char_to_long(char* p_from, long* p_result)
{
  *p_result = 0;
  int cnt_char = 0;
  int i;
  for (i=0; i<10; i++){//loop di sicurezza
    if ( (*p_from < '0') || (*p_from > '9') ){
      return cnt_char;      //end char cifre, exit
    }
    cnt_char++;             //trovato char cifra
    *p_result = *p_result * 10;
    *p_result = *p_result + (long)(*p_from++ - '0');
  }
  return cnt_char;
}



/******************************************************************************************* */
#if 0
/*
genera una strinaga con il valore float convertito in char
nel buffer passato
*txt deve contenere la formattazione del float %f
*/
char* value_float_to_char(char* txt, float value, char* p_to)
{
  sprintf(loc_char_buff, (char*)txt, value);
  p_loc_char_buff = loc_char_buff;
  /* copia fino ai decimi*/
  while (*p_loc_char_buff != MIO_NULL){
    if (*p_loc_char_buff == '.'){
      *p_to++ = *p_loc_char_buff++;//metti punto
      *p_to++ = *p_loc_char_buff++;//metti decimi
      break;
    }
    else
      *p_to++ = *p_loc_char_buff++;//metti char
  }
  return p_to;//exit su primo char libero
}
#endif
/******************************************************************************************* */

