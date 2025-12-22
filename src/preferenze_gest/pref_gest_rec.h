/*
pref_gest_rec.h
*/
#ifndef _PREF_GEST_REC_H
#define _PREF_GEST_REC_H

/* definizione chiavi usate nel space di NVS Flash */
#define BASE_SPACE_NAME             "baseXX"
#define BASE_CKS_LEN                2
#define BASE_NAME_LEN               48
#define BASE_BLUE_PIN_LEN           24
/**/
#define BASE_DEV_NAME               "base00"          //  string      firmware[FIRMWARE_LEN];          // 1 2..33   32 byte
#define BASE_DEV_NAME_LEN           BASE_NAME_LEN     //  4 x uint32_t
#define BASE_DEV_NAME_CKS           "base00_cks"      //  uint16_t    firmware_check;                   // 1 34..35   4 byte  
/**/
#define BASE_DEV_SERIE              "base01"          //  string      n_serie[N_SERIE_LEN];            // 2 34..41    8
#define BASE_DEV_SERIE_LEN          8                 //  2 x unit32_t
#define BASE_DEV_SERIE_CKS          "base01_cks"      //  uint16_t    n_serie_check;                    // 2 42..43    4
/**/
#define BASE_WIFI_NAME              "base02"          //  string      wifi_name[WIFI_NAME_LEN];        // 3 42..141   100
#define BASE_WIFI_NAME_LEN          BASE_NAME_LEN     // 25 x uint32_t
#define BASE_WIFI_NAME_CKS          "base02_cks"      //  uint16_t    wifi_name_check;                 // 3 142..143   4
/**/
#define BASE_WIFI_PSW               "base03"          //  string      wifi_psw[WIFI_PSW_LEN];          // 4 142..189   48
#define BASE_WIFI_PSW_LEN           BASE_NAME_LEN     // 12 x uint32_t
#define BASE_WIFI_PSW_CKS           "base03_cks"      //  uint16_t    wifi_psw_check;                  // 4 190..191   4
/**/
#define BASE_BLUE_NAME_SLAVE        "base04"          //  string      blue_name_slave[BLUE_NAME_LEN];  // 5 190..289  100
#define BASE_BLUE_NAME_SLAVE_LEN    BASE_NAME_LEN     // 25 x uint32_t
#define BASE_BLUE_NAME_SLAVE_CKS    "base04_cks"      //  uint16_t    blue_name_slave_check;           // 5 290..291   4
/**/
#define BASE_BLUE_NAME_MASTER       "base05"          //  string      blue_name_master[BLUE_NAME_LEN]; // 7 314..413  100
#define BASE_BLUE_NAME_MASTER_LEN   BASE_NAME_LEN     // 25 x uint32_t  
#define BASE_BLUE_NAME_MASTER_CKS   "base05_cks"      //  uint16_t    blue_name_master_check;          // 7 414..415   4
/**/
#define BASE_BLUE_PIN_SLAVE         "base06"          //  string      blue_pin_slave[BLUE_PIN_LEN];    // 6 290..313   24
#define BASE_BLUE_PIN_SLAVE_LEN     BASE_BLUE_PIN_LEN //  6 x uint32_t
#define BASE_BLUE_PIN_SLAVE_CKS     "base06_cks"      //  uint16_t    blue_pin_slave_check;            // 6 314..315   4
/**/
#define BASE_BLUE_PIN_MASTER        "base07"          //  string      blue_pin_master[BLUE_PIN_LEN];   // 8 414..437   24
#define BASE_BLUE_PIN_MASTER_LEN    BASE_BLUE_PIN_LEN //  6 x uint32_t  
#define BASE_BLUE_PIN_MASTER_CKS    "base07_cks"      //  uint16_t    blue_pin_master_check;           // 8 438..439   4
/**/
#define BASE_BLUE_NAME_REMOTE       "base08"          //  string      blue_name_remote[BLUE_NAME_LEN]; // 9 438..537  100
#define BASE_BLUE_NAME_REMOTE_LEN   BASE_NAME_LEN     // 25 x uint32_t
#define BASE_BLUE_NAME_REMOTE_CKS   "base08_cks"      //  uint16_t    blue_name_remote_check;          // 9 538..539   4
/**/
#define BASE_NTP_SERVER             "base09"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_NTP_SERVER_LEN         BASE_NAME_LEN     // 25 x uint32_t
#define BASE_NTP_SERVER_CKS         "base09_cks"      //  uint16_t    ntp_server_check;                //10 638..639   4
/**/
#define BASE_GMT_OFFSET             "base10"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_GMT_OFFSET_LEN         sizeof(T_Lclk)     // 25 x uint32_t
#define BASE_GMT_OFFSET_CKS         "base10_cks"      //  uint16_t    ntp_server_check;                //10 638..639   4
/**/
#define BASE_NTP_LEGAL_OFFSET       "base11"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_NTP_LEGAL_OFFSET_LEN   sizeof(T_Lclk)     // 25 x uint32_t
#define BASE_NTP_LEGAL_OFFSET_CKS   "base11_cks"      //  uint16_t    ntp_server_check;                //10 638..639   4
/**/
#define BASE_XTAL_SEL               "base12"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_XTAL_SEL_LEN           sizeof(uint16_t)     // 25 x uint32_t
#define BASE_XTAL_SEL_CKS           "base12_cks"      //  uint16_t    ntp_server_check;                //10 638..639   4
/**/
#define BASE_XTAL_BASE              "base13"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_XTAL_BASE_LEN          sizeof(uint64_t)  // 25 x uint32_t
#define BASE_XTAL_BASE_CKS          "base13_cks"      //  uint16_t    ntp_server_check;                //10 638..639   4
/**/
#define BASE_XTAL_BASE_1            "base14"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_XTAL_BASE_1_LEN         sizeof(uint64_t) // 25 x uint32_t
#define BASE_XTAL_BASE_1_CKS         "base14_cks"     //  uint16_t    ntp_server_check;                //10 638..639   4
/**/
#define BASE_XTAL_CHECK_SUM         "base15"          //  string      ntp_server[NTP_SERVER_LEN];      //10 538..637  100
#define BASE_XTAL_CHECK_SUM_LEN     sizeof(uint64_t)     // 25 x uint32_t
#define BASE_XTAL_CHECK_SUM_CKS     "base15_cks"      //  uint16_t    ntp_server_check;                //10 638..639   4
/**/

/* struttura record gestione parametri */
typedef struct {
  char const* p_space_name;         //-> const nome spazio
  char const* p_key_name;           //-> const nome chiave
  char const* p_key_name_cks;       //-> const nome chiave checksum
  void const* p_key_value_default;  //-> const valore chiave default
  int  const  key_value_len;        //-> numer char valore chiave
  void*       p_key_value;          //-> RAM buffer valore chiave
  uint8_t     value_type;           //-> true se string, false altrimenti
  char const* value_descri;         //-> descrizione testo del parametro
}T_pref_record;

#define PARAM_REC_START_IDX         1 //serve per generare la lista da inviare su seriale

extern T_pref_record* tbl_base_param_rec[];

#define PREF_BASE_READ_ALL_IDX          0
#define PREF_BASE_DEV_NAME_IDX          1
#define PREF_BASE_DEV_SERIE_IDX         2//PREF_BASE_DEV_NAME_IDX + 1
#define PREF_BASE_WIFI_NAME_IDX         3//PREF_BASE_GMT_OFFSET_IDX + 1
#define PREF_BASE_WIFI_PSW_IDX          4//PREF_BASE_WIFI_NAME_IDX + 1
#define PREF_BASE_BLUE_NAME_SLAVE_IDX   5//PREF_BASE_WIFI_PSW_IDX + 1
#define PREF_BASE_BLUE_PIN_SLAVE_IDX    6//PREF_BASE_BLUE_NAME_SLAVE_IDX + 1
#define PREF_BASE_BLUE_NAME_MASTER_IDX  7//PREF_BASE_BLUE_PIN_SLAVE_IDX + 1
#define PREF_BASE_BLUE_PIN_MASTER_IDX   8//PREF_BASE_BLUE_NAME_MASTER_IDX + 1
#define PREF_BASE_BLUE_NAME_REMOTE_IDX  9//PREF_BASE_BLUE_PIN_MASTER_IDX + 1
#define PREF_BASE_NTP_SERVER_IDX        10//PREF_BASE_BLUE_NAME_REMOTE_IDX + 1
#define PREF_BASE_GMT_OFFSET_IDX        11//PREF_BASE_NTP_SERVER_IDX + 1
#define PREF_BASE_NTP_LEGAL_OFFSET_IDX  12//PREF_BASE_GMT_OFFSET_IDX + 1
#define PREF_BASE_XTAL_SEL_IDX          13
#define PREF_BASE_XTAL_BASE_IDX         14
#define PREF_BASE_XTAL_BASE_1_IDX       15
#define PREF_BASE_XTAL_CHECKSUM_IDX     16
#define PREF_BASE_TBL_REC_MAX           17 //////////////////

/* tipo di dato del parametro */
#define PARAM_END_TBL   0 // fine tsbella
#define PARAM_STRING    1 // tipo stringa
#define PARAM_LONG      2 // tipo T_Lclk / long
#define PARAM_INT       3 // tipo integer
#define PARAM_HEX       4 // tipo esadecimale
#define PARAM_BOOL      5 // tipo bool, singolo byte

/*
************************************************************************************************ 
 * parametri utente generici come volume, gest sensore, backlight LCD, ecc.
*************************************************************************************************
*/
/* definizione chiavi usate nel space di NVS Flash */
#define USER_SPACE_NAME                     "userXX"

/**/
#define USER_SENS_TASK_ENABLE               "user01"          //  
#define USER_SENS_TASK_ENABLE_LEN           sizeof(char)      //  1 x uint8_t
#define USER_SENS_TASK_ENABLE_CKS           "user01_cks"      //  
/**/
#define USER_SENS_AUTOEMIT_DELAY            "user02"          //  
#define USER_SENS_AUTOEMIT_DELAY_LEN        sizeof(int)      //   1 x int
#define USER_SENS_AUTOEMIT_DELAY_CKS        "user02_cks"      //  
/**/

extern T_pref_record* tbl_user_param_rec[];
#define PREF_USER_READ_ALL_IDX                 0
#define PREF_USER_SENS_TASK_ENABLE_IDX         1
#define PREF_USER_SENS_AUTOEMIT_DELAY_IDX      2
#define PREF_USER_TBL_REC_MAX                  3

#endif //_PREF_GEST_REC_H