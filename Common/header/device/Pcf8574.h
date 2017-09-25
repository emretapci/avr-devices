#ifndef _PCF8574_H_
#define _PCF8574_H_

#include <util/Util.h>

class Pcf8574
{
public:
	//must be called from within a task function
	static void Initialize();

	//reads the masked pins
	static Bool Read(uint8_t deviceId, uint8_t mask, uint8_t *data);

	//writes the masked pins
	static Bool Write(uint8_t deviceId, uint8_t mask, uint8_t data);

private:
	static uint8_t written;
	static const uint8_t Pcf8574AddressBase = 0x3F; //device base address
	static const uint8_t Pcf8574MaxDevices = 8; //max devices, depends on address (3 bit)
	static const uint8_t Pcf8574MaxPins = 8; //max pins per device
};

#endif
