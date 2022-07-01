#ifndef file_CQueue
#define file_CQueue

#include <Arduino.h>

#define QueueCapacity 3

struct QueueData
{
  double Temperature1;
  double Temperature2;
  double Pressure;
  double Hummidity;
};

class CQueue
{
  private:
    byte _capacity;
    byte _size;
	  QueueData _queueData[20];
    QueueData* _pQueueData;
    
  public:  
    CQueue(const int _capacity);
    byte Capacity();
    byte Size();
    bool IsEmpty();
    void Push(QueueData item);
    QueueData Pop();
    void Clear();
    QueueData ItemAt(int index);

    
};

#endif
