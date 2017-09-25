#include <device/TemperatureSensor.h>
#include <core/timer.h>
#include <util/delay.h>

TemperatureSensor::TemperatureSensor(Gpio pin, Bool useInternalPullUp, Bool oneBus, void (*callback)(void *, uint16_t))
{
	sensorWire.Initialize(port, pin, useInternalPullUp, oneBus);
	converting = false;
}

void TemperatureSensor::StartMeasuring(void *obj)
{
	TemperatureSensor *instance = (TemperatureSensor *)obj;

	instance->sensorWire.Reset();
	instance->sensorWire.Command(SkipRom, 0);
	instance->sensorWire.Command(StartConversion, 0);

	//wait 100 ms and get result.
	Timer::Start(instance->getResultTimerId);
}

void TemperatureSensor::GetMeasurement(void *obj)
{
	TemperatureSensor *instance = (TemperatureSensor *)obj;

	Timer::Stop(instance->getResultTimerId);

	instance->sensorWire.Reset();
	instance->sensorWire.Command(SkipRom, 0);
	instance->sensorWire.Command(ReadScratchpad, 0);

	for(uint8_t i = 0; i < 10; i++)
	{
		instance->data[i] = instance->sensorWire.ReadByte();
	}

	((uint8_t *)(&(instance->temperature)))[0] = instance->data[0];
	((uint8_t *)(&(instance->temperature)))[1] = instance->data[1];

	(instance->callback)(instance->obj, instance->temperature);
}

void TemperatureSensor::SetHandler(void (*callback)(void *, INT16), void *obj)
{
	this->callback = callback;
	this->obj = obj;

	if(!converting)
	{
		converting = true;
		Timer::Start(Timer::CreateTimer(StartMeasuring, 1000, this));
		getResultTimerId = Timer::CreateTimer(GetMeasurement, 100, this);
	}
}