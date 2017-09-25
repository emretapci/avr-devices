#include <device/Esp8266.h>
#include <interface/uart.h>
#include <string.h>
#include <stdlib.h>

#include <freertos.h>
#include <task.h>

char Esp8266::messagePush[BUFFER_SIZE], Esp8266::messagePop[BUFFER_SIZE];
QueueHandle_t Esp8266::messageQueue;
SemaphoreHandle_t Esp8266::semQueueLock;
uint8_t Esp8266::msgpos;
void (*Esp8266::MessageReceived)(const char *);

void Esp8266::Initialize(void (*MessageReceivedP)(const char *))
{
	Uart::Initialize(9600);
	Uart::StartTransmitter();
	Uart::StartReceiver();
	MessageReceived = MessageReceivedP;
	msgpos = 0;

	messageQueue = xQueueCreate(6, BUFFER_SIZE);
	semQueueLock = xSemaphoreCreateMutex();

	xTaskCreate(vReadEsp8266, "vReadEsp8266", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY + 3, 0);
	xTaskCreate(vPollMessage, "vPollMessage", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY + 3, 0);
}

void Esp8266::Test()
{
	taskENTER_CRITICAL();
	Uart::TransmitString("AT");
	EndCommand();
	taskEXIT_CRITICAL();
}

void Esp8266::Reset()
{
	taskENTER_CRITICAL();
	Uart::TransmitString("AT+RST");
	EndCommand();
	taskEXIT_CRITICAL();
}

void Esp8266::ConnectAp(char *ssid, char *password)
{
	taskENTER_CRITICAL();
	Uart::TransmitString("AT+CWJAP=\"");
	Uart::TransmitString(ssid);
	Uart::TransmitString("\",\"");
	Uart::TransmitString(password);
	Uart::TransmitString("\"");
	EndCommand();
	taskEXIT_CRITICAL();
	vTaskDelay(5000);
}

void Esp8266::QueryIp()
{
	taskENTER_CRITICAL();
	Uart::TransmitString("AT+CIFSR");
	EndCommand();
	taskEXIT_CRITICAL();
}

void Esp8266::ConnectTcpSocket(char *ip, char *port)
{
	taskENTER_CRITICAL();
	Uart::TransmitString("AT+CIPSTART=\"TCP\",\"");
	Uart::TransmitString(ip);
	Uart::TransmitString("\", ");
	Uart::TransmitString(port);
	EndCommand();
	taskEXIT_CRITICAL();
	vTaskDelay(1000);
}

void Esp8266::SendLineFromTcp(char *message)
{
	char s[5];
	taskENTER_CRITICAL();
	Uart::TransmitString("AT+CIPSEND=");
	itoa(strlen(message), s, 10);
	Uart::TransmitString(s);
	EndCommand();
	Uart::TransmitString(message);
	EndCommand();
	taskEXIT_CRITICAL();
	vTaskDelay(1000);
}

void Esp8266::DisconnectAp()
{
	taskENTER_CRITICAL();
	Uart::TransmitString("AT+CWQAP");
	EndCommand();
	taskEXIT_CRITICAL();
}

void Esp8266::Send(char *message)
{
	taskENTER_CRITICAL();
	Uart::TransmitString(message);
	EndCommand();
	taskEXIT_CRITICAL();
}

void Esp8266::EndCommand()
{
	Uart::TransmitByte(0x0D);
	Uart::TransmitByte(0x0A);
}

void Esp8266::vReadEsp8266(void *pvParameters)
{
	while(true)
	{
		char rcvd = Uart::ReceiveByte();
		if(rcvd == 0x0A || rcvd == 0x0D) //end of line
		{
			if(msgpos > 0)
			{
				messagePush[msgpos] = 0;
				msgpos = 0;
				xSemaphoreTake(semQueueLock, portMAX_DELAY);
				xQueueSend(messageQueue, messagePush, 0);
				xSemaphoreGive(semQueueLock);
			}
		}
		else
		{
			if(msgpos < BUFFER_SIZE - 1)
			{
				messagePush[msgpos++] = rcvd;
			}
		}
	}
}

void Esp8266::vPollMessage(void *pvParameters)
{
	while(true)
	{
		xSemaphoreTake(semQueueLock, portMAX_DELAY);

		if(uxQueueMessagesWaiting(messageQueue) > 0)
		{
			xQueueReceive(messageQueue, messagePop, 0);
			xSemaphoreGive(semQueueLock);
			messagePop[BUFFER_SIZE - 1] = 0;
			ProcessMessage(messagePop);
		}
		else
		{
			xSemaphoreGive(semQueueLock);
		}

		vTaskDelay(10);
	}
}

void Esp8266::ProcessMessage(const char *message)
{
	char *prefix = "+IPD,";
	int iPrefixLen = strlen(prefix);
	char *match = strstr(message, prefix);
	if(match != NULL)
	{
		match += iPrefixLen;
		int strlength = atoi(match);
		char strlenString[4];
		itoa(strlength, strlenString, 10);
		match += (strlen(strlenString) + 1);
		MessageReceived(match);
	}
}