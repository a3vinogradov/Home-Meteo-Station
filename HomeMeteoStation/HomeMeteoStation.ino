// Конфигурирование вайфай молуля в режиме точки доступа
// Можно поменять ssid сети и пароль, но длина пароля не может быть меньше 8 символов
// Можно настроить свободный доступ без пароля
//
// Не удалось настроить ip точки доступа
// По умолчанию она занимает  http://192.168.4.1
//
// При запуске создается точка доступа с заданным именем и паролем
// В браузере открывается страница http://192.168.4.1

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#ifndef APSSID
#define APSSID "BelkoESP_AP"
#define APPSK  "liskabelko"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  handlePage("/index.html");
}

void handlePage(String pageName){
  File file = SPIFFS.open(pageName, "r");
  if(!file){
    server.send(404, "text/plain", "File not found");
    return;
  }  

  String res = "";
  while(file.available())
  {
    res = res + char(file.read());
  }
  file.close();
  server.send(200, "text/html", res);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void handleAbout() {
  handlePage("/about.html");
}

void handleGenericArgs() //обработчик
{
  String message = "Number of args received:";
  message += server.args();      // получить количество параметров
  message += "\n";               // переход на новую строку

  for (int i = 0; i < server.args(); i++) 
  {
    message += "Arg nº" + (String)i + " –> "; // добавить текущее значение счетчика
    message += server.argName(i) + ": ";      // получить имя параметра
    message += server.arg(i) + "\n";          // получить значение параметра
  } 

  server.send(200, "text/plain", message);    // ответить на HTTP запрос
}

void handleSettings() {
  handlePage("/settings.html");
}
void handleHistory() {
  handlePage("/history.html");
}
void handleStyle() {
  handlePage("/style.css");
}
void setup() {
  Serial.begin(115200);
  
  delay(1000);
  WiFi.softAP(ssid); // свободный доступ
  delay(100);

  IPAddress myIP = WiFi.softAPIP();

   if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", handleRoot);
  server.on("/index.html", handleRoot);
  server.on("/settings.html", handleSettings);
  server.on("/action.html", handleGenericArgs);
  server.on("/about.html", handleAbout);
  server.on("/history.html", handleHistory);
  server.on("/style.css", handleStyle);
  
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}
