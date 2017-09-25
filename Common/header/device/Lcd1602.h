#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <util/Util.h>

class Lcd1602
{
public:
	//must be called from within a task function
	static void Initialize();

	static void SetBackLight(Bool on);

	static void SetRowText(uint8_t row, char *str);
	static void Scroll(Bool down);

private:
	static void Clear();
	static void ShiftText(Bool left);
	static void AppendChar(char c);
	static void SetCursorPosition(uint8_t row, uint8_t col);
	static void SendCommand(uint8_t command);
	static void SendData(uint8_t data);
	static uint8_t ReadData();
	static void Join();
	static void TriggerRead();
	static void TriggerWrite();

	static void ReadDdRam(uint8_t start, uint8_t length, uint8_t *buffer);
	static void WriteDdRam(uint8_t start, uint8_t length, uint8_t *buffer);
};

#endif
