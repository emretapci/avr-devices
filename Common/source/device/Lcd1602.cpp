#include <device/Lcd1602.h>
#include <device/pcf8574.h>

#include <freertos.h>
#include <task.h>

#define RS 0
#define RW 1
#define EN 2
#define BL 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define MAXROW 2
#define MAXCOL 16

void Lcd1602::Initialize()
{
	Pcf8574::Initialize();

	vTaskDelay(20);
	SendCommand(0x33);
	vTaskDelay(10);
	SendCommand(0x32);
	vTaskDelay(10);
	SendCommand(0x28); //function set, 4-bit mode, 2 lines
	SendCommand(0x0C); //display: on, cursor: on, blink cursor: off
	SendCommand(0x01); //clear display and returns cursor to the home position
	SendCommand(0x06); //entry mode set, increment cursor pos, do not shift display
	SendCommand(0x80); //set DDRAM address to 0
	SendCommand(0x0C); //display: on, cursor: on, blink cursor: off
}

void Lcd1602::SetBackLight(Bool on)
{
	Pcf8574::Write(0, 1 << BL, on ? 0xff : 0x00);
}

void Lcd1602::SetRowText(uint8_t row, char *str)
{
	uint8_t pos = 0;
	SetCursorPosition(row, 0);
	while(*str && pos < MAXCOL)
	{
		AppendChar(*str);
		str++;
		pos++;
	}
	while(pos < MAXCOL)
	{
		AppendChar(' ');
		pos++;
	}
}

void Lcd1602::Scroll(Bool down)
{
	uint8_t buffer[MAXCOL];

	if (down)
	{
		//read row 1 into buffer
		ReadDdRam(0x40, 16, buffer);

		//write buffer to row 0
		WriteDdRam(0, 16, buffer);

		//write blank line to row 1
		/*buffer[0] = ' ';
		for(uint8_t i = 0; i < MAXCOL; i++)
		{
			WriteDdRam(0x40 + i, 1, buffer);
		}*/
	}
	else
	{
		//read row 0 into buffer
		ReadDdRam(0, 16, buffer);

		//write buffer to row 1
		WriteDdRam(0x40, 16, buffer);

		//write blank line to row 0
		/*buffer[0] = ' ';
		for(uint8_t i = 0; i < MAXCOL; i++)
		{
			WriteDdRam(i, 1, buffer);
		}*/
	}
}

void Lcd1602::Clear()
{
	SendCommand(0x01);
}

void Lcd1602::ShiftText(Bool left)
{
	SendCommand(left ? 0x18 : 0x1C);
}

void Lcd1602::AppendChar(char c)
{
	SendData(c);
}

void Lcd1602::SetCursorPosition(uint8_t row, uint8_t col)
{
	SendCommand(0x80 | (row * 0x40 + col));
}

void Lcd1602::SendCommand(uint8_t command)
{
	Join();
	Pcf8574::Write(0, 1 << RW, 0); //set RW=0
	Pcf8574::Write(0, 1 << RS, 0); //set RS=0
	Pcf8574::Write(0, 0xf0, command); //write high nibble
	TriggerWrite();
	Pcf8574::Write(0, 0xf0, command << 4); //write low nibble
	TriggerWrite();
}

void Lcd1602::SendData(uint8_t data)
{
	Join();
	Pcf8574::Write(0, 1 << RW, 0); //set RW=0
	Pcf8574::Write(0, 1 << RS, 0xff); //set RS=1
	Pcf8574::Write(0, 0xf0, data); //write high nibble
	TriggerWrite();
	Pcf8574::Write(0, 0xf0, data << 4); //write low nibble
	TriggerWrite();
}

uint8_t Lcd1602::ReadData()
{
	Join();
	Pcf8574::Write(0, 1 << EN, 0x00); //set EN=0
	Pcf8574::Write(0, 1 << RW, 0xff); //set RW=1
	Pcf8574::Write(0, 1 << RS, 0xff); //set RS=1

	uint8_t temp = 0, data = 0;

	//read high nibble
	TriggerRead();
	Pcf8574::Read(0, 0xf0, &temp);
	data |= temp;

	//read low nibble
	TriggerRead();
	temp = 0;
	Pcf8574::Read(0, 0xf0, &temp);
	data |= (temp >> 4);

	return data;
}

void Lcd1602::Join()
{
	Pcf8574::Write(0, 1 << RW | 1 << RS, 1 << RW); //set RS=0, RW=1 for status input

	uint8_t data = 1 << D7;
	while(data & (1 << D7) > 0)
	{
		Pcf8574::Read(0, 1 << D7, &data);
	}
}

void Lcd1602::TriggerRead()
{
	Pcf8574::Write(0, 1 << EN, 0);
	vTaskDelay(2);
	Pcf8574::Write(0, 1 << EN, 0xff);
}

void Lcd1602::TriggerWrite()
{
	Pcf8574::Write(0, 1 << EN, 0xff);
	vTaskDelay(2);
	Pcf8574::Write(0, 1 << EN, 0);
}

void Lcd1602::ReadDdRam(uint8_t start, uint8_t length, uint8_t *buffer)
{
	SendCommand(0x80 | start); //set DDRAM address

  	uint8_t pos = 0;
	while(pos < length)
	{
		buffer[pos++] = ReadData();
	}
}

void Lcd1602::WriteDdRam(uint8_t start, uint8_t length, uint8_t *buffer)
{
	SendCommand(0x80 | start); //set DDRAM address

	uint8_t pos = 0;
	while(pos < length)
	{
		SendData(buffer[pos++]);
	}
}
