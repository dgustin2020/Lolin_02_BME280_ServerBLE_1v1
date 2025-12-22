/*
cmd_p.cpp
gestione arametrin operativi salvati in flash
leggo i valori memorizzati
modifico i valori memorizzati e li salvo in flash
*/
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>
#include <my_Task_Gest.h>
#include <my_itr_time.h>
#include <usb_gest.h>
#include <my_string.h>
#include <proto.h>
#include <proto_common.h>
//#include <my_Flash.h>
//#include <..\src\Flash_param\Flash_param.h>
#include  <..\..\src\preferenze_gest\pref_gest.h>



/* usata per leggere o programmare i vari parametri operativi che si trovano in flash
la stringa:
$,p,R,0 -> xx,r / w, CR LF	
0 -> xx : è il parametro operativo fa leggere, da 0 a xx (vedi tabella flash_parametri.h)
r / w   : indica se parametro da leggere o da scrivere

Con parametro == '0', 
- tipo read (r), viene inviata su seriale la lista dei parametri programmati
- tipo write (w), i parametri vengono salvati in flash ("$,p,C,0,w,")
- tipo (w,d), salva valori default ("$,p,C,0,w,d")
Risposta 
Tutto ok => "$,p,C,0,x,1"  dove (x --< 'r' o 'w'):
Errore   => "$,p,C,0,x,0"  dove (x --< 'r' o 'w'):

Scrivi prametro
"$,p,R,xx,w,val,CR LF"	dove 'xx' è il parametro operativo e 'val' è il valore da programmare
in risposta abbiamo la stringa $,p,C,xx,W, Cr LF
Leggi parametro
"$,p,R,xx,r,CR LF"	dove 'xx' è il parametro operativo da leggere
in risposta abbiamo $,p,R,xx,r,val
**/

/* variabili */
static char read_write_cmd = 0;
/* procedure */
static char* set_common_start_answear(long* p_parametro);

/****/
char cmd_p(char com)//_param_gest
{
  long parametro;
  int n_char;
  proto_from++;//skip separatore
  if ( (*proto_from != P_REQUEST_TYPE) && (*proto_from != P_REQUEST_TYPE_min) )   //test se comando richiesta
    return PROTO_ERR_TYPE_REQ; //no richiesta, esco con errore  
  proto_from++;//skip request type
  proto_from++; //skip separatore

  //leggi parametro
  n_char = buff_char_to_long(proto_from, (long*)&parametro);
  proto_from = proto_from + n_char;//goto dopo char cifre
  if (!n_char){
    return PROTO_ERR_SUB_CMD;//nessun parametro trovato, segnalo errore
  }
  if (parametro >= PREF_BASE_TBL_REC_MAX){//test se parametro nella list
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
  se tipo read, leggo da buffer "base_param" e lo metto in buffer
  se tipo write, a seconda del tipo parametro (stringa, long, int), leggo dato da seriala
  e lo metto in buffer "base_param"
  */
  T_pref_record** p_tbl_rec;
  p_tbl_rec = tbl_base_param_rec;

  if (read_write_cmd == P_READ_COMMAND)
  {  //tipo read

    /* metti comando riuscito */
    *proto_to++ = COMMAND_OK;
    *proto_to++ = P_SEPARA;

    #ifdef DEBUG_FLASH
    Serial.printf("parametro= %d\r\n", parametro);
    #endif
    // a seconda del parmetro ricevuto, invio risposta
    long* l = (long*)p_tbl_rec[parametro]->p_key_value;
    switch (parametro)
    {
      case PREF_BASE_READ_ALL_IDX://richiesta invio tuttii parametri
        pref_gest_write_all_param_to_serial((T_pref_record**)&tbl_base_param_rec, com);
        break;
      default://alri parametri
        if ( (parametro < PREF_BASE_TBL_REC_MAX) && (p_tbl_rec[parametro]) )//test se parametro esiste
        {//minore
          switch ( p_tbl_rec[parametro]->value_type){
            case PARAM_STRING:
              proto_to = proto_to +  sprintf(proto_to,"%s", p_tbl_rec[parametro]->p_key_value);
              break;
            case PARAM_LONG:
              proto_to = proto_to +  sprintf(proto_to,"%i", *l);
              break;
            case PARAM_INT:
              proto_to = proto_to +  sprintf(proto_to,"%i", *l);
              break;
            case PARAM_HEX:
              proto_to = proto_to +  sprintf(proto_to,"%2x", *l);
              break;
            default:
              proto_to = proto_to +  sprintf(proto_to,"%s",(char*)"ERR parameter value type");
              break;
          }
        }
        else 
        {//magiore segnalo errore
          proto_to = proto_to +  sprintf(proto_to,"%s",(char*)"ERR no valid parameter");
        }
        break;//end default
    }
    /* metti comando riuscito */
//    *proto_to++ = COMMAND_OK;
//    *proto_to++ = P_SEPARA;
  }
  else 
  {//tipo write
    switch (parametro){
      case PREF_BASE_READ_ALL_IDX://richiesta salva parametri
        /*verifica se richiesta rimetti valori default: se trovo 'd' metto default*/
        if (*proto_from == 'd'){
          pref_gest_set_all_param_default((T_pref_record**)&tbl_base_param_rec, SAVE_DEFAULT_TO_SPACE_YES);//salva valori default
        }        
        else {
          pref_gest_save_all_param_to_space((T_pref_record**)&tbl_base_param_rec, (Preferences*)&impo_base_space); //write valori attuali
        }
        break;  //end PREF_BASE_READ_ALL
      default:
        if ( (parametro < PREF_BASE_TBL_REC_MAX) && (p_tbl_rec[parametro]) )
        { //parametro disponibile //richiesta imposta singolo parametro
          long* p_long;
          long  val_l;
          char* p_char;;
          char  val_c;
          switch (p_tbl_rec[parametro]->value_type)
          {
            case PARAM_STRING://tipo stringa, leggi stringa inviata
              p_char = (char*)p_tbl_rec[parametro]->p_key_value;
              n_char = 0;
              while(*proto_from != 0){
                val_c = *proto_from++;
                if ( (val_c == P_SEPARA) || (val_c == P_END_STRING) ){//test se finito parametro
                  break;
                }
                *p_char++ = val_c;
                n_char++;
              }
              *p_char = 0;
              if (n_char == 0){//test se ho testo da memorizzare
                proto_to = proto_str;//riposiziona ptr out stringa
                return PROTO_ERR_SUB_SUB_CMD;//no testo, segnalo errore
              }
              break;
            case PARAM_LONG://type long, leggi long
              p_long = (long*)p_tbl_rec[parametro]->p_key_value;
              n_char = buff_char_to_long(proto_from,p_long);
              break;
            case PARAM_INT://convrti in int
              p_long = (long*)p_tbl_rec[parametro]->p_key_value;
              n_char = buff_char_to_long(proto_from,p_long);
              break;
          }
        }
        else 
        {  //parametron non disponibile, segnalare
          proto_to = proto_to +  sprintf(proto_to,"%s",(char*)"ERR no valid parameter");
        }
        break;//end default
    }//end switch parametro
    /* metti comando riuscito*/
    *proto_to++ = COMMAND_OK;
    *proto_to++ = P_SEPARA;
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