// Конфигурирование вайфай молуля в режиме точки доступа
// Можно поменять ssid сети и пароль, но длина пароля не может быть меньше 8 символов
// Можно настроить свободный доступ без пароля
//
// Не удалось настроить ip точки доступа
// По умолчанию она занимает  http://192.168.4.1
//
// При запуске создается точка доступа с заданным именем и паролем
// В браузере открывается страница http://192.168.4.1

#include "CWebController.h"
#include "CQueue.h"
#include "CSensorController.h"
#include "CSensorTask.h"

CWebController* webController = CWebController::GetInstance();
CQueue gMeasureStore = CQueue(10);
CSensorController gSensorController;
CSensorTask gSensorTask(&gSensorController, &gMeasureStore);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("");
  Serial.println("");
  Serial.println("Init Start setup");

  gSensorController.Setup();

  // проверка нажатия кнопки ресет
  if (digitalRead(16) == HIGH)
  {
    delay(40);
    if (digitalRead(16) == HIGH)
    {
      webController->Reset();
      Serial.println("Reset configuration");
    }
  }
  
  webController->Setup(&gSensorController, &gMeasureStore);
  Serial.println("End Start setup");

  //gMeasureStore.Push(gSensorController.GetMeasure());
  gSensorTask.Setup();
}

void loop() {
  webController->Exec();
  gSensorTask.Exec();
}
