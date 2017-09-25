#ifndef _UTIL_H_
#define _UTIL_H_

#include <avr/io.h>

typedef uint8_t Bool;
typedef uint8_t Gpio; //bits 4-3: PortA-PortD, bits 2-0: Pin7-0
typedef uint8_t Level; //0 or 1

class GpioUtil
{
public:
	static const uint8_t High = 1;
	static const uint8_t Low = 0;

	//Creates a Gpio.
	//port: 'A' .. 'D', pin: 0 .. 7
	static Gpio CreateGpio(char port, uint8_t pin);

	//Sets given Gpio as input.
	static void MakeInput(Gpio gpio);

	//Sets given Gpio as output.
	static void MakeOutput(Gpio gpio);

	//Gets the level of the given Gpio.
	static Level GetLevel(Gpio gpio);

	//Sets the level of the given Gpio.
	static void SetLevel(Gpio gpio, Level level);
};

#endif
