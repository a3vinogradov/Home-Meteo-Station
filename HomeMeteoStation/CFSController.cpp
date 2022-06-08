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
  File file = SPIFFS.open(pageName, "r");
  if(!file){
    Serial.println("Can not open file");
    return "";
  }  

  String res = "";
  int cnt=0;
  while(file.available())
  {
    cnt++;
    res = res + char(file.read());
  }

  file.close();  
  
  return res;
}
