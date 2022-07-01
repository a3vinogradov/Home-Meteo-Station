#include "CQueue.h"
#include "CSensorController.h"

CSensorController gSensorController;

void setup()
{
  Serial.begin(9600);
  gSensorController.Setup();

}

void loop()
{

  QueueData measures = gSensorController.GetMeasure();
  Serial.println("");
  Serial.println("Read measures");
  Serial.println("Temperature1    = "+String(measures.Temperature1));
  Serial.println("Temperature2    = "+String(measures.Temperature2));
  Serial.println("Pressure        = "+String(measures.Pressure));
  Serial.println("Hummidityature1 = "+String(measures.Hummidity));
  
  delay(4000);
}
