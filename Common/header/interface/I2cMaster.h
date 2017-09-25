#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

#include <util/Util.h>

class I2c
{
public:
	static const uint8_t READ = 0x01;
	static const uint8_t WRITE = 0x00;

	//scl_frequency: 100000 - 400000 bit/sec
	//prescaler: 0: 1, 1: 4, 2: 16, 3: 64
	static void Initialize(uint32_t sclFrequency, uint8_t prescaler);

	//reads a register. does not need start or stop.
	static Bool ReadRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, uint16_t length);

	//writes a register. does not need start or stop.
	static Bool WriteRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, uint16_t length);

	//transmits data. does not need start or stop.
	//address: Slave device's address, not shifted left.
	static Bool Transmit(uint8_t deviceAddress, uint8_t *data, uint16_t length);

	//receives data. does not need start or stop.
	//address: Slave device's address, not shifted left.
	static Bool Receive(uint8_t deviceAddress, uint8_t *data, uint16_t length);

	//address: Slave device's address, not shifted left.
	//read: true for read, false for write.
	static Bool Start(uint8_t deviceAddress, Bool read);

	static void Stop();

	//writes a byte. needs a start before and a stop after.
	static Bool Write(uint8_t data);

	//reads an ACK and returns read data. needs a start before and a stop after.
	static uint8_t ReadAck();

	//reads a NAK and returns read data. needs a start before and a stop after.
	static uint8_t ReadNak();
};

#endif
