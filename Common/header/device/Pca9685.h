#ifndef _PCA9685_H_
#define _PCA9685_H_

#include <util/Util.h>
#include <interface/I2cMaster.h>

class Pca9685
{
public:
	//hwAddress: X X A5 A4 A3 A2 A1 A0
	//must be called from a task function.
	static void Initialize(uint8_t hwAddressP);

	//sets PWM high at counter=0 and low at counter=width
	//port: 0-15, width: 0-4095
	static void SetPulseWidth(uint8_t port, uint16_t width);

private:
	static uint8_t hwAddress;
};

#endif
