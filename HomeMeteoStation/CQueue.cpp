#include "CQueue.h"

CQueue::CQueue(const int capacity)
{
  _capacity = capacity;
  _size = 0;
  _pQueueData = _queueData;
}

byte CQueue::Capacity()
{
  return _capacity;
}

byte CQueue::Size()
{
  return _size;
}

bool CQueue::IsEmpty()
{
  return Size()==0;
}

void CQueue::Push(QueueData item)
{
  if (Size() < Capacity())
  {
    _size++;
  }
 
  for(int i=Size()-1; i > 0; i--)
  {
    _pQueueData[i] = _pQueueData[i-1];
  }
  _pQueueData[0] = item;
}

QueueData CQueue::Pop()
{
  QueueData item;
  if (Size()==0)
  {
    return item;
  }
  
  item = _pQueueData[Size()-1];
  _size--;
  
  return item;
}

void CQueue::Clear()
{
  _size = 0;
}

QueueData CQueue::ItemAt(int index)
{
  if (index >= 0 && index < Size())
  {
    return _pQueueData[index];
  }

  QueueData item;
  return item;  
}
