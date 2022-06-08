#include "CFSController.h"

CFSController::CFSController()
{
  
}

void CFSController::Setup()
{
  Serial.println("Init FS...");
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
}

void CFSController::Exec()
{
  
}

String CFSController::ReadFile(String pageName)
{
  Serial.println("Create file object... "+pageName);
  File file = SPIFFS.open(pageName, "r");
  Serial.println("Cr1");
  if(!file){
    Serial.println("Can not open file");
    return "";
  }  
  Serial.println("Cr2");
  Serial.println("Reading file "+String(file.name())+" ... ");

  String res = "";
  int cnt=0;
  while(file.available())
  {
    cnt++;
    res = res + char(file.read());
  }
  Serial.println(String(cnt)+" has been read.");
  Serial.println(res);

  file.close();  
  
  return res;
}
