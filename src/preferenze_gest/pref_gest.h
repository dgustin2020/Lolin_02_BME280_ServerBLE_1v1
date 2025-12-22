/*
pref_gest.h

gestione PREF_BASEmetri usando la libreria "preference" 

*/
#ifndef _PREF_GEST_H
#define _PREF_GEST_H
#include <Preferences.h>
#include "pref_gest_rec.h"

extern Preferences impo_base_space;
extern Preferences impo_user_space;

/* i PREF_BASEmetri operativi usati dal sistema */
typedef struct {
char      dev_name[BASE_NAME_LEN];                      
char      dev_serie[BASE_DEV_SERIE_LEN];            
char      wifi_name[BASE_WIFI_NAME_LEN];          // 3 42..141   100
char      wifi_psw[BASE_WIFI_PSW_LEN];           // 4 142..189   48
char      blue_name_slave[BASE_NAME_LEN];    // 5 190..289  100
char      blue_pin_slave[BASE_BLUE_PIN_LEN];     // 6 290..313   24
char      blue_name_master[BASE_NAME_LEN];   // 7 314..413  100
char      blue_pin_master[BASE_BLUE_PIN_LEN];    // 8 414..437   24
char      blue_name_remote[BASE_NAME_LEN];   // 9 438..537  100
char      ntp_server[BASE_NAME_LEN];            //10 538..637  100
T_Lclk    ntp_gmt_offset;                        //11 640..643  4 [NTP_GMT_OFFSET_LEN];//long
T_Lclk    ntp_legal_offset;                     //12 644..647  4 [NTP_LEGAL_OFFSET_LEN];//int
int16_t   xtal_sel;                        //13 648..655  8 calibrazione xtal valore da compare a 'cnt_reload' x  usare cal_base o cal_base_1 
uint64_t  xtal_base;                       //14 656..663  8 calibrazione xtal -> valore da sommare a 'itr_compare_buff' x prossimo utr
uint64_t  xtal_base_1;                     //15 664..671  8 calibrazione xtal -> valore da sommare a 'itr_compare_buff' x prossimo utrchar firmware[FIRMWARE_LEN];
uint64_t  xtal_check_sum;                  //16 672..679  8 xtal check_sum
} T_base_param;
//  uint16_t  checksum; 
extern T_base_param base_param;

/******************************************************************************** */
/* i PREF_BASEmetri utente usati  */
typedef struct {
char      sens_task_enable;
int       sens_autoemit_delay;
} T_user_param;
extern T_user_param user_param;
extern uint16_t user_param_write_flag;//parametri modificati, il singolo bit indica quale
#define USER_SENS_WRITE       (1<<0)
#define USER_VOLUME_WRITE     (1<<1)
#define USER_BACKLITE_WRITE   (1<<2)

/********************************************************* */
/*procedure*/
extern void     pref_gest_save_all_param_to_space(T_pref_record** p_tbl_rec, Preferences* p_space);
extern void     pref_gest_save_param_to_space(T_pref_record *p_rec, Preferences* p_space);
extern bool     pref_gest_load_all_param_from_space(T_pref_record** p_tbl_rec, Preferences* p_space);
extern uint16_t pref_gest_load_param_from_space(T_pref_record *p_rec, Preferences* p_space);
extern void     pref_gest_set_all_param_default(T_pref_record** p_tbl_rec, bool save_nvs_flash);
#define SAVE_DEFAULT_TO_SPACE_YES       true
#define SAVE_DEFAULT_TO_SPACE_NO        false
#define PREF_LOAD_PARAM_OK              0
#define PREF_LOAD_PARAM_NOT_EXIST       1
#define PREF_LOAD_PARAM_CKS_NOT_EXIST   2
#define PREF_LOAD_PARAM_CKS_ERR         3     

#ifdef USE_MY_PROT_SERIAL
extern void     pref_gest_write_all_param_to_serial(T_pref_record** p_tbl_rec, bool com);
#else
extern void     pref_gest_write_all_param_to_serial(T_pref_record** p_tbl_rec);
#endif

#endif //_PREF_GEST_H