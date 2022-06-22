#include "CWebController.h"

CWebController* CWebController::_instance = 0;
//=======================================

void HandleRoot() {
  CWebController::GetInstance()->HandlePage("/index.html");
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

void CWebController::Setup()
{
  if (TryToConnect(APSSID, APPSK))
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());    
  }
  else
  {
    Serial.println("Non Connecting to WiFi..");
    Serial.println("Start create AP");
    WiFi.softAP(LOCALSSID); // свободный доступ
    IPAddress myIP = WiFi.softAPIP();    
  }
  
  _fsController->Setup();
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
  double temperature = 23.6;
  double pressure = 874;
  double hummidity = 31;
  String apSSID = "";
  String staSSID = "";
  String staPass = "";
  int freq = 1800; 
  content.replace("<%Version%>", String(ver));  
  content.replace("<%Temperature%>", String(temperature));  
  content.replace("<%Pressure%>", String(pressure));  
  content.replace("<%Hummidity%>", String(hummidity));  
  content.replace("<%APSSID%>", String(apSSID));  
  content.replace("<%STASSID%>", String(staSSID));  
  content.replace("<%STAPassword%>", String(staPass));  
  content.replace("<%PoolingFrequency%>", String(freq));  
  
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

  message += _eeController->EEDataToString(_eeController->ReadData());
  message += "<br><a href =\"/index.html\">index</a>";
  message += "</body></html>";

  SendContent(200, "text/html", message);    // ответить на HTTP запрос
}

void CWebController::ConfigureWebServer()
{
  _webServer->on("/", HandleRoot);
  _webServer->on("/index.html", HandleRoot);
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

  WiFi.begin(APSSID, APPSK);
  
  while (--tries && WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  return WiFi.status() == WL_CONNECTED;
}
