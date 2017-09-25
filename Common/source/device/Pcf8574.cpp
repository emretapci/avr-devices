#include <avr/io.h>

#include <interface/I2cMaster.h>
#include <device/pcf8574.h>

#include <freertos.h>
#include <task.h>

uint8_t Pcf8574::written;

void Pcf8574::Initialize()
{
	I2c::Initialize(100000, 0);
	vTaskDelay(10);
}

Bool Pcf8574::Read(uint8_t deviceid, uint8_t mask, uint8_t *pdata)
{
	if(deviceid >= 0 && deviceid < Pcf8574MaxDevices)
	{
		//make mask pins high
		if(!Write(deviceid, 0xff, written | mask))
		{
			return false;
		}

		//read mask pins
		I2c::Start(Pcf8574AddressBase + deviceid, I2c::READ);
		*pdata = (I2c::ReadNak()) & mask | (*pdata & ~mask);
		I2c::Stop();
		written |= mask;
		return true;
	}
	return false;
}

Bool Pcf8574::Write(uint8_t deviceid, uint8_t mask, uint8_t data)
{
	if(deviceid >= 0 && deviceid < Pcf8574MaxDevices)
	{
		if(!I2c::Start(Pcf8574AddressBase + deviceid, I2c::WRITE))
		{
			return false;
		}
		uint8_t towrite = (data & mask) | (written & ~mask);
		if(!I2c::Write(towrite))
		{
			return false;
		}
		I2c::Stop();
		written = towrite;
		return true;
	}
	return false;
}
