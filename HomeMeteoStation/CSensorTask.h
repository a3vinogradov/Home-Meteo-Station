#ifndef file_CSensorTask
#define file_CSensorTask

#include "CQueue.h"
#include "CSensorController.h"

class CSensorTask
{
  public:
    CSensorTask(CSensorController* sensorController, CQueue* measureStore);
    void Setup();
    void Exec();
    void Start();
    void Stop();
    void SetPeriod(uint32_t period);

    

  private:
    bool _active;
    uint32_t _period;
    uint32_t _lastTime;
    CSensorController* _sensorController;
    CQueue* _measureStore;
    void DoTask();
};

#endif
