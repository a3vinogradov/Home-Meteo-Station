#include "CEEController.h"

CEEController::CEEController()
{
  
}

void CEEController::Setup()
{
  
}

void CEEController::Exec()
{
  
}

EEData CEEController::GetEmptyData()
{
  EEData res;
  res.dataKey = EEPROM_KEY;
  res.wifiMode = EEPROM_WIFI_AP;
  strcpy(res.apSSID, "");   
  strcpy(res.apPassword, "");   
  strcpy(res.staSSID, "");   
  strcpy(res.staPassword, ""); 
  res.pollingPeriod = 0;

  return res;
}
EEData CEEController::GetDefaultData()
{
  EEData res;
  res.dataKey = EEPROM_KEY;
  res.wifiMode = EEPROM_WIFI_AP;
  strcpy(res.apSSID, "MeteoStationAuto");   
  strcpy(res.apPassword, "");   
  strcpy(res.staSSID, "BelkoNet");   
  strcpy(res.staPassword, "liskabelko"); 
  res.pollingPeriod = 1800;
    
  return res;
}    

EEData CEEController::ReadData()
{
  EEPROM.begin(512);
  EEData res;
  EEPROM.get(0,res);
  Serial.println("");
  Serial.println("Read EEPROM");
  Serial.println(EEDataToString(res));

  if (res.dataKey != EEPROM_KEY)
  {
    res = GetDefaultData();
  }
  
  return res;
}

bool CEEController::WriteData(EEData eeData)
{
  EEPROM.begin(512);
  EEPROM.put(0,eeData);
  EEPROM.commit();
  return true;
}

String CEEController::EEDataToString(EEData eeData)
{
  String res = "struct EEData:\n<br>";
  res = res + "dataKey = " + String(eeData.dataKey) + ";\n<br>"; 
  res = res + "wifiMode = " + String(eeData.wifiMode) + ";\n<br>"; 
  res = res + "apSSID = " + String(eeData.apSSID) + ";\n<br>"; 
  res = res + "apPassword = " + String(eeData.apPassword) + ";\n<br>"; 
  res = res + "staSSID = " + String(eeData.staSSID) + ";\n<br>"; 
  res = res + "staPassword = " + String(eeData.staPassword) + ";\n<br>"; 
  res = res + "pooling period = " + String(eeData.pollingPeriod) + "сек ;\n<br>"; 
  
  return res;
}
