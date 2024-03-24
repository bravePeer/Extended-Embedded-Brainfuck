#include <Arduino.h>
#include "eebrainfuck_interpreter.h"
#include "eebrainfuck_code.h"

void setup() 
{
  DDRB;  // ((0x04) + 0x20)) = 0x24 => 36 = 6 * 6
  PORTB; // ((0x05) + 0x20)) = 0x25 => 37 = 6 * 6 + 1
  Serial.begin(9600);
  BrainfuckInterpreter<> bf;
  bf.execute(eebf, eebfSize);
}

void loop() 
{

}
