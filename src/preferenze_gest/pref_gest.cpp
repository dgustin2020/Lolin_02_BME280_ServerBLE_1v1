/*
pref_gest.cpp
*/
#include  <common_include.h>
#include  <Preferences.h>
#include  "..\version\version.h"
#include  "..\local_default.h"
#ifdef USE_MY_PROT_SERIAL
#include  "../../src/Blue_gest/blue_gest.h"
#endif

//#define PREF_DEBUG

Preferences impo_base_space;
Preferences impo_user_space;

T_base_param base_param;
T_user_param user_param;
uint16_t user_param_write_flag = 0;//singolo bit segnala quali parametri modificati

/************************************************************************* */
/*
carica tuttii parametri nel buffer in RAM
Exit:
false -> se tutto ok, valori caricari da flash
true  -> errori nel caricamento, messi valori default
*/
bool pref_gest_load_all_param_from_space(T_pref_record** p_tbl_rec, Preferences* p_space)
{
  char idx = PARAM_REC_START_IDX;
  bool result = false;
  while (p_tbl_rec[idx])
  {
    T_pref_record* p_rec = *p_tbl_rec;
    uint16_t res = pref_gest_load_param_from_space(p_tbl_rec[idx], p_space);
    if ( res != PREF_LOAD_PARAM_OK ){
      result = true; //se errore carico parametro, segnalo carico valori default
    }
    idx++;
  }
  return result;
}

/*
salva tuttii parametri in NVS flash
*/
void pref_gest_save_all_param_to_space(T_pref_record** p_tbl_rec, Preferences* p_space)
{
  char idx = PARAM_REC_START_IDX;
  while (p_tbl_rec[idx])
  {
    pref_gest_save_param_to_space(p_tbl_rec[idx], p_space);
    idx++;
  }
}

/*
salva il valore della chiave nello spazio passato con T_pref_record *p_rec
calcola il checksum e lo salva nella relativa chiave
*/
void pref_gest_save_param_to_space(T_pref_record *p_rec, Preferences* p_space )
{
  uint16_t check_calc;
  //open spave name
  p_space->begin(p_rec->p_space_name, 0); //apro parametri base in sola lettura
  //save parametro in key
  p_space->putBytes(p_rec->p_key_name, p_rec->p_key_value, p_rec->key_value_len); //write value
  //calcola checksuma
  check_calc = variabili_calcola_checksum((uint8_t*)p_rec->p_key_value, p_rec->key_value_len);
  //save checksum
  p_space->putBytes(p_rec->p_key_name_cks, (uint8_t*)&check_calc, BASE_CKS_LEN); //write value
  //close space
  p_space->end();
}

/*
recupera il valore della chiave contenuto in T_pref_record *p_rec
Se la chiave esiste, verifica se il relativo checksum è corretto con il volre memorizzato
Se corretto, carica il valore in ram ed esce con 0
Exit:
0 -> tutto ok, valore letto caricato in RAM
1 -> la chiave non esiste, valore default caricato in RAM
2 -> La chiave esiste ma non esiste la relativa chiave checksum, valore default caricato in RAM
3 -> La chiave e la chiave checksum esistono, ma il checksum non è corretto, valore default caricato in RAM
*/
uint16_t pref_gest_load_param_from_space(T_pref_record *p_rec, Preferences* p_space)
{
  size_t len;
  uint16_t check_nvs;//checksum letto dall NVS
  uint16_t check_calc;
  p_space->begin(p_rec->p_space_name, 0); //apro parametri base in sola lettura
  #ifdef PREF_DEBUG
  Serial.printf("PREF test if key %s esiste len=%d\r\n", p_rec->p_key_name, p_rec->key_value_len);
  #endif
  if (p_space->isKey(p_rec->p_key_name)){ //verifico se esiste la chiave
    //esiste key, leggo parametro
    #ifdef PREF_DEBUG
    Serial.printf("PREF key %s esiste len=%d, read\r\n", p_rec->p_key_name, p_rec->key_value_len);
    #endif
    p_space->getBytes(p_rec->p_key_name, p_rec->p_key_value, p_rec->key_value_len); //read value
    //test se esiste checksum
    #ifdef PREF_DEBUG
    Serial.printf("PREF test if key %s esiste len=%d\r\n", p_rec->p_key_name_cks, p_rec->key_value_len);
    #endif
    if (p_space->isKey(p_rec->p_key_name_cks)){//test se esiste checksum della varaibile
      //esiste, verifica se corretto con valore letto
      p_space->getBytes(p_rec->p_key_name_cks, (uint8_t*)&check_nvs, BASE_CKS_LEN);//read checksum
      #ifdef PREF_DEBUG
      Serial.printf("PREF key %s esiste len=%d val=%d, \r\n", p_rec->p_key_name_cks, p_rec->key_value_len, check_nvs);
      #endif
      check_calc = variabili_calcola_checksum((uint8_t*)p_rec->p_key_value, p_rec->key_value_len);
      #ifdef PREF_DEBUG
      Serial.printf("chek read =%d check calc=%d val=%s len=%d\r\n", check_nvs, check_calc, p_rec->p_key_value, p_rec->key_value_len);
      #endif
      if (check_nvs == check_calc){//test se checksum corretto
        //checksum OK, segnalo e ed esco
        #ifdef PREF_DEBUG
        Serial.printf("PREF key %s - %s - %d, read\r\n", p_rec->p_key_name, p_rec->p_key_value, p_rec->key_value_len);
        #endif
        len = PREF_LOAD_PARAM_OK;//valore ok
      }
      else {
        #ifdef PREF_DEBUG
        Serial.printf("PREF key %s checksum error, set default\r\n", p_rec->p_key_name);
        #endif
        //load default in ram buffer
        if (p_rec->value_type == PARAM_STRING)
          memcpy((char*)p_rec->p_key_value, (char*)p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
        else
          memcpy((char*)p_rec->p_key_value, (char*)&p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
        len = PREF_LOAD_PARAM_CKS_ERR;//errore checksum
      }
    }
    else {
      //cecksum non esiste, segnalo e metto valori default
      #ifdef PREF_DEBUG
      Serial.printf("PREF key %s checksum non esiste, set default\r\n", p_rec->p_key_name);
      #endif
      //load default in ram buffer
      if (p_rec->value_type == PARAM_STRING)
        memcpy((char*)p_rec->p_key_value, (char*)p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
      else
        memcpy((char*)p_rec->p_key_value, (char*)&p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
      len = PREF_LOAD_PARAM_CKS_NOT_EXIST;//errore checksum non esiste
    }
  }
  else {
    //key non esiste, segnalo e metto valore defalt
    #ifdef PREF_DEBUG
    Serial.printf("PREF key %s non esiste %d, write\r\n", p_rec->p_key_name, p_rec->key_value_len);
    Serial.printf("PREF key default=%d\r\n", p_rec->p_key_value_default);
    Serial.printf("PREF key value=%x\r\n", p_rec->p_key_value);
    #endif
    //load default in ram buffer
    if (p_rec->value_type == PARAM_STRING)
      memcpy((char*)p_rec->p_key_value, (char*)p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
    else
      memcpy((char*)p_rec->p_key_value, (char*)&p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
    len = PREF_LOAD_PARAM_NOT_EXIST;//chiave non esiste
  }
  p_space->end();
  return len;
}

/*********************************************************************************************** */
/*********************************************************************************************** */
/*
mette tutti i parametri in buffer RAM al vlore default
save_nvs_flash = true --> salva i parametri in nvs_flash
               = fasle -> non salva
*/
void pref_gest_set_all_param_default(T_pref_record** p_tbl_rec, bool save_nvs_flash)
{
  uint8_t idx_param = PARAM_REC_START_IDX;
  T_pref_record* p_rec; 
  while (p_tbl_rec[idx_param]){//test se esiste ptr al record
    p_rec = p_tbl_rec[idx_param];
    //load default in ram buffer
    if (p_rec->value_type == PARAM_STRING)
      memcpy((char*)p_rec->p_key_value, (char*)p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
    else
      memcpy((char*)p_rec->p_key_value, (char*)&p_rec->p_key_value_default, p_rec->key_value_len);//load valore default in buffer
    idx_param++;
    if (save_nvs_flash)//test se salvare in nvs flash
      pref_gest_save_all_param_to_space(p_tbl_rec, (Preferences*)&impo_base_space);
  }
}

/**/
#ifdef USE_MY_PROT_SERIAL
void pref_gest_write_all_param_to_serial(T_pref_record** p_tbl_rec, bool com)
#else
void pref_gest_write_all_param_to_serial(T_pref_record** p_tbl_rec)
#endif
{
  uint8_t idx_param = PARAM_REC_START_IDX;//primo record da gestire
  uint8_t n_char;
  char* p_mem = (char*)malloc(200 * sizeof(char));
  /*metti intestazione start*/
  if (strcmp( (char*)p_tbl_rec[idx_param]->p_space_name, (char*)BASE_SPACE_NAME) )
    sprintf(p_mem,(char*)"List Parameters....: actual value - default value\r\n");
  else
    sprintf(p_mem,(char*)"List Parameters....: actual value - default value\r\n");
  #ifdef USE_MY_PROT_SERIAL
  if (!com){
    usb_232.load_tx_my_string((uint8_t*)p_mem);      // --> rs232
  }
  else {
    blue_232.load_tx_my_string((uint8_t*)p_mem);     //  --> blue  
  }
  #else
  Serial.printf("%s\r\n", p_mem);
  #endif

  //metti valore del parametro a seconda del tipo 
  //while (idx_param < PARAM_TBL_REC_MAX){
  while (p_tbl_rec[idx_param]){//test se esiste ptr al record
    char* p_to = p_mem;
    n_char = sprintf(p_to, "%s", p_tbl_rec[idx_param]->value_descri);
    p_to += n_char;
    long* l = (long*)p_tbl_rec[idx_param]->p_key_value;
    switch (p_tbl_rec[idx_param]->value_type){
       case PARAM_BOOL:
        //proto_to = proto_to + sprintf(proto_to,"%c\r\n", p_tbl_rec[idx_param]->p_key_value);
        n_char = sprintf(p_to," %d - %d\r\n", *(char*)p_tbl_rec[idx_param]->p_key_value, p_tbl_rec[idx_param]->p_key_value_default);
        break;
      case PARAM_STRING:
        n_char = sprintf(p_to," %s - %s\r\n", p_tbl_rec[idx_param]->p_key_value, p_tbl_rec[idx_param]->p_key_value_default);
        break;
      case PARAM_LONG:
        n_char = sprintf(p_to," %i - %i\r\n", *l, p_tbl_rec[idx_param]->p_key_value_default);
        break;
      case PARAM_INT:
        n_char = sprintf(p_to," %i - %i\r\n", *l, p_tbl_rec[idx_param]->p_key_value_default);
        break;
      case PARAM_HEX:
        n_char = sprintf(p_to," %2x - %2x\r\n", *l, p_tbl_rec[idx_param]->p_key_value_default);
        break;
      default:
        n_char = sprintf(p_to,"%s type=%i",(char*)"Flash Param Error Type", p_tbl_rec[idx_param]->value_type);
        break;
    }
    p_to += n_char;
    *p_to = MIO_NULL;
    #ifdef USE_MY_PROT_SERIAL
    if (!com){
      usb_232.load_tx_my_string((uint8_t*)p_mem);      // --> rs232
    }
    else {
      blue_232.load_tx_my_string((uint8_t*)p_mem);     //  --> blue  
    }
    #else
    Serial.print(p_mem);
    #endif    
    idx_param++;  
  }
  /*metti intestazione end*/
  sprintf(p_mem,(char*)"END List Param\r\n");
  #ifdef USE_MY_PROT_SERIAL
  if (!com){
    usb_232.load_tx_my_string((uint8_t*)p_mem);      // --> rs232
  }
  else {
    blue_232.load_tx_my_string((uint8_t*)p_mem);     //  --> blue  
  }
  #else
  Serial.printf("%s\r\n", p_mem);
  #endif
  free(p_mem);//clear buffer ram x seriale
}

