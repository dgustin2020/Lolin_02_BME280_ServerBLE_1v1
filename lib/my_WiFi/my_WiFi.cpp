/*
myWiFi.cpp
*/
#include <IPAddress.h>

#if 0 //impostazione fatte in wifi_gest.cpp con i dati recuperati da my_defualt.h
/* impostazioni rete locale: nome e password*/
const char* ssid = "FASTWEB-xva7kA";
const char* password = "J56fEbe36E"; //"REPLACE_WITH_YOUR_PASSWORD";
#endif

/*impostazione IP della scheda*/
IPAddress   local_IP(192, 168, 1, 80);// Set your Static IP address
IPAddress   gateway(192, 168, 1, 254);// Set your Gateway IP address
IPAddress   subnet(255, 255, 255, 0);
IPAddress   primaryDNS(8, 8, 8, 8); //optional
IPAddress   secondaryDNS(8, 8, 4, 4); //optional

/*impostazioni NTP server per lettura ora*/
#define     NTPSERVER         "pool.ntp.org"
#define     GMT_OFFSET        3600   // fuso orario in sec
#define     LEGAL_OFFSET      3600   // ora legale in sec
char*       ntpServer = (char*)&NTPSERVER;
long        gmtOffset_sec = GMT_OFFSET;
int         daylightOffset_sec = LEGAL_OFFSET;

