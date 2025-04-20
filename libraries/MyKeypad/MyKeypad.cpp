#include "Arduino.h"
#include "MyKeypad.h"

#include <Key.h>
#include <Keypad.h>

MyKeypad::MyKeypad(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8) {

  byte rows[4] = {pin8, pin7, pin6, pin5};
  byte cols[4] = {pin4, pin3, pin2, pin1};
    
  char hexaKeys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
  };

  Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rows, cols, 4, 4); 
  
}

void MyKeypad::numInput(){
  char customKey = 1;
  while(customKey!= '#'){
    customKey = customKeypad.getKey();
    if(customKey){
      Serial.println(customKey);
    }
  }
}