#ifndef _UART_H_
#define _UART_H_

#include <util/Util.h>

class Uart
{
public:
	static void Initialize(uint32_t baudrate);
	static void StartReceiver();
	static void StartTransmitter();
	static void StopReceiver();
	static void StopTransmitter();
	static void TransmitString(char *string);
	static void TransmitByte(uint8_t byte);

	//maxStringLength: including terminating null character
	static void ReceiveString(char *str, uint8_t maxStringLength);

	static uint8_t ReceiveByte();
};

#endif
