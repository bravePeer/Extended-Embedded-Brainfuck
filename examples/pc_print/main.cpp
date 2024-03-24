#include <iostream>
#include <fstream>
#include "eebrainfuck_interpreter.h"
#include "eebrainfuck_code.h"

using namespace std;

//Command: .\main.exe ; python .\insert_eebf.py ; cl main.cpp ; ./out:main.exe

int main()
{
	// cout<<"Akkkkkffffffwdawd";
	// fstream file;
	// file.open("test.bf", ios::in);
	// if(!file.good())	
	// 	return -1;

	// char data[4096];
	// file.read(data, 4096);
	// string ss(data);

	BrainfuckInterpreter<> bf;

	bf.execute(eebf, eebfSize);
	// bf.printMemory(0, 10);

	// file.close();
	return 0;
}