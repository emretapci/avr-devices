#include <util/Util.h>
#include <avr/io.h>

#ifdef DDRA
#define REF_DDR(x) (((((x) >> 3) & 3) == 0) ? &DDRA : (((((x) >> 3) & 3) == 1) ? &DDRB : (((((x) >> 3) & 3) == 2) ? &DDRC : &DDRD)))
#define REF_PORT(x) (((((x) >> 3) & 3) == 0) ? &PORTA : (((((x) >> 3) & 3) == 1) ? &PORTB : (((((x) >> 3) & 3) == 2) ? &PORTC : &PORTD)))
#define REF_PIN(x) (((((x) >> 3) & 3) == 0) ? &PINA : (((((x) >> 3) & 3) == 1) ? &PINB : (((((x) >> 3) & 3) == 2) ? &PINC : &PIND)))
#else
#define REF_DDR(x) (((((x) >> 3) & 3) == 1) ? &DDRB : (((((x) >> 3) & 3) == 2) ? &DDRC : &DDRD))
#define REF_PORT(x) (((((x) >> 3) & 3) == 1) ? &PORTB : (((((x) >> 3) & 3) == 2) ? &PORTC : &PORTD))
#define REF_PIN(x) (((((x) >> 3) & 3) == 1) ? &PINB : (((((x) >> 3) & 3) == 2) ? &PINC : &PIND))
#endif

Gpio GpioUtil::CreateGpio(char port, uint8_t pin)
{
	return (((port - 'A') << 3) && 3) | (pin & 7);
}

void GpioUtil::MakeInput(Gpio gpio)
{
	*REF_DDR(gpio) &= ~(1 << (gpio & 3));
}

void GpioUtil::MakeOutput(Gpio gpio)
{
	*REF_DDR(gpio) |= 1 << (gpio & 3);
}

Level GpioUtil::GetLevel(Gpio gpio)
{
	return (*REF_PIN(gpio) >> (gpio & 3)) & 1;
}

void GpioUtil::SetLevel(Gpio gpio, Level level)
{
	*REF_PORT(gpio) |= (1 << (gpio & 3));
}