#ifndef CWebControllerFile
#define CWebControllerFile

#include "CFSController.h"

#define APSSID "BelkoESP_AP"
#define APPSK  "liskabelko"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class CWebController {
  public:
    CWebController();
    static CWebController* GetInstance();

    void Setup();
    void Exec();
    void SendContent(int code, String contentType, String content );
    void HandlePage(String pageName);
    void HandleAction();

  private:
    static CWebController* _instance;
    CFSController* _fsController;
    ESP8266WebServer* _webServer;
};

#endif
