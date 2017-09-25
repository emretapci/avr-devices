#include <avr/interrupt.h>
#include <device/ShiftRegister.h>
#include <FreeRTOS.h>
#include <task.h>

void ShiftRegister::Initialize(Gpio shift, Gpio store, Gpio data)
{
	this->shift = shift;
	this->store = store;
	this->data = data;

	GpioUtil::MakeOutput(shift);
	GpioUtil::MakeOutput(store);
	GpioUtil::MakeOutput(data);
}

void ShiftRegister::SetDataPin(Level level)
{
	GpioUtil::SetLevel(data, level);
}

void ShiftRegister::Shift()
{
	GpioUtil::SetLevel(shift, GpioUtil::Low);
	GpioUtil::SetLevel(shift, GpioUtil::High);
}

void ShiftRegister::Store()
{
	GpioUtil::SetLevel(store, GpioUtil::Low);
	GpioUtil::SetLevel(store, GpioUtil::High);
}

void ShiftRegister::SetState(uint16_t data)
{
	taskENTER_CRITICAL();
	uint8_t srPatternHigh = data >> 8, srPatternLow = data & 0xff, i;

	for(i = 0; i < 8; i++)
	{
		SetDataPin(srPatternHigh >> 7);
		Shift();
		srPatternHigh = (srPatternHigh << 1);
	}

	for(i = 0; i < 8; i++)
	{
		SetDataPin(srPatternLow >> 7);
		Shift();
		srPatternLow = (srPatternLow << 1);
	}
	Store();
	taskEXIT_CRITICAL();
}
