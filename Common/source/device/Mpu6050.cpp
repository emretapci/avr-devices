#include <device/Mpu6050.h>
#include <interface/I2cMaster.h>

#include <util/delay.h>

uint16_t Mpu6050::AcX, Mpu6050::AcY, Mpu6050::AcZ;
uint16_t Mpu6050::GyX, Mpu6050::GyY, Mpu6050::GyZ;
uint8_t Mpu6050::accel[6];
uint8_t Mpu6050::gyro[6];

uint8_t Mpu6050::Initialize()
{
	I2c::Initialize(100000, 1);

	uint8_t data;

	//xxx00xxx
	data = 0x00;
	if(!I2c::WriteRegister(0x68, 0x1B, &data, 1)) //+- 250 deg/sec
	{
		return false;
	}

	//xxx00xxx
	data = 0x00;
	if(!I2c::WriteRegister(0x68, 0x1C, &data, 1)) //+-2g range
	{
		return false;
	}

	//x0xxxxx1
	data = 0x01;
	if(!I2c::WriteRegister(0x68, 0x6B, &data, 1))
	{
		return false;
	}

	return true;
}

uint8_t Mpu6050::ReadSensor()
{
	if(!I2c::ReadRegister(0x68, 0x3b, accel, 6)) //ACCEL_XOUT_H
	{
		return false;
	}

	if(!I2c::ReadRegister(0x68, 0x43, gyro, 6)) //GYRO_XOUT_H
	{
		return false;
	}

	AcX = accel[0] << 8 | accel[1];
	AcY = accel[2] << 8 | accel[3];
	AcZ = accel[4] << 8 | accel[5];

	GyX = gyro[0] << 8 | gyro[1];
	GyY = gyro[2] << 8 | gyro[3];
	GyZ = gyro[4] << 8 | gyro[5];

	return true;
}
