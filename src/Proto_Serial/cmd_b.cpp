/*
cmd_B info batteria
Risposta al comando
$,b,C,tensione batteria, stato ricarica, percento batteria, auto invio stringa, intervallo invio
  tensione batteria in millivolt
  stato ricarica:
     1 = on
     0 = off
    FF = batteria non presente
  percento batteria
    Vmax-Vmin
  auto invio
     N = disabilitato
     Y = abilitato
  intervallo in secondi
Comndo
  $,b,R  --> chiede invio stato batteria
  $,b,R,0  --> disabilota auto invio stringa
  $,b,R,xx --> abilita auto invio con l'intervallo xx in secondi

*/
#include <common_include.h>
#include "..\Battery_Task\battery_task.h"


static char* battery_info_to_str(char* p_to);


char cmd_b(char com)//info batteria
{
  proto_from++;//skip separatore
  if  ( (*proto_from == P_REQUEST_TYPE) || (*proto_from == P_REQUEST_TYPE_min) ){   //load & skip sepa
    proto_from++;//skip request type
    if (*proto_from == P_SEPARA) {
      proto_from++;//skip separatore
      if (*proto_from == '0'){
          Batt_Litio.com_sel = -1;//off invio automatico
      } 
      else {//no '0', test se esiste un numero per impostare il delay
        if (*proto_from >= '1' && *proto_from <= '9'){ //test se numero 
          //numero, devo leggere la stringa per convertire in cifra 
          int x;
          x = *proto_from++ - '0';
          if (*proto_from >= '0' && *proto_from <= '9'){
            x = x * 10 + (*proto_from - '0');
          }
          //in x ho quanti secondi di delay
          Batt_Litio.com_delay = x * L_SEC;//set delay invio 
          Batt_Litio.com_sel = com;//on invio automatico
        } 
      }
      proto_from++;//skip parametro - inutile, non ho altri dati da leggere
    }

    proto_to = proto_metti_cmd('b', proto_str);
    *proto_to++ = P_CONFIRM_TYPE;                   /* metti type */
    *proto_to++ = P_SEPARA;
    *proto_to++ = P_SEPARA;
    proto_to = battery_info_to_str(proto_to);
    proto_set_end_of_string(proto_to);
    return PROTO_NO_ERR;
  }
  return PROTO_ERR_TYPE_REQ;
}

/*
invia la stringa dati batterie
*/
char proto_invia_cmd_b(char com)
{
  char* p_buff = (char*)malloc(sizeof(char)*100);
  proto_set_end_of_string( battery_info_to_str(p_buff) );
  proto_tx_info('b', p_buff, com);
  free(p_buff);
  return TRUE;
}

/****
genera una stringa con i dati della batteria in *p_to
p_to = puntatore alla stringa dove generare la risposta
****/
static char* battery_info_to_str(char* p_to)
{
    /* metti tensione batteria */
  p_to += sprintf(p_to, "%d", Batt_Litio.v_batt);  
  *p_to++ = P_SEPARA;
  /* metti recharge state */
  if (Batt_Litio.v_batt_no_present){//yest se batteria presente
    p_to += sprintf(p_to, "F"); //batteria non presente
  }
  else {
    if (Batt_Litio.recharge)
      p_to += sprintf(p_to, "Y");//batteria in carica
    else
      p_to += sprintf(p_to, "N");//batteria non in carica
  }
  *p_to++ = P_SEPARA;
  /* metti percento batteria */
  p_to += sprintf(p_to, "%d", Batt_Litio.v_batt_percento);
  *p_to++ = P_SEPARA;
  //metti invio auto enable/disable
  if (Batt_Litio.com_sel == -1)
    *p_to++ = 'N';
  else
    *p_to++ = 'Y';
  *p_to++ = P_SEPARA;
  //metti valore delay invio automatico in secondi
  p_to += sprintf(p_to, "%d", (Batt_Litio.com_delay / L_SEC ) );
  return p_to;
}