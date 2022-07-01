#include "CSensorTask.h"

CSensorTask::CSensorTask(CSensorController* sensorController, CQueue* measureStore)
{
  _sensorController = sensorController;
  _measureStore = measureStore;  
}
  
void CSensorTask::Setup()
{
  SetPeriod(5000);
  Start();
}
void CSensorTask::Start()
{
  _active = true;
  _lastTime = millis();
}

void CSensorTask::Stop()
{
  _active = false;
}

void CSensorTask::Exec()
{
  if (!_active)
  {
    return;
  }
  if (millis() - _lastTime >= _period)
  {
    _lastTime = millis();
    DoTask();
  }
}

void CSensorTask::SetPeriod(uint32_t period)
{
  _period = period;
}

void CSensorTask::DoTask()
{
  _measureStore->Push(_sensorController->GetMeasure());
}
