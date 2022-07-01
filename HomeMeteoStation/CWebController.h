#ifndef CWebControllerFile
#define CWebControllerFile

#include "CFSController.h"
#include "CEEController.h"
#include "CSensorController.h"

#define LOCALSSID "HomeStationAuto"
#define APSSID "BelkoNet"
#define APPSK  "liskabelko"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class CWebController {
  public:
    CWebController();
    static CWebController* GetInstance();

    void Setup(CSensorController* sensorController, CQueue* measureStore);
    void Exec();
    void SendContent(int code, String contentType, String content );
    void HandlePage(String pageName);
    void HandleAction();
    void HandleRoot();
    void Reset();
    //void HandleAbout();
    String FormatPage(String content, String pageName);
    


  private:
    static CWebController* _instance;
    CFSController* _fsController;
    CEEController* _eeController;
    ESP8266WebServer* _webServer;
    EEData _eeCurrentData;
    QueueData _lastMeasureData;
    CSensorController* _sensorController;
    CQueue* _measureStore;

    void ConfigureWebServer();
    bool TryToConnect(String ssid, String pass);
    EEData GetDateFromWebServerArgs();
    String GetTableHistoryHTML();
};

#endif
