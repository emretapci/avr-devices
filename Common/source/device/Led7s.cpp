#include <device/led7s.h>
#include <util/Util.h>

const uint8_t Led7s::segments[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

void Led7s::Initialize(Gpio *segmentPins_, Gpio *digitPins_, uint8_t numDigits_, Bool anodeSegments_)
{
	segmentPins = segmentPins_;
	digitPins = digitPins_;
	numDigits = numDigits_;
	anodeSegments = anodeSegments_;

	displayedDigitIndex = numDigits - 1;

	for(uint8_t i = 0; i < 7; i++)
	{
		GpioUtil::MakeOutput(segmentPins[i]);
	}

	for(uint8_t i = 0; i < numDigits; i++)
	{
		GpioUtil::MakeOutput(digitPins[i]);
	}
}

void Led7s::Display(uint8_t number_)
{
	//convert 32 bit binary number to bcd format
	//numberBcd = Utils::ConvertBinaryToBcd(number_); TODO
}

void Led7s::TaskFunction()
{
	uint8_t digit = (numberBcd >> (displayedDigitIndex * 4)) & 0x0f;

	for(uint8_t segmentIndex = 0; segmentIndex < 7; segmentIndex++)
	{
		if(((segments[digit] & (1 << segmentIndex)) > 0) ^ anodeSegments)
		{
			GpioUtil::SetLevel(segmentPins[segmentIndex], GpioUtil::Low);
		}
		else
		{
			GpioUtil::SetLevel(segmentPins[segmentIndex], GpioUtil::High);
		}
	}

	for(uint8_t digitIndex = 0; digitIndex < numDigits; digitIndex++)
	{
		if((digitIndex == displayedDigitIndex) ^ anodeSegments)
		{
			GpioUtil::SetLevel(digitPins[numDigits - digitIndex - 1], GpioUtil::High);
		}
		else
		{
			GpioUtil::SetLevel(digitPins[numDigits - digitIndex - 1], GpioUtil::Low);
		}
	}

	displayedDigitIndex = displayedDigitIndex - 1;
	
	if(displayedDigitIndex == 0xff)
	{
		displayedDigitIndex = numDigits - 1;
	}
}
