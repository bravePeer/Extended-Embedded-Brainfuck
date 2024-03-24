#pragma once
#include "Arduino.h"
#define USE_STATIC_STACK
#define SIMPLE_STACK_SIZE 50

template <class T>
class Stack
{
public:
  #if defined(USE_STATIC_STACK)
  Stack()
    :stackSize(SIMPLE_STACK_SIZE)
  #else
  Stack(T stackSize = SIMPLE_STACK_SIZE)
    :stackSize(stackSize)
  #endif
  {
    stackMemory = new T[stackSize]{0};
    //if(stackMemory == nullptr) // ERROR

    stackIndex = 0;
  }
  ~Stack()
  {
    delete[] stackMemory;
  }

  void push(T data)
  {
    if(stackIndex >= stackSize) // ERROR
      return;
    stackMemory[++stackIndex] = data;
  }
  
  T top()
  {
    return stackMemory[stackIndex];
  }

  void pop()
  {
    if(stackIndex == 0)
      return;
    stackIndex--;
  }
private:
  T stackSize;
  T stackIndex;
  T* stackMemory;
};