#include "CWebController.h"

CWebController* CWebController::_instance = 0;
//=======================================

void GHandleRoot() {
  CWebController::GetInstance()->HandleRoot();
}

void Handle_NotFound() {
  CWebController::GetInstance()->SendContent(404, "text/plain", "Not found");
}

void HandleSettings() {
  CWebController::GetInstance()->HandlePage("/settings.html");
}

void HandleHistory() {
  CWebController::GetInstance()->HandlePage("/history.html");
}

void HandleStyle() {
  CWebController::GetInstance()->HandlePage("/style.css");
}

void GHandleAbout() {
  CWebController::GetInstance()->HandlePage("/about.html");
}

void GHandleAction() 
{
  CWebController::GetInstance()->HandleAction();
}


//=======================================

CWebController::CWebController()
{
  _fsController = new CFSController;
  _eeController = new CEEController;
  _webServer = new ESP8266WebServer(80);
}

CWebController* CWebController::GetInstance()
{
  if(!_instance)
  {
    _instance = new CWebController();
  }
  
  return _instance;
}

void CWebController::Setup(CSensorController* sensorController, CQueue* measureStore)
{
  _sensorController = sensorController;
  _measureStore = measureStore;
    
  _eeCurrentData = _eeController->ReadData();
  if(_eeCurrentData.dataKey != EEPROM_KEY)
  {
    Reset();
  }
  
  _eeCurrentData = _eeController->ReadData();
  if (_eeCurrentData.wifiMode == EEPROM_WIFI_AP)
  {
    Serial.println("Start create AP");
    WiFi.softAP(_eeCurrentData.apSSID); // свободный доступ
    IPAddress myIP = WiFi.softAPIP();    
    Serial.print("IP: ");
    Serial.println(myIP);
  } 
  else 
  {
    TryToConnect(_eeCurrentData.staSSID, _eeCurrentData.staPassword);
    Serial.println("");
    //Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   
    WiFi.hostname("MeteoStation"); 
  }

  _fsController->Setup();
  _eeController->Setup();
  
  delay(100);

  ConfigureWebServer();
}

void CWebController::Exec()
{
  _webServer->handleClient();  
}

void CWebController::SendContent(int code, String contentType, String content )
{
  _webServer->send(code,contentType, content);
}

void CWebController::HandlePage(String pageName)
{
  String res = _fsController->ReadFile(pageName);
  if (res == "")
  {
    _webServer->send(404, "text/plain", "Error reading file!");
  }
  else
  {  
    res = FormatPage(res, pageName);
    _webServer->send(200, "text/html", res);
  }
}

void CWebController::Reset()
{
  _eeController->WriteData( _eeController->GetDefaultData());
}

String CWebController::FormatPage(String content, String pageName)
{
  String ver = "0.1-dev";  
  double temperature = _lastMeasureData.Temperature1;
  double pressure = _lastMeasureData.Pressure;
  double hummidity = _lastMeasureData.Hummidity;
  String apSSID = _eeCurrentData.apSSID;
  String staSSID = _eeCurrentData.staSSID;
  String staPass = _eeCurrentData.staPassword;
  int freq = _eeCurrentData.pollingPeriod; 
  content.replace("<%Version%>", String(ver));  
  content.replace("<%Temperature%>", String(temperature));  
  content.replace("<%Pressure%>", String(pressure));  
  content.replace("<%Hummidity%>", String(hummidity));
  
  if (_eeCurrentData.wifiMode == EEPROM_WIFI_STA)
  {
    content.replace("<%ModeWiFiAP%>", "");
    content.replace("<%ModeWiFiSTA%>", "checked" ); 
  } 
  else 
  {
    content.replace("<%ModeWiFiAP%>", "checked");
    content.replace("<%ModeWiFiSTA%>", "" ); 
  }
  
  content.replace("<%APSSID%>", String(apSSID));  
  content.replace("<%STASSID%>", String(staSSID));  
  content.replace("<%STAPassword%>", String(staPass));  
  content.replace("<%PoolingFrequency%>", String(freq));  
  content.replace("<%TableHistory%>", GetTableHistoryHTML());
  return content;
}

void CWebController::HandleAction()
{
  EEData eeData = GetDateFromWebServerArgs();
 
  String message = "<html><head><meta charset=\"UTF-8\"/></head><body> ";
  message += "Number of args received:<br>";
  message += _webServer->args();      // получить количество параметров
  message += "\n<br>";               // переход на новую строку

  for (int i = 0; i < _webServer->args(); i++) 
  {
    message += "Arg nº" + (String)i + " –> ";      // добавить текущее значение счетчика
    message += _webServer->argName(i) + ": ";      // получить имя параметра
    message += _webServer->arg(i) + "\n<br>";      // получить значение параметра
  } 
  message += "EEPROM Data: \n<br>";
  message += _eeController->EEDataToString(_eeCurrentData);
  message += "\n<br>";

  message += "Server Data: \n<br>";
  message += _eeController->EEDataToString(eeData);
  message += "\n<br>";
  
  message += "<br><a href =\"/index.html\">index</a>";
  message += "</body></html>";


  SendContent(200, "text/html", message);    // ответить на HTTP запрос
  
  if(_eeController->WriteData(eeData))
  {
    _eeCurrentData = eeData;
  }
}

void CWebController::HandleRoot()
{
  _lastMeasureData = _sensorController->GetMeasure();
   HandlePage("/index.html");
}

EEData CWebController::GetDateFromWebServerArgs()
{
  EEData res = _eeController->GetEmptyData();
    
  for (int i = 0; i < _webServer->args(); i++) 
  {
    if (_webServer->argName(i) == "wifimode")
    {
      if (_webServer->arg(i) == "STA")
      {
        res.wifiMode = EEPROM_WIFI_STA;
      }
      else
      {
        res.wifiMode = EEPROM_WIFI_AP;
      }
    }
    else if( _webServer->argName(i) == "ssidap")
    {
      strcpy(res.apSSID, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "ssidsta")
    {
      strcpy(res.staSSID, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "passwordsta")
    {
      strcpy(res.staPassword, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "polingfrequency")
    {
      res.pollingPeriod = _webServer->arg(i).toInt();
    }
  }
  return res;
}

void CWebController::ConfigureWebServer()
{
  _webServer->on("/", GHandleRoot);
  _webServer->on("/index.html", GHandleRoot);
  _webServer->on("/settings.html", HandleSettings);
  _webServer->on("/action.html", GHandleAction);
  _webServer->on("/about.html", GHandleAbout);
  _webServer->on("/history.html", HandleHistory);
  _webServer->on("/style.css", HandleStyle);
  _webServer->onNotFound(Handle_NotFound);
  _webServer->begin();  
}

bool CWebController::TryToConnect(String ssid, String pass)
{
  byte tries = 20;

  WiFi.begin(ssid, pass);
  
  while (--tries && WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  return WiFi.status() == WL_CONNECTED;
}

String CWebController::GetTableHistoryHTML()
{
  String result = "";
  for( int i = 0; i < _measureStore->Size(); i++)
  {
    QueueData item = _measureStore->ItemAt(i);
    result += "<tr>";
    result += "<td>"+String(i+1)+"</td>";
    result += "<td>"+String("test test date")+"</td>";
    result += "<td>"+String(item.Temperature1)+"</td>";
    result += "<td>"+String(item.Pressure)+"</td>";
    result += "<td>"+String(item.Hummidity)+"</td>";
    result += "</tr>";
  }
  return result;
}
