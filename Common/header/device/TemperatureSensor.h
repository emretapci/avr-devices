#ifndef _TEMPERATURESENSOR_H_
#define _TEMPERATURESENSOR_H_

#include <util/Util.h>
#include <interface/one_wire.h>

class TemperatureSensor
{
public:
	static Initialize(Gpio pin, Bool useInternalPullUp, Bool oneBus, void (*callback)(void *, uint16_t));

private:
	static const uint8_t SkipRom = 0xcc;
	static const uint8_t StartConversion = 0x44;
	static const uint8_t ReadScratchpad = 0xbe;

	static OneWire sensorWire;
	static char data[10];
	static uint16_t temperature; //2^-4 resolution, 2's complement
	static void (*callback)(void *, uint16_t);
	static void *obj;
	static Bool converting;
	static uint8_t getResultTimerId;

	static void StartMeasuring(void *);
	static void GetMeasurement(void *);
};

#endif
