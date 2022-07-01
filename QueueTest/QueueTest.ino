#include "CQueue.h"
CQueue* queue;

void setup()
{
  Serial.begin(115200);
  Serial.println("Start test");
  Serial.println("Create queue...");
  queue = new CQueue(2);
  Serial.println("success!");

  Serial.println("");
  Serial.println("Size(): actual = "+String(queue->Size())+"; expected = 0");

  Serial.println("Add item {1,1,1,1}");
  QueueData item;
  item.Temperature1 = 1;
  item.Temperature2 = 1;
  item.Pressure = 1;
  item.Hummidity = 1;
  queue->Push(item);
  Serial.println("");
  Serial.println("Size(): actual = "+String(queue->Size())+"; expected = 1");
  Serial.println("Get item[0]");
  item = queue->ItemAt(0);
  Serial.println("item[0].Temperature1: actual = "+String(item.Temperature1)+"; expected = 1");
  Serial.println("item[0].Temperature2: actual = "+String(item.Temperature2)+"; expected = 1");
  Serial.println("item[0].Pressure: actual = "+String(item.Pressure)+"; expected = 1");
  Serial.println("item[0].Hummidity: actual = "+String(item.Hummidity)+"; expected = 1");

  Serial.println("Add item {2,2,2,2}");
  item.Temperature1 = 2;
  item.Temperature2 = 2;
  item.Pressure = 2;
  item.Hummidity = 2;
  queue->Push(item);
  Serial.println("");
  Serial.println("Size(): actual = "+String(queue->Size())+"; expected = 2");
  Serial.println("Get item[1]");
  item = queue->ItemAt(1);
  Serial.println("item[1].Temperature1: actual = "+String(item.Temperature1)+"; expected = 1");
  Serial.println("item[1].Temperature2: actual = "+String(item.Temperature2)+"; expected = 1");
  Serial.println("item[1].Pressure: actual = "+String(item.Pressure)+"; expected = 1");
  Serial.println("item[1].Hummidity: actual = "+String(item.Hummidity)+"; expected = 1");
  Serial.println("Get item[0]");
  item = queue->ItemAt(0);
  Serial.println("item[0].Temperature1: actual = "+String(item.Temperature1)+"; expected = 2");
  Serial.println("item[0].Temperature2: actual = "+String(item.Temperature2)+"; expected = 2");
  Serial.println("item[0].Pressure: actual = "+String(item.Pressure)+"; expected = 2");
  Serial.println("item[0].Hummidity: actual = "+String(item.Hummidity)+"; expected = 2");

  Serial.println("Add item {3,3,3,3}");
  item.Temperature1 = 3;
  item.Temperature2 = 3;
  item.Pressure = 3;
  item.Hummidity = 3;
  queue->Push(item);
  Serial.println("");
  Serial.println("Size(): actual = "+String(queue->Size())+"; expected = 2");
  Serial.println("Get item[1]");
  item = queue->ItemAt(1);
  Serial.println("item[1].Temperature1: actual = "+String(item.Temperature1)+"; expected = 2");
  Serial.println("item[1].Temperature2: actual = "+String(item.Temperature2)+"; expected = 2");
  Serial.println("item[1].Pressure: actual = "+String(item.Pressure)+"; expected = 2");
  Serial.println("item[1].Hummidity: actual = "+String(item.Hummidity)+"; expected = 2");
  Serial.println("Get item[0]");
  item = queue->ItemAt(0);
  Serial.println("item[0].Temperature1: actual = "+String(item.Temperature1)+"; expected = 3");
  Serial.println("item[0].Temperature2: actual = "+String(item.Temperature2)+"; expected = 3");
  Serial.println("item[0].Pressure: actual = "+String(item.Pressure)+"; expected = 3");
  Serial.println("item[0].Hummidity: actual = "+String(item.Hummidity)+"; expected = 3");  

  Serial.println("Pop item");
  item = queue->Pop();
  Serial.println("item.Temperature1: actual = "+String(item.Temperature1)+"; expected = 2");
  Serial.println("item.Temperature2: actual = "+String(item.Temperature2)+"; expected = 2");
  Serial.println("item.Pressure: actual = "+String(item.Pressure)+"; expected = 2");
  Serial.println("item.Hummidity: actual = "+String(item.Hummidity)+"; expected = 2");
  Serial.println("Get item[0]");   
  
  Serial.println("Size(): actual = "+String(queue->Size())+"; expected = 1");
  Serial.println("Get item[0]");
  item = queue->ItemAt(0);
  Serial.println("item[0].Temperature1: actual = "+String(item.Temperature1)+"; expected = 3");
  Serial.println("item[0].Temperature2: actual = "+String(item.Temperature2)+"; expected = 3");
  Serial.println("item[0].Pressure: actual = "+String(item.Pressure)+"; expected = 3");
  Serial.println("item[0].Hummidity: actual = "+String(item.Hummidity)+"; expected = 3");   

  Serial.println("Pop item");
  item = queue->Pop();
  Serial.println("item.Temperature1: actual = "+String(item.Temperature1)+"; expected = 3");
  Serial.println("item.Temperature2: actual = "+String(item.Temperature2)+"; expected = 3");
  Serial.println("item.Pressure: actual = "+String(item.Pressure)+"; expected = 3");
  Serial.println("item.Hummidity: actual = "+String(item.Hummidity)+"; expected = 3");
  Serial.println("Size(): actual = "+String(queue->Size())+"; expected = 0");
}

void loop()
{
}

void PrintQueueInfo(CQueue* queue)
{
  Serial.println();
}
//void TestAndPrint()
