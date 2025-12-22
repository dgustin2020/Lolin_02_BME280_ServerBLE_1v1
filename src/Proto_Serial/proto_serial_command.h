/*
proto_serial_command.h
*/
#ifndef PROTO_SERIAL_COMMAND_H
#define PROTO_SERIAL_COMMAND_H

/* gestione comandi ricevuti */
extern char cmd_a(char com);
extern char cmd_b(char com);
extern char proto_invia_cmd_b(char com);//invio informativo, a tempo se impostato delay
//extern char cmd_c(char com);
extern char cmd_i(char com);
extern char cmd_p(char com);
extern char cmd_s(char com);
extern char cmd_u(char com);
extern char cmd_w(char com);
#endif

