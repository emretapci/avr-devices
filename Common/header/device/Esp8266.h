#ifndef _ESP8266_H_
#define _ESP8266_H_

#include <util/Util.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <semphr.h>

class Esp8266
{
public:
	//Initializes ESP8266, starts tasks.
	//MessageReceived: Callback to be called when a message is received.
	static void Initialize(void (*MessageReceived)(const char *));

	//Sends test command to ESP8266.
	static void Test();

	//Resets ESP8266.
	static void Reset();

	//Connects ESP8266 to an access point.
	static void ConnectAp(char *ssid, char *password);

	//Queries IP address from ESP8266.
	static void QueryIp();

	//Starts a client TCP request to the given IP and port.
	static void ConnectTcpSocket(char *ip, char *port);

	//Sends a message from the TCP connection.
	static void SendLineFromTcp(char *message);

	//Disconnects ESP8266 from access point.
	static void DisconnectAp();

private:
	static const uint8_t BUFFER_SIZE = 32;

	static char messagePush[BUFFER_SIZE], messagePop[BUFFER_SIZE];
	static uint8_t msgpos;
	static void (*MessageReceived)(const char *);

	static QueueHandle_t messageQueue;
	static SemaphoreHandle_t semQueueLock;

	static void Send(char *message);
	static void EndCommand();
	static void vReadEsp8266(void *pvParameters);
	static void vPollMessage(void *pvParameters);
	static void ProcessMessage(const char *message);
};

#endif
