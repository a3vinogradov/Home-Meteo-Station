#include "CSensorController.h"

CSensorController::CSensorController()
{
	_AHT21Active = false;
  _BMP280Active = false;
}

void CSensorController::Setup()
{
  _AHT21Active = _AHT21.begin();
  _BMP280Active = _BMP280.begin(0x76);
}

QueueData CSensorController::GetMeasure()
{
  QueueData res;
  res.Temperature1 = 0;
  res.Temperature2 = 0;
  res.Pressure = 0;
  res.Hummidity = 0;

  if (_AHT21Active)
  {
    sensors_event_t humidity, temp;
    _AHT21.getEvent(&humidity, &temp);
    res.Temperature1 = temp.temperature;
    res.Hummidity = humidity.relative_humidity;
  }
  
  if (_BMP280Active)
  {
    sensors_event_t temp_event, pressure_event;    
    _BMP280.getTemperatureSensor()->getEvent(&temp_event);
    res.Temperature2 = temp_event.temperature;
    _BMP280.getPressureSensor()->getEvent(&pressure_event);
    res.Pressure = pressure_event.pressure;
  }

  return res;
}
