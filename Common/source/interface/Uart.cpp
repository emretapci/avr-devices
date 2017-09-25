#include <interface/Uart.h>

void Uart::Initialize(uint32_t baudrate)
{
	uint16_t regval = ((F_CPU)/(baudrate * 16UL)) - 1;
	UBRR0L = (uint8_t)(regval & 0xff);
	UBRR0H = (uint8_t)(regval >> 8); //set baud rate
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); //8 bit data format
	UCSR0B |= (1 << RXCIE0);
	UCSR0B &= ~(1 << RXCIE0); //disable receive interrupt. interferes with freertos
	UCSR0B &= ~(1 << TXCIE0); //disable transmit interrupt. interferes with freertos
}

void Uart::StartReceiver()
{
	UCSR0B |= (1 << RXEN0);
}

void Uart::StartTransmitter()
{
	UCSR0B |= (1 << TXEN0);
}

void Uart::StopReceiver()
{
	UCSR0B &= ~(1 << RXEN0);
}

void Uart::StopTransmitter()
{
	UCSR0B &= ~(1 << TXEN0);
}

void Uart::TransmitString(char *string)
{
	while(*string != 0x00)
	{
		TransmitByte((uint8_t)(*string));
		string++;
	}
}

void Uart::TransmitByte(uint8_t b)
{
	while((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = (unsigned char)b;
	while((UCSR0A & (1 << TXC0)) == 0);
}

void Uart::ReceiveString(char *str, uint8_t maxStringLength)
{
	uint8_t index = 0;
	while(true)
	{
		if(index == maxStringLength)
		{
			str[index] = 0;
			break;
		}
		str[index] = (char)ReceiveByte();
		if(str[index] == 0)
		{
			break;
		}
		index++;
	}
}

uint8_t Uart::ReceiveByte()
{
	//wait until a byte is ready to read
	while((UCSR0A & (1 << RXC0)) == 0);

	//grab the byte from the serial port
	return UDR0;
}
