#ifndef _LED7S_H_
#define _LED7S_H_

#include <util/Util.h>

class Led7s
{
public:
	//segmentPins_: 7 element array of Gpio, for A .. G segments
	//digitPins_: numDigits element array of Gpio, for each digit, 0 for least significant digit
	//numDigits_: number of digits
	//anodeSegments_: true if segment pins are anode, false if digit pins are anode
	static void Initialize(Gpio *segmentPins_, Gpio *digitPins_, uint8_t numDigits_, Bool anodeSegments_);
	
	//number_: number to be displayed
	static void Display(uint8_t number_);

	//must be called periodically in a separate task to drive the led digits	
	static void TaskFunction();

private:
	static uint8_t displayedDigitIndex; //0 .. num_digits-1: 0: least significant digit
	static Gpio *segmentPins;
	static Gpio *digitPins;
	static uint32_t numberBcd; //binary coded decimal digits of number to be displayed (8 digits max)
	static uint8_t numDigits;
	static Bool anodeSegments;
	static const uint8_t segments[10];
};

#endif
