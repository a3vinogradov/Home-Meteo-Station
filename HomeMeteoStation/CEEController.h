#ifndef CEEControllerFile
#define CEEControllerFile

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_KEY 3
#define EEPROM_WIFI_AP 0
#define EEPROM_WIFI_STA 1

struct EEData
{
  byte dataKey;
  byte wifiMode;
  char ssid[33];
  char ssPassword[33];
  char ssidAP[33];
  char ssPasswordAP[33];
};

class CEEController
{
  public:
    CEEController();
    void Setup();
    void Exec();
    EEData GetEmtyData();
    EEData GetDefaultData();
    EEData ReadData();
    bool WriteData(EEData eeData);
    String EEDataToString(EEData eeData);  
};

#endif
