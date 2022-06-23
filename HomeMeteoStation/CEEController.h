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
  char apSSID[33];
  char apPassword[33];
  char staSSID[33];
  char staPassword[33];
  int pollingPeriod; // период опроса датчиков в секундах
};

class CEEController
{
  public:
    CEEController();
    void Setup();
    void Exec();
    EEData GetEmptyData();
    EEData GetDefaultData();
    EEData ReadData();
    bool WriteData(EEData eeData);
    String EEDataToString(EEData eeData);  
};

#endif
