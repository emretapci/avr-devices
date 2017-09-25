#ifndef _ONEWIRE_H_
#define _ONEWIRE_H_

#include <util/Util.h>

class OneWire
{
public:
	static void Initialize(Gpio pin, Bool useInternalPullUp, Bool oneBus);
	static uint8_t Reset();
	static uint8_t BitIo(uint8_t b);
	static uint8_t WriteByte(uint8_t b);
	static uint8_t ReadByte();
	static uint8_t RomSearch(uint8_t diff, uint8_t *id);
	static void Command(uint8_t command, uint8_t *id);
	static void CommandWithParasiteEnable(uint8_t command, uint8_t *id);
	static void ParasiteEnable();
	static void ParasiteDisable();

private:
	static const uint16_t RecoveryTime = 10; //usec
	static const uint8_t MatchRom = 0x55;
	static const uint8_t SkipRom = 0xcc;
	static const uint8_t SearchRom = 0xf0;
	static const uint8_t SearchFirst = 0xff;
	static const uint8_t PresenceErr = 0xff;
	static const uint8_t DataErr = 0xfe;
	static const uint8_t LastDevice = 0x00;
	static const uint8_t RomCodeSize = 8; //rom-code size including CRC
	static const uint8_t ConfCyclesPreaccess = 13;
	static const uint16_t ConfDelayOffset = (uint16_t)(ConfCyclesPreaccess * 1000000L / F_CPU);
	static Gpio pin;
	static Bool useInternalPullUp;
	static Bool oneBus;
	static uint8_t BitIoInternal(uint8_t b, uint8_t withParasiteEnable);
	static void CommandInternal(uint8_t command, uint8_t *id, uint8_t withParasiteEnable);
	static uint8_t WriteByteWithParasiteEnabled(uint8_t b);
};

#endif
