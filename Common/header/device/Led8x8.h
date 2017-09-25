#ifndef _LED8X8_H_
#define _LED8X8_H_

#include <util/Util.h>
#include <device/ShiftRegister.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

class Led8x8
{
public:
	static void Initialize(uint8_t shift, uint8_t store, uint8_t data);

	//sets displayed text	
	static void DisplayText(char *text);

	//every call to this function slides displayed text left by one pixel
	static void SlideText();

	//must be called in a separate task
	static void Drive();

private:
	static const uint8_t anodes[8];
	static const uint8_t cathodes[8];
	static const uint8_t ledToSr[16] PROGMEM;
	static const uint8_t ascii[95 * 8] PROGMEM;

	static uint8_t row;
	static uint16_t slidingCol;
	static uint16_t slidingTextLength;
	static char *slidingText;
	static ShiftRegister sr;
	static uint8_t matrix[8];

	static void ReadCharMatrixFromPgm(uint8_t *matrix, char letter);
	static void ToggleLed(uint8_t row, uint8_t col, Bool on);
};

#endif
