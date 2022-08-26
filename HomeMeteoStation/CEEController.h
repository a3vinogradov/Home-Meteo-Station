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

struct EENMData
{
  byte dataKey;
  char MAC[13];
  char BMP280T[11];
  char BMP280P[11];
  char AHT21bT[11];
  char AHT21bH[11];
  int pollingPeriod;
};

struct AllData
{
  EEData data1;
  EENMData data2;
};

class CEEController
{
  public:
    CEEController();
    void Setup();
    void Exec();
    EEData GetEmptyData();
    EENMData GetEmptyNMData();
    EEData GetDefaultData();
    EENMData GetDefaultNMData();
    EEData ReadData();
    EENMData ReadNMData();
    bool WriteData(EEData eeData);
    bool WriteNMData(EENMData eeNMData);
    String EEDataToString(EEData eeData); 
    String EENMDataToString(EENMData eeNMData); 
     
};

#endif
