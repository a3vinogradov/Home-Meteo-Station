#ifndef CWebControllerFile
#define CWebControllerFile

#include "CFSController.h"
#include "CEEController.h"

#define LOCALSSID "HomeStationAuto"
#define APSSID "BelkoNet"
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
    void Reset();
    //void HandleAbout();
    String FormatPage(String content, String pageName);


  private:
    static CWebController* _instance;
    CFSController* _fsController;
    CEEController* _eeController;
    ESP8266WebServer* _webServer;

    void ConfigureWebServer();
    bool TryToConnect(String ssid, String pass);
};

#endif
