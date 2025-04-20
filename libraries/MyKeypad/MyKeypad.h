#ifndef MyKeypad_h
#define MyKeypad_h

#include "Arduino.h"

class MyKeypad {
  public:
    MyKeypad(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8);
    void numInput();
}

#endif