#ifndef _SHIFTREGISTER_H_
#define _SHIFTREGISTER_H_

#include <util/Util.h>

class ShiftRegister
{
public:
	void Initialize(Gpio shift, Gpio store, Gpio data);
	
	//msb .. lsb bits are: sr2.qh .. sr2.qa sr1.qh .. sr1.qa
	void SetState(uint16_t data);

private:
	static Gpio shift;
	static Gpio store;
	static Gpio data;

	void SetDataPin(Level level);
	void Shift();
	void Store();
};

#endif
