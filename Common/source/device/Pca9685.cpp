#include <device/Pca9685.h>

#include <freertos.h>
#include <task.h>

#define MODE1 0x00
#define MODE2 0x01
#define PRESCALE 0xfe
#define LED0_ON_L 0x06

void Pca9685::Initialize(uint8_t hwAddressP)
{
	I2c::Initialize(100000, 0);
	hwAddress = hwAddressP;

	I2c::Start(hwAddress, I2c::WRITE);
	I2c::Write(MODE1);
	I2c::Write(0x31); //all call, sleep, auto increment
	I2c::Stop();

	vTaskDelay(1);

	I2c::Start(hwAddress, I2c::WRITE);
	I2c::Write(PRESCALE);
	I2c::Write(F_CPU / (4096 * 50) - 1);
	I2c::Stop();

	vTaskDelay(1);

	I2c::Start(hwAddress, I2c::WRITE);
	I2c::Write(MODE1);
	I2c::Write(0xa1); //restart, auto increment, internal clock, normal mode, all call
	I2c::Stop();

	vTaskDelay(1);

	I2c::Start(hwAddress, I2c::WRITE);
	I2c::Write(MODE2);
	I2c::Write(0x04); //output logic state not inverted, outputs change on ACK
	I2c::Stop();
}

void Pca9685::SetPulseWidth(uint8_t port, uint16_t width)
{
	uint8_t width8 = width;
	I2c::Start(hwAddress, I2c::WRITE);
	I2c::Write(LED0_ON_L + 4 * port);
	I2c::Write(0x00); //LEDx_ON_L
	I2c::Write(0x00); //LEDx_ON_H
	I2c::Write(width8); //LEDx_OFF_L
	width8 = width >> 8;
	I2c::Write(width8); //LEDx_OFF_H
	I2c::Stop();
}