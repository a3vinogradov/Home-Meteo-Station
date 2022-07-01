#ifndef file_CSensorController
#define file_CSensorController

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include "CQueue.h"

class CSensorController
{
  private:
    bool _AHT21Active;
    bool _BMP280Active;
    Adafruit_AHTX0 _AHT21;
    Adafruit_BMP280 _BMP280;
     
  public:  
    CSensorController();
    void Setup();
    QueueData GetMeasure();
};
#endif
