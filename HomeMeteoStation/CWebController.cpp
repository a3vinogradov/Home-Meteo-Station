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

void HandleAbout() {
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
  _fsController->Setup();
  
  WiFi.softAP(APSSID); // свободный доступ
  delay(100);
  IPAddress myIP = WiFi.softAPIP();

  _webServer->on("/", HandleRoot);
  _webServer->on("/index.html", HandleRoot);
  _webServer->on("/settings.html", HandleSettings);
  _webServer->on("/action.html", GHandleAction);
  _webServer->on("/about.html", HandleAbout);
  _webServer->on("/history.html", HandleHistory);
  _webServer->on("/style.css", HandleStyle);
  _webServer->onNotFound(Handle_NotFound);
  _webServer->begin();
}

void CWebController::Exec()
{
  _webServer->handleClient();  
}

void CWebController::SendContent(int code, String contentType, String content )
{
  _webServer->send(code,contentType, content);
}

void CWebController::HandlePage(String pageName){
  String res = _fsController->ReadFile(pageName);
  Serial.println("Handle Page: ");
  Serial.println(res);


  if (res == "")
  {
    _webServer->send(404, "text/plain", "Error reading file!");
  }
  else
  {  
    _webServer->send(200, "text/html", res);
  }
}

void CWebController::HandleAction(){
  String message = "Number of args received:";
  message += _webServer->args();      // получить количество параметров
  message += "\n";               // переход на новую строку

  for (int i = 0; i < _webServer->args(); i++) 
  {
    message += "Arg nº" + (String)i + " –> "; // добавить текущее значение счетчика
    message += _webServer->argName(i) + ": ";      // получить имя параметра
    message += _webServer->arg(i) + "\n";          // получить значение параметра
  } 

  SendContent(200, "text/plain", message);    // ответить на HTTP запрос
}
