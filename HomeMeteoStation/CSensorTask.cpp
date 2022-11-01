#include "CSensorTask.h"
#include "CWebController.h"

CSensorTask::CSensorTask(CSensorController* sensorController, CQueue* measureStore, CEEController* eeController)
{
  _sensorController = sensorController;
  _measureStore = measureStore;  
  _eeController = eeController;
}
  
void CSensorTask::Setup()
{
  EENMData nmData = _eeController->ReadNMData();  
  uint32_t newPeriod = (uint32_t)(nmData.pollingPeriod<MinPoolingPeriod?MinPoolingPeriod:nmData.pollingPeriod);
  SetPeriodMin(newPeriod);

  //SetPeriodMS(newPeriod*1000);

  _active = nmData.isActive;
  _lastTime = millis();
}

void CSensorTask::Start()
{
  Setup();
}

void CSensorTask::Restart()
{
  Serial.println("SensorTask.Restart started...");
  Stop();
  Start();
  Serial.println("SensorTask.Restart complete...");
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

void CSensorTask::SetPeriodMin(uint32_t period)
{
  _period = period*60*1000;
}

void CSensorTask::SetPeriodMS(uint32_t periodMS)
{
  _period = periodMS;
}
    
void CSensorTask::DoTask()
{
  QueueData nmData = _sensorController->GetMeasure();
  _measureStore->Push(nmData);
  SendNMMessage(nmData);
}

void CSensorTask::SendNMMessage(QueueData nmData)
{
  CWebController* webController = CWebController::GetInstance();
  webController->SendToNarodmon(nmData);
}
