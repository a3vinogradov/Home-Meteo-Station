#ifndef file_CSensorTask
#define file_CSensorTask

// минимальный период опроса датчиков в минутах
#define MinPoolingPeriod 5 

#include "CQueue.h"
#include "CSensorController.h"
#include "CEEController.h"

class CSensorTask
{
  public:
    CSensorTask(CSensorController* sensorController, CQueue* measureStore, CEEController* eeController);
    void Setup();
    void Exec();
    void Start();
    void Restart();
    void Stop();
    void SetPeriodMin(uint32_t period);
    void SetPeriodMS(uint32_t periodMS);
    void SendNMMessage(QueueData nmData);

  private:
    bool _active;
    uint32_t _period;
    uint32_t _lastTime;
    CSensorController* _sensorController;
    CQueue* _measureStore;
    CEEController* _eeController;
    void DoTask();
};

#endif
