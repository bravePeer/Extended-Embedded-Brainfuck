#pragma once
#include "simple_stack.h"

#define EXTENDED_EMBEDDED_BRAINFUCK

#define SIMULATE_SET_ADDRESS
#define USE_ARDUINO_WAIT_FOR_INPUT

#define USE_PRINT_ON_STD_STREAM
// #define USE_PRINT_ON_ARDUINO_SERIAL
#define BrainfuckSerial Serial


#define BRAINFUCK_MEMORY_SIZE 30000
#define BRAINFUCK_DEBUG_BEFORE_MEMORY_INDEX 0
#define BRAINFUCK_DEBUG_AFTER_MEMORY_INDEX 20


#if defined(PRINT_DEBUG_ON_STD_STREAM) && defined(PRINT_DEBUG_ON_ARDUINO_SERIAL)
#error PRINT_DEBUG_ON_STD_STREAM and PRINT_DEBUG_ON_ARDUINO_SERIAL are defined, should be defined only one!
#endif

template <class T = int8_t, class K = int32_t>
class BrainfuckInterpreter
{
public:
	BrainfuckInterpreter(unsigned int memorySize = BRAINFUCK_MEMORY_SIZE)
		:memorySize(memorySize)
	{
		memory = new T[memorySize] {0};
		memoryPointer = memory;
		additionalBracetCounter = 0;
		jumping = false;
		storageMemory = 0;
	}
	~BrainfuckInterpreter()
	{
		delete[] memory;
	}

	int executeInstruction(char instruction, int instructionIndex)
	{
		if (jumping)
		{
			if (instruction == '[')
				additionalBracetCounter++;

			if (instruction == ']')
			{
				if (additionalBracetCounter == 0)
				{
					jumping = false;
					return instructionIndex;
				}

				additionalBracetCounter--;
			}

			return instructionIndex;
		}

		switch (instruction)
		{
		case '>':
			memoryPointer++;
			if (memoryPointer > &memory[memorySize-1])
				memoryPointer = &memory[0];
			break;
		case '<':
			memoryPointer--;
			if (memoryPointer < &memory[0])
				memoryPointer = &memory[memorySize - 1];
			break;
		case '+':
			(*memoryPointer)++;
			break;
		case '-':
			(*memoryPointer)--;
			break;
		case '[':
			beginBracket.push(instructionIndex);
			if (*memoryPointer == 0) // jump
			{
				jumping = true;
			}
			break;
		case ']':
		{
			int tmp = beginBracket.top();
			if (*memoryPointer != 0)
			{
				instructionIndex = tmp;
			}
			else
				beginBracket.pop();
		}
			break;
		case ',':
		#if defined(USE_PRINT_ON_STD_STREAM)
			*memoryPointer = getchar();
		#elif defined(USE_PRINT_ON_ARDUINO_SERIAL)
			#if defined(USE_ARDUINO_WAIT_FOR_INPUT)
			while (BrainfuckSerial.available() == 0)  // It should be made in bf code
			{	}
			#endif
			*memoryPointer = BrainfuckSerial.read();
		#endif
			break;
		case '.':
		#if defined(USE_PRINT_ON_STD_STREAM)
			// putchar(memory[memoryPointer]);
			cout << *memoryPointer;
		#elif defined(USE_PRINT_ON_ARDUINO_SERIAL)
			BrainfuckSerial.write(*memoryPointer);
		#endif

			break;
#ifdef EXTENDED_EMBEDDED_BRAINFUCK
		case '~':
			*memoryPointer = ~(*memoryPointer);
			break;
		case '^':
			*memoryPointer ^= storageMemory;
			break;
		case '|':
			*memoryPointer |= storageMemory;
			break;
		case '&':
			*memoryPointer &= storageMemory;
			break;
		case '_':
			*memoryPointer -= storageMemory;
			break;
		case '=':
			*memoryPointer += storageMemory;
			break;
		case '*':
			*memoryPointer *= storageMemory;
			break;
		case '/':
			*memoryPointer /= storageMemory;
			break;
		case '%':
			*memoryPointer %= storageMemory;
			break;
		case '!':
			storageMemory = *memoryPointer;
			break;
		case '?':
			*memoryPointer = storageMemory;
			break;
		case '$':
			instructionIndex = storageMemory;
			break;
		case ':': // Memory cell is physical address
	#if defined(SIMULATE_SET_ADDRESS)
		#if defined(USE_PRINT_ON_STD_STREAM)
			cout << "Set value at address: " << (int)storageMemory << " to value at memory pointer: "
			<< (int)(memoryPointer - memory[0]) << "(" << (int)*memoryPointer << ")" << endl;
			
		#elif defined(USE_PRINT_ON_ARDUINO_SERIAL)
			BrainfuckInterpreter.print("Set value at address: ");
			BrainfuckInterpreter.print((int)storageMemory);
			BrainfuckInterpreter.print(" to value at memory pointer: ");
			BrainfuckInterpreter.print((int)(memoryPointer - memory[0]));
			BrainfuckInterpreter.print("(");
			BrainfuckInterpreter.print((int)*memoryPointer);
			BrainfuckInterpreter.print(")");
			BrainfuckInterpreter.print("\n");
		#endif
	#else
			*reinterpret_cast<int*>(storageMemory) = *memoryPointer;
	#endif
		break;
		case ';':
	#if defined(SIMULATE_SET_ADDRESS)
		#if defined(USE_PRINT_ON_STD_STREAM)
			cout << "Get value from address: " << (int)storageMemory<<" and set at memory pointer: "<<(int)(memoryPointer - memory[0])<<endl;
			
		#elif defined(USE_PRINT_ON_ARDUINO_SERIAL)
			BrainfuckInterpreter.print("Get value from address: ");
			BrainfuckInterpreter.print((int)storageMemory);
			BrainfuckInterpreter.print(" and set at memory pointer: ");
			BrainfuckInterpreter.print((int)(memoryPointer - memory[0]));
			BrainfuckInterpreter.print("\n");
		#endif
	#else
			*memoryPointer = *reinterpret_cast<int*>(storageMemory);
	#endif
		break;
		case '#': // Print debug
			printMemory(
				memoryPointer - BRAINFUCK_DEBUG_BEFORE_MEMORY_INDEX,
				BRAINFUCK_DEBUG_AFTER_MEMORY_INDEX 
				);
		break;
#endif
		}
		return instructionIndex;
	}

	void execute(const char code[], size_t length)
	{
		for (size_t i = 0; i < length; i++)
		{
			i = executeInstruction(code[i], i);
		}
	}

	void printMemory(T* tmpPointer, T lenght)
	{
		if(tmpPointer < &memory[0])
			tmpPointer = &memory[0];
		T* tmp = (tmpPointer + lenght > &memory[memorySize-1]) ? &memory[memorySize - 1] : tmpPointer + lenght;
		
	#if defined(USE_PRINT_ON_STD_STREAM)
		cout << endl << "----Memory Dump----" << endl;
		for (T* i = tmpPointer; i != tmp; i++)
		{
			cout << "[" << static_cast<int>(*i) << "]";
		}
		cout<<"\n ^ "<< tmpPointer - &memory[0] <<endl;
		cout<<"memoryIndex: "<< tmpPointer - &memory[0]<<endl;
		cout<<"storage: "<< static_cast<int>(storageMemory) <<endl;
	#elif defined(USE_PRINT_ON_ARDUINO_SERIAL)
		BrainfuckSerial.print("\n----Memory Dump----\n");

		for (T* i = tmpPointer; i != tmp; i++)
		{
			BrainfuckSerial.print("[");
			BrainfuckSerial.print(static_cast<int>(*i));
			BrainfuckSerial.print("]");
		}
		BrainfuckSerial.print("\n ^ ");
		BrainfuckSerial.print(tmpPointer - &memory[0]);
		BrainfuckSerial.print("\nmemoryIndex: ");
		BrainfuckSerial.print(tmpPointer - &memory[0]);
		BrainfuckSerial.print("\nstorage: ");
		BrainfuckSerial.print(static_cast<int>(storageMemory));
		BrainfuckSerial.print("\n");
	#endif
	}

private:
	const unsigned int memorySize;
	T* memory;
	T* memoryPointer;
	Stack<int> beginBracket;
	bool jumping;
	int additionalBracetCounter;
	K storageMemory;
};