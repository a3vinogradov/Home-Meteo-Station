#include "CWebController.h"
#include <ESP8266HTTPClient.h>

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
  setTime(11,20,0,14,7,22); // todo: брать из интернетов
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
  
  _eeCurrentNMData = _eeController->ReadNMData();
  if(_eeCurrentNMData.dataKey != EEPROM_KEY)
  {
    _eeController->WriteNMData(_eeController->GetDefaultNMData());
  }
  _eeCurrentNMData = _eeController->ReadNMData();
  
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

  content.replace("<%nmMAC%>", String(_eeCurrentNMData.MAC));
  content.replace("<%nmBMP280T%>", String(_eeCurrentNMData.BMP280T));
  content.replace("<%nmBMP280P%>", String(_eeCurrentNMData.BMP280P));
  content.replace("<%nmAHT21bT%>", String(_eeCurrentNMData.AHT21bT));
  content.replace("<%nmAHT21bH%>", String(_eeCurrentNMData.AHT21bH));
  content.replace("<%nmFrequency%>", String(_eeCurrentNMData.pollingPeriod));

  return content;
}

void CWebController::HandleAction()
{
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
  message += "====================================\n<br>\n<br>";
  
  String actionType = GetActionType();
  message += "actionType = " + actionType; 
  if (actionType == "setWiFiMode")
  {
    message += "EEPROM Data: \n<br>";
    message += _eeController->EEDataToString(_eeCurrentData);
    message += "\n<br>";
  
    EEData eeData = GetDataFromWebServerArgs();
    message += "Server Data: \n<br>";
    message += _eeController->EEDataToString(eeData);

    
    if(_eeController->WriteData(eeData))
    {
      _eeCurrentData = eeData;
    }    
  } 
  else if(actionType == "setDateTime")
  {
    //HandleActionSetDateTime();
    String webDateTime = GetStringParameter("pDateTime");
    message += "<h3>======= HandleActionSetDateTime =====================</h3>";
    message += "<p>webDateTime = " + webDateTime + "</p>"; 
    
    time_t dt = WebDateTimeToTime_t(webDateTime);
    message += "<p>time_t = "+String(dt)+"</p>"; 
    setTime(dt);
  }
  else if(actionType == "SendNMMessage")
  {
    QueueData data = _sensorController->GetMeasure();
    message += "<p>Try to send message... </p>"; 
    SendToNarodmon(data);
    message += "<p>End sending. Status - n/a </p>"; 
  }
  else if(actionType == "NarodmonSettings")
  {
    message += "<p>Handling action NarodmonSettings ... </p>"; 
    message += "EEPROM NMData: \n<br>";
    message += _eeController->EENMDataToString(_eeCurrentNMData);
    message += "\n<br>";
  
    EENMData eeNMData = GetNMDataFromWebServerArgs();
    //message += "Server Data: \n<br>";
    //message += _eeController->EEDataToString(eeData);

    
    if(_eeController->WriteNMData(eeNMData))
    {
      _eeCurrentNMData = eeNMData;
    }    
  }
  
  message += "\n<br>";
  message += "<br><a href =\"/index.html\">index</a>";
  message += "</body></html>";
  SendContent(200, "text/html", message);    // ответить на HTTP запрос
}

void CWebController::HandleRoot()
{
  _lastMeasureData = _sensorController->GetMeasure();
   HandlePage("/index.html");
}

String CWebController::GetActionType()
{
  return GetStringParameter("actionType");
}

String CWebController::GetStringParameter(String paramName)
{
  for (int i = 0; i < _webServer->args(); i++) 
  {
    if (_webServer->argName(i) == paramName)
    {
      return String(_webServer->arg(i).c_str());
    }
  }
  
  return "";
}

EEData CWebController::GetDataFromWebServerArgs()
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

EENMData CWebController::GetNMDataFromWebServerArgs()
{
  EENMData res = _eeController->GetEmptyNMData();
    
  for (int i = 0; i < _webServer->args(); i++) 
  {
    if( _webServer->argName(i) == "nmMAC")
    {
      strcpy(res.MAC, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "nmBMP280T")
    {
      strcpy(res.BMP280T, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "nmBMP280P")
    {
      strcpy(res.BMP280P, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "nmAHT21bT")
    {
      strcpy(res.AHT21bT, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "nmAHT21bH")
    {
      strcpy(res.AHT21bH, _webServer->arg(i).c_str());
    }
    else if( _webServer->argName(i) == "nmFrequency")
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
    String dateStr = String( day(item.DT))+"-"+
                     String( month(item.DT))+"-"+
                     String( year(item.DT))+" "+
                     String( hour(item.DT))+":"+
                     String( minute(item.DT))+":"+
                     String( second(item.DT));
    result += "<tr>";
    result += "<td>"+String(i+1)+"</td>";
    result += "<td>"+dateStr+"</td>";
    result += "<td>"+String(item.Temperature1)+"</td>";
    result += "<td>"+String(item.Pressure)+"</td>";
    result += "<td>"+String(item.Hummidity)+"</td>";
    result += "</tr>";
  }
  return result;
}

time_t CWebController::WebDateTimeToTime_t(String webDateTime)
{
  //2022-07-20T08:43
  tmElements_t dt;
  dt.Year = webDateTime.substring(0,4).toInt()-1970;
  dt.Month = webDateTime.substring(5,7).toInt();
  dt.Day = webDateTime.substring(8,10).toInt();
  dt.Hour = webDateTime.substring(11,13).toInt();
  dt.Minute = webDateTime.substring(14,16).toInt();
  dt.Second = 0;
  
  return makeTime(dt); 
}

void CWebController::SendToNarodmon(QueueData data)
{
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://narodmon.ru/json");
    http.addHeader("Content-Type", "application/json");
    Serial.print("[HTTP] POST...\n");
    
    String Message = String("{ \"devices\":[{\"mac\": \"14DE803A90C9\",\"sensors\":[{ \"id\": \"TemperatureAHT21\",\"value\": ") +
          String(data.Temperature1) +
          String("},{\"id\": \"TemperatureBMP280\",\"value\": ")+
          String(data.Temperature2) +
          String("},{\"id\": \"HumidityAHT21\",\"value\": ")+
          String(data.Hummidity) +
          String("},{\"id\": \"PressureBMP280\",\"value\": ")+
          String(data.Pressure) +
          String("}]}]}"); 
    int httpCode = http.POST(Message);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    
    Serial.println("===============================");
    Serial.println("String sended:");
    Serial.println(Message);
    Serial.println("===============================");
}

CEEController* CWebController::GetEEController()
{
  return _eeController;
}
