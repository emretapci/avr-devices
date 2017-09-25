#include <interface/OneWire.h>
#include <util/atomic.h>
#include <util/delay.h>

void OneWire::Initialize(Gpio pin, Bool useInternalPullUp_p, Bool oneBus_p)
{
	useInternalPullUp = useInternalPullUp_p;
	oneBus = oneBus_p;
	OneWire::pin = pin;
	GpioUtil::MakeOutput(pin);
	Reset();
}

uint8_t OneWire::Reset()
{
	uint8_t err;

	GpioUtil::MakeOutput(pin);
	GpioUtil::SetLevel(pin, GpioUtil::Low);
	_delay_us(480);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		//set pin as input, wait for clients to pull low
		GpioUtil::MakeInput(pin);

		if(useInternalPullUp)
		{
			GpioUtil::SetLevel(pin, GpioUtil::High);
		}
		
		_delay_us(64);
		err = GpioUtil::GetLevel(pin); //no presence detect
		//if err!=0: nobody pulled to low, still high
	}

	//after a delay the clients should release the line
	//and input-pin gets back to high by pull-up-resistor
	_delay_us(480 - 64);
	if(GpioUtil::GetLevel(pin) == GpioUtil::Low)
	{
		err = 1; //short circuit, expected low but got high
	}

	return err;
}

uint8_t OneWire::BitIo(uint8_t b)
{
	return BitIoInternal(b & 1, 0);
}

uint8_t OneWire::WriteByte(uint8_t b)
{
	uint8_t i = 8, j;

	do
	{
		j = BitIo(b & 1);
		b >>= 1;
		if(j)
		{
			b |= 0x80;
		}
	}
	while(--i);

	return b;
}

uint8_t OneWire::WriteByteWithParasiteEnabled(uint8_t b)
{
	uint8_t i = 8, j;

	do
	{
		if (i != 1)
		{
			j = BitIoInternal(b & 1, 0);
		}
		else
		{
			j = BitIoInternal(b & 1, 1);
		}
		b >>= 1;
		if(j)
		{
			b |= 0x80;
		}
	}
	while(--i);

	return b;
}

uint8_t OneWire::ReadByte()
{
	//read by sending only "1"s, so bus gets released
	//after the init low-pulse in every slot
	return WriteByte(0xff);
}

uint8_t OneWire::RomSearch(uint8_t diff, uint8_t *id)
{
	uint8_t i, j, next_diff;
	uint8_t b;

	if(Reset())
	{
		return PresenceErr; //error, no device found <--- early exit!
	}

	WriteByte(SearchRom);

	next_diff = LastDevice; //unchanged on last device

	i = RomCodeSize * 8;

	do
	{
		j = 8;

		do
		{
			b = BitIo(1); //read bit

			if(BitIo(1))
			{
				if(b)
				{
					return DataErr; //data error <--- early exit!
				}
			}
			else
			{
				if(!b)
				{ //0b00 = 2 devices
					if(diff > i || ((*id & 1) && diff != i))
					{
						b = 1; //now 1
						next_diff = i; //next pass 0
					}
				}
			}

			BitIo(b); //write bit

			(*id) >>= 1;

			if(b)
			{
				(*id) |= 0x80; //store bit
			}

			i--;
		}
		while( --j );

		id++; //next byte
	}
	while(i);

	return next_diff; //to continue search
}

void OneWire::Command(uint8_t command, uint8_t *id)
{
	CommandInternal(command, id, 0);
}

void OneWire::CommandWithParasiteEnable(uint8_t command, uint8_t *id)
{
	CommandInternal(command, id, 1);
}

void OneWire::ParasiteEnable()
{
	GpioUtil::MakeOutput(pin);
	GpioUtil::SetLevel(pin, GpioUtil::High);
}

void OneWire::ParasiteDisable()
{
	GpioUtil::MakeInput(pin);
	if(!useInternalPullUp)
	{
		GpioUtil::SetLevel(pin, GpioUtil::Low);
	}
}

uint8_t OneWire::BitIoInternal(uint8_t b, uint8_t with_parasite_enable)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		GpioUtil::MakeOutput(pin);
		if(useInternalPullUp)
		{
			GpioUtil::SetLevel(pin, GpioUtil::Low);
		}

		_delay_us(2);    //T_INT > 1usec accoding to timing-diagramm
		if(b)
		{
			GpioUtil::MakeInput(pin); //to write "1" release bus, resistor pulls high
			if(useInternalPullUp)
			{
				GpioUtil::SetLevel(pin, GpioUtil::High);
			}
		}

		//"Output data from the DS18B20 is valid for 15usec after the falling
		//edge that initiated the read time slot. Therefore, the master must
		//release the bus and then sample the bus state within 15ussec from
		//the start of the slot."
		_delay_us(15 - 2 - ConfDelayOffset);

		if(GpioUtil::GetLevel(pin) == GpioUtil::Low)
		{
			b = 0;  //sample at end of read-timeslot
		}

		_delay_us(60 - 15 - 2 + ConfDelayOffset);

		if(useInternalPullUp)
		{
			GpioUtil::SetLevel(pin, GpioUtil::High);
		}

			GpioUtil::MakeInput(pin); //to write "1" release bus, resistor pulls high
		
		if (with_parasite_enable)
		{
			ParasiteEnable();
		}
	}

	_delay_us(RecoveryTime); //may be increased for longer wires

	return b;
}

void OneWire::CommandInternal(uint8_t command, uint8_t *id, uint8_t with_parasite_enable)
{
	uint8_t i;

	Reset();

	if(id)
	{
		WriteByte(MatchRom); //to a single device
		i = RomCodeSize;

		do
		{
			WriteByte(*id);
			id++;
		}
		while(--i);
	}
	else
	{
		WriteByte(SkipRom); //to all devices
	}

	if(with_parasite_enable)
	{
		WriteByteWithParasiteEnabled(command);
	}
	else
	{
		WriteByte(command);
	}
}
