#ifndef CWebControllerFile
#define CWebControllerFile

#include "CFSController.h"
#include "CEEController.h"
#include "CSensorController.h"
#include "Time.h"
#include "TimeLib.h"
#include "CSensorTask.h"

#define LOCALSSID "HomeStationAuto"
#define APSSID "BelkoNet"
#define APPSK  "liskabelko"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class CWebController {
  public:
    CWebController();
    static CWebController* GetInstance();

    void Setup(CSensorController* sensorController, CQueue* measureStore, CSensorTask* sensorTask);
    void Exec();
    void SendContent(int code, String contentType, String content );
    void HandlePage(String pageName);
    void HandleAction();
    void HandleRoot(); 
    void Reset();
    //void HandleAbout();
    String FormatPage(String content, String pageName);
    void SendToNarodmon(QueueData data);
    CEEController* GetEEController();

  private:
    static CWebController* _instance;
    CFSController* _fsController;
    CEEController* _eeController;
    ESP8266WebServer* _webServer;
    EEData _eeCurrentData;
    EENMData _eeCurrentNMData;
    QueueData _lastMeasureData;
    CSensorController* _sensorController;
    CQueue* _measureStore;
    CSensorTask* _sensorTask;

    void ConfigureWebServer();
    bool TryToConnect(String ssid, String pass);
    EEData GetDataFromWebServerArgs();
    EENMData GetNMDataFromWebServerArgs();
    String GetTableHistoryHTML();
    String GetActionType();
    String GetStringParameter(String paramName);
    time_t WebDateTimeToTime_t(String webDateTime); 
  
};

#endif
