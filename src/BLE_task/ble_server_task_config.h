/*
ble_server_task_config.h
*/
#ifndef _BLE_SERVER_TASK_CONFIG_H
#define _BLE_SERVER_TASK_CONFIG_h
// https://www.bluetooth.com/specifications/assigned-numbers/
#include "..\version\version.h"

#define bleServerName NOME_PROGRAMMA_BLE_SERVER

//BLE server name
// Default UUID for Environmental Sensing Service
#define SERVICE_UUID (BLEUUID((uint16_t)0x181A)) //indicatore sensori ambientali
// Temperature Characteristic and Descriptor (default UUID)
BLECharacteristic temperatureCharacteristic(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor temperatureDescriptor(BLEUUID((uint16_t)0x2902)); //Client Characteristic Configuration
// Humidity Characteristic and Descriptor (default UUID)
BLECharacteristic humidityCharacteristic(BLEUUID((uint16_t)0x2A6F), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor humidityDescriptor(BLEUUID((uint16_t)0x2902)); //Client Characteristic Configuration
// Pressure Characteristic and Descriptor (default UUID)
BLECharacteristic pressureCharacteristic(BLEUUID((uint16_t)0x2A6D), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor pressureDescriptor(BLEUUID((uint16_t)0x2902)); //Client Characteristic Configuration
// Altitudine Characteristic and Descriptor (default UUID)
BLECharacteristic elevationCharacteristic(BLEUUID((uint16_t)0x2A6C), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor elevationDescriptor(BLEUUID((uint16_t)0x2902)); //Client Characteristic Configuration


extern bool     deviceConnected;
//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks 
{
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    pServer->getAdvertising()->start(); // Riavvia l'advertising
  }
};

#endif // _BLE_TASK_CONFIG_h