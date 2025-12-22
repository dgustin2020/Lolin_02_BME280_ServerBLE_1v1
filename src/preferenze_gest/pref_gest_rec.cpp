/*
pref_gest_rec.cpp
*/
#include  <common_include.h>
#include  "..\version\version.h"
#include  "..\local_default.h"
#include  "pref_gest.h"


/* base00 */
T_pref_record rec_dev_name ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_DEV_NAME,
  (char const*)BASE_DEV_NAME_CKS,
  (char const*)NOME_PROGRAMMA,
  BASE_DEV_NAME_LEN,
  &base_param.dev_name,
  PARAM_STRING, //string_type
  (char const*)" 1 dev Name........."
};
/* base01 */
T_pref_record rec_dev_serie ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_DEV_SERIE,
  (char const*)BASE_DEV_SERIE_CKS,
  (char const*)SERIAL_NUMBER_DEF,
  BASE_DEV_SERIE_LEN,
  &base_param.dev_serie,
  PARAM_STRING, //string_type
  (char const*)" 2 dev Serie........"
};
/* base02 */
T_pref_record rec_wifi_name ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_WIFI_NAME,
  (char const*)BASE_WIFI_NAME_CKS,
  (char const*)WIFI_NAME_DEF,
  BASE_WIFI_NAME_LEN,
  &base_param.wifi_name,
  PARAM_STRING, //string_type
  (char const*)" 3 WiFi Name........"
};
/* base03*/
T_pref_record rec_wifi_psw ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_WIFI_PSW,
  (char const*)BASE_WIFI_PSW_CKS,
  (char const*)WIFI_PSW_DEF,
  BASE_WIFI_PSW_LEN,
  &base_param.wifi_psw,
  PARAM_STRING, //string_type
  (char const*)" 4 WiFi psw........."
};
/*base04*/
T_pref_record rec_blue_name_slave ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_BLUE_NAME_SLAVE,
  (char const*)BASE_BLUE_NAME_SLAVE_CKS,
  (char const*)BLUE_NAME_SLAVE_DEF,
  BASE_BLUE_NAME_SLAVE_LEN,
  &base_param.blue_name_slave,
  PARAM_STRING, //string_type
  (char const*)" 5 Blue Name Slave.."
};
/*base05*/
T_pref_record rec_blue_name_master ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_BLUE_NAME_MASTER,
  (char const*)BASE_BLUE_NAME_MASTER_CKS,
  (char const*)BLUE_NAME_MASTER_DEF,
  BASE_BLUE_NAME_MASTER_LEN,
  &base_param.blue_name_master,
  PARAM_STRING, //string_type
  (char const*)" 7 Blue Name Master."
};
/*base06*/
T_pref_record rec_blue_pin_slave ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_BLUE_PIN_SLAVE,
  (char const*)BASE_BLUE_PIN_SLAVE_CKS,
  (char const*)BLUE_PIN_SLAVE_DEF,
  BASE_BLUE_PIN_SLAVE_LEN,
  &base_param.blue_pin_slave,
  PARAM_STRING, //string_type
  (char const*)" 6 Blue Pin Slave..."
};
/*base07*/
T_pref_record rec_blue_pin_master ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_BLUE_PIN_MASTER,
  (char const*)BASE_BLUE_PIN_MASTER_CKS,
  (char const*)BLUE_PIN_MASTER_DEF,
  BASE_BLUE_PIN_MASTER_LEN,
  &base_param.blue_pin_master,
  PARAM_STRING, //string_type
  (char const*)" 0 Blue Pin Master.."
};
/*base08*/
T_pref_record rec_blue_name_remote ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_BLUE_NAME_REMOTE,
  (char const*)BASE_BLUE_NAME_REMOTE_CKS,
  (char const*)"0",
  BASE_BLUE_NAME_REMOTE_LEN,
  &base_param.blue_name_remote,
  PARAM_STRING, //string_type
  (char const*)" 9 Blue Name Remote."
};
/*base009*/
T_pref_record rec_ntp_server ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_NTP_SERVER,
  (char const*)BASE_NTP_SERVER_CKS,
  (char const*)NTP_SERVER_DEF,
  BASE_NTP_SERVER_LEN,
  &base_param.ntp_server,
  PARAM_STRING, //string_type
  (char const*)"10 NTP Server......."
};
/* base10 */
T_Lclk const NTP_GMT_OFFSET_DEF_VAL = NTP_GMT_OFFSET_DEF;
T_pref_record rec_gmt_offset ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_GMT_OFFSET,
  (char const*)BASE_GMT_OFFSET_CKS,
  (T_Lclk const*)NTP_GMT_OFFSET_DEF_VAL,
  BASE_GMT_OFFSET_LEN,
  &base_param.ntp_gmt_offset,
  PARAM_LONG, //no string_type
  (char const*)"11 NTP GMT offset..."
};
/* base11 */
T_Lclk const NTP_LEGAL_OFFSET_DEF_VAL = NTP_LEGAL_OFFSET_DEF;
T_pref_record rec_ntp_legal_offset ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_NTP_LEGAL_OFFSET,
  (char const*)BASE_NTP_LEGAL_OFFSET_CKS,
  (T_Lclk const*)NTP_LEGAL_OFFSET_DEF_VAL,
  BASE_NTP_LEGAL_OFFSET_LEN,
  &base_param.ntp_legal_offset,
  PARAM_LONG, //no string_type
  (char const*)"12 NTP Legal offset."
};
/* base12 */
int16_t const XTAL_SEL_DEF_VAL = XTAL_SEL_DEFAULT;
T_pref_record rec_xtal_sel ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_XTAL_SEL,
  (char const*)BASE_XTAL_SEL_CKS,
  (int16_t const*)XTAL_SEL_DEF_VAL,
  BASE_XTAL_SEL_LEN,
  &base_param.xtal_sel,
  PARAM_INT, 
  (char const*)"13 Xtal sel........."
};
/* base13 */
uint64_t const XTAL_BASE_DEF_VAL = XTAL_BASE_DEFAULT;
T_pref_record rec_xtal_base ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_XTAL_BASE,
  (char const*)BASE_XTAL_BASE_CKS,
  (uint64_t const*)XTAL_BASE_DEF_VAL,
  BASE_XTAL_BASE_LEN,
  &base_param.xtal_base,
  PARAM_LONG, 
  (char const*)"14 Xtal Base........"
};
/* base14 */
uint64_t const XTAL_BASE_1_DEF_VAL = XTAL_BASE_1_DEFAULT;
T_pref_record rec_xtal_base_1 ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_XTAL_BASE_1,
  (char const*)BASE_XTAL_BASE_1_CKS,
  (uint64_t const*)XTAL_BASE_1_DEF_VAL,
  BASE_XTAL_BASE_1_LEN,
  &base_param.xtal_base_1,
  PARAM_LONG, 
  (char const*)"15 Xtal Base_1......"
};
/* base15 */
uint64_t const XTAL_CHECK_SUM_DEF_VAL = XTAL_CHECK_SUM_DEAFULT;
T_pref_record rec_xtal_check_sum ={
  (char const*)BASE_SPACE_NAME,
  (char const*)BASE_XTAL_CHECK_SUM,
  (char const*)BASE_XTAL_CHECK_SUM_CKS,
  (uint64_t const*)XTAL_CHECK_SUM_DEF_VAL,
  BASE_XTAL_CHECK_SUM_LEN,
  &base_param.xtal_check_sum,
  PARAM_LONG, 
  (char const*)"16 Xtal CheckSum...."
};


/********************************************************************* */
T_pref_record* tbl_base_param_rec[] = 
{
  0,                        //00 per avere l'indice parametri che parte da 1
  &rec_dev_name,            //01
  &rec_dev_serie,           //02
  &rec_wifi_name,           //03
  &rec_wifi_psw,            //04
  &rec_blue_name_slave,     //05
  &rec_blue_name_master,    //06
  &rec_blue_pin_slave,      //07
  &rec_blue_pin_master,     //08
  &rec_blue_name_remote,    //09
  &rec_ntp_server,          //10
  &rec_gmt_offset,          //11
  &rec_ntp_legal_offset,    //12  
  &rec_xtal_sel,            //13
  &rec_xtal_base,           //14
  &rec_xtal_base_1,         //15
  &rec_xtal_check_sum,      //15
  0                         //17 per avere la fine dei parametri
};

/********************************************************************* */
/********************************************************************* */
/********************************************************************* */
/* user01 */
char const USER_SENS_TASK_ENABLE_VAL = SENS_TASK_ENABLE_VAL_DEFAULT;
T_pref_record rec_user_sens_task_enable ={
  (char const*)USER_SPACE_NAME,
  (char const*)USER_SENS_TASK_ENABLE,
  (char const*)USER_SENS_TASK_ENABLE_CKS,
  (uint64_t const*)USER_SENS_TASK_ENABLE_VAL,
  USER_SENS_TASK_ENABLE_LEN,
  &user_param.sens_task_enable,
  PARAM_BOOL, 
  (char const*)"00 Sens_Task Enable "
};
/* user01 */
char const USER_SENS_AUTOEMIT_DELAY_VAL = SENS_AUTOEMIT_DELAY_VAL_DEFAULT;
T_pref_record rec_user_sens_autoemit_delay ={
  (char const*)USER_SPACE_NAME,
  (char const*)USER_SENS_AUTOEMIT_DELAY,
  (char const*)USER_SENS_AUTOEMIT_DELAY_CKS,
  (uint64_t const*)USER_SENS_AUTOEMIT_DELAY_VAL,
  USER_SENS_AUTOEMIT_DELAY_LEN,
  &user_param.sens_autoemit_delay,
  PARAM_INT, 
  (char const*)"02 Sens Autoemit Delay "
};
/********************************************************************* */
T_pref_record* tbl_user_param_rec[] = 
{
  0,                              //00 per avere l'indice parametri che parte da 1->serve per print impo_user
  &rec_user_sens_task_enable,     //01
  &rec_user_sens_autoemit_delay,  //02
  0                               //03 per avere la fine dei parametri
};
