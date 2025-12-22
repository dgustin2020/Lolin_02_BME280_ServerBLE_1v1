/*
proto_common.h
header locale per getione proto
*/
#ifndef PROTO_COMMON_H
#define PROTO_COMMON_H


#define COMMAND_OK        '1'
#define COMMAND_ERR       '0'

/*variabili*/
extern char   proto_cmd;
extern char*  proto_from;
extern char*  proto_to;
extern char   proto_str[200];//buffer dove generare le stringhe in formato proto
extern char   proto_tx[200];//buffer dove generare le stringhe da inviare come Informazione
//extern char   loc_char_buff[50];

/* procedure da usare all'interno delle procedure x comandi*/

extern int   buff_char_to_long(char* p_from, long* p_result);

extern char* genera_device_info_short(char cmd, char type, char* p_to);
extern char* genera_device_info_task(char cmd, char type, char* p_to);
extern char* genera_wifi_info(char cmd, char type, char* p_to);
//extern char* value_sensor_to_char(char* txt, float value, char* p_to);

/*********************************************************** */


#endif