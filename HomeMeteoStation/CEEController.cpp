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

EEData CEEController::GetEmtyData()
{
  EEData res;
  res.dataKey = EEPROM_KEY;
  res.wifiMode = EEPROM_WIFI_AP;
  strcpy(res.ssid, "");   
  strcpy(res.ssPassword, "");   
  strcpy(res.ssidAP, "");   
  strcpy(res.ssPasswordAP, ""); 
    
  return res;
}
EEData CEEController::GetDefaultData()
{
  EEData res;
  res.dataKey = EEPROM_KEY;
  res.wifiMode = EEPROM_WIFI_AP;
  strcpy(res.ssid, "");   
  strcpy(res.ssPassword, "");   
  strcpy(res.ssidAP, "HomeStationAuto");   
  strcpy(res.ssPasswordAP, ""); 
    
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
  String res = "struct EEData:\n";
  res = res + "dataKey = " + String(eeData.dataKey) + "\n"; 
  res = res + "wifiMode = " + String(eeData.wifiMode) + "\n"; 
  res = res + "dataKey = " + String(eeData.ssid) + "\n"; 
  res = res + "dataKey = " + String(eeData.ssPassword) + "\n"; 
  res = res + "dataKey = " + String(eeData.ssidAP) + "\n"; 
  res = res + "dataKey = " + String(eeData.ssPasswordAP) + "\n"; 
  
  return res;
}
