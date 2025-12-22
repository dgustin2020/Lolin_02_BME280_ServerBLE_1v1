/*
my_WiFi.h
*/
#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#if 0
extern const char* ssid;
extern const char* password;
#endif
extern IPAddress   local_IP;// Set your Static IP address
extern IPAddress   gateway;// Set your Gateway IP address
extern IPAddress   subnet;
extern IPAddress   primaryDNS; //optional
extern IPAddress   secondaryDNS; //optional

extern char* ntpServer;
extern long  gmtOffset_sec;
extern int   daylightOffset_sec;

#endif


