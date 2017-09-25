#ifndef _MPU_6050_H_
#define _MPU_6050_H_

#include <util/Util.h>

class Mpu6050
{
public:
	//+-2g, 16 bit 2's complement, 16384 LSB/g
	static uint16_t AcX, AcY, AcZ;

	//+-250 deg/sec, 16 bit 2's complement, 131 LSB/(deg/sec)
	static uint16_t GyX, GyY, GyZ;

	static uint8_t accel[6];
	static uint8_t gyro[6];

	static Bool Initialize();
	static Bool ReadSensor();
};

#endif
