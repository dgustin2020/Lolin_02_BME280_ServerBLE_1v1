/*
proto_seriale.h

protocollo per collegamento seriale, sia cavo che wireless
Sringa composta:
PREAMBOLO   inizio stringa
sepa        separatore
COMANDO     comando
sepa        separatore
TYPE        tipo di comando: Richiesta, Conferma, INdicazione, Responso
sepa        separatore
DATI        campi dati separati da un separatore
*/
#ifndef PROTO_H
#define PROTO_H

#define P_CMD_MIN 'a'
#define P_CMD_MAX 'z'                        

#define COM_PORT_USB        0
#define COM_PORT_BLUE       1

#define P_PREAMBOLO         '$'
#define P_SEPARA            ','
#define P_SEPARA_FILE       TAB   //';'
#define P_REQUEST_TYPE      'R'
#define P_REQUEST_TYPE_min  'r'
#define P_CONFIRM_TYPE      'C'
#define P_INDICATION_TYPE   'I'
#define P_RESPONSE_TYPE     'r'
#define P_END_STRING        CR
#define P_END_STRING_CR_LF  "\r\n"
#define BLUE_SEPARA         ','
#define P_INFO_AUTO         'z'
#define P_WRITE_COMMAND     'w'
#define P_READ_COMMAND      'r'

#define RENOTE_FLAG_NONE      0
#define REMOTE_FLAG_REQ       1
#define REMOTE_FLAG_CFM       2
#define REMOTE_FLAG_IND       3

#define PROTO_NO_ERR          0
#define PROTO_ERR_PREAMBLE    1
#define PROTO_ERR_CMD         2
#define PROTO_ERR_TYPE_REQ    3
#define PROTO_ERR_SUB_CMD     4
#define PROTO_ERR_SUB_SUB_CMD 5
#define PROTO_ERR_HARDWARE    6

/* procedure */
extern bool   proto_gest(char* p_from, char com_port);
extern void   proto_tx_info(char cmd, char* txt, char com_port);
extern char*  proto_set_end_of_string(char* p_to);
extern char*  proto_metti_cmd(char cmd, char* p_to);
extern char*  proto_string_cmd_cfm_ok(char cmd, char* p_to);
#endif //PRORO_H
