# Extended Embedded Brainfuck

This variant of Brainfuck add one separated memory block called **storage** and 15 commands.

## Commands List
Basic commands:
```
> -> increases memory pointer, or moves the pointer to the right 1 block
< -> decreases memory pointer, or moves the pointer to the left 1 block
+ -> increases value stored at the block pointed to by the memory pointer
- -> decreases value stored at the block pointed to by the memory pointer
[ -> like c while(cur_block_value != 0) loop
] -> if block currently pointed to's value is not zero, jump back to [
, -> input 1 character
. -> print 1 character to the console
```
Extended commands:
```
~ -> bitwise NOT of value stored at the block pointed to by the memory pointer
^ -> bitwise XOR of value stored at the block pointed to by the memory pointer and value stored in storage
| -> bitwise OR of value stored at the block pointed to by the memory pointer and value stored in storage
& -> bitwise AND of value stored at the block pointed to by the memory pointer and value stored in storage
_ -> substract of value stored at the block pointed to by the memory pointer and value stored in storage
= -> add of value stored at the block pointed to by the memory pointer and value stored in storage
* -> mul of value stored at the block pointed to by the memory pointer and value stored in storage
/ -> div of value stored at the block pointed to by the memory pointer and value stored in storage
% -> mod of value stored at the block pointed to by the memory pointer and value stored in storage
! -> overwrite value stored in storage with value stored at the block pointed to by the memory pointer
? -> overwrite value stored at the block pointed to by the memory pointer with value stored in storage
: -> interpret value in storage as physical address and set value at physical address to value stored at the block pointed to by the memory pointer
; -> interpret value in storage as physical address and set value stored at the block pointed to by the memory pointer to value at physical address
$ -> goto, value stored in storage is instruction index where to jump
# -> print memory dump, debug usage
```

## Usage
To use Extended Embedded Brainfuck interpreter include to c++ code file and execute Brainfuck commands.
Minimalistic example to run interpreter:
```c++
/* more includes */
#include "eebrainfuck_interpreter.h"

int main()
{
  BrainfuckInterpreter<> bf;
  const char[] code = "+++#";
  bf.execute(code, 4);
  
  /* more code */
}
```

This repository contain file `generate_eebf_code.py` which generate header file from `*.eebf` files. Python script search all files with extension `.eebf`, merge alphabeticly to one file, clean and create header file (default `eebrainfuck_code.h`). To use, include file `eebrainfuck_code.h.h` and pass arguments to Brainfuck execute function.
Example:
```c++
/* more includes */
#include "eebrainfuck_interpreter.h"
#include "eebrainfuck_code.h"

int main()
{
	BrainfuckInterpreter<> bf;
	bf.execute(eebf, eebfSize);

	/* more code */
}
```
Python script `generate_eebf_code.py` can be simply modified to change paths.


Using Extended Embedded Brainfuck interpreter with Arduino is the same. Example:
```c++
#include "eebrainfuck_interpreter.h"
#include "eebrainfuck_code.h"

void setup() 
{
  Serial.begin(9600); // This is needed to properly print debug messages
  BrainfuckInterpreter<> bf;
  bf.execute(eebf, eebfSize);
}

void loop() 
{ }
```

Type of memory and type of storage can be easly changed. 
Example:
```c++
BrainfuckInterpreter<int32_t, int64_t> bf; // Changes memory type to int32_t and storage type to int64_t
```

I suggest you to use PlatformIO, when compiling is on microcontroller. The reason is you can easly add Python script to automatic execing before compiling, just add to `platformio.ini` file:
```
extra_scripts = pre:generate_eebf_code.py
```

## Example

Folder `exaples\arduino_blink` contain ready to build and upload configuration for Arduino Mega2560 developmet board (should also work on Arduino Uno after change in `platformio.ini` file). Code written in Extended Embedded Brainfuck confguring buildin led and blinking. Code:
```
>>>>++++++[>++++++<-]>!>>~:
<<+!>>>:
#
>>+[>
~[>~[-]<-]~[>~[-]<-]~[>~[-]<-]~[>~[-]<-]
>>~:<<<]
```
