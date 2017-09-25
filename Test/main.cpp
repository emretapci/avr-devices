#include <freertos.h>
#include <task.h>

#include <util/Util.h>
#include <device/Esp8266.h>
#include <interface/Uart.h>

static void vMainTask(void *pvParameters);
static void ProcessMessage(const char *message);

extern "C"
{
	void vApplicationIdleHook();
}

int main()
{
	Esp8266::Initialize(ProcessMessage);

	xTaskCreate(vMainTask, "vMainTask", configMINIMAL_STACK_SIZE, 0, tskIDLE_PRIORITY + 3, 0);

	vTaskStartScheduler();
	return 0;
}

void vMainTask(void *pvParameters)
{
	Esp8266::ConnectAp("topitop3", "topitoptopitop3");
	Esp8266::ConnectTcpSocket("192.168.1.12", "4444");
	Esp8266::SendLineFromTcp("abcde");
	Esp8266::SendLineFromTcp("emre tapci");

	Gpio pd5 = GpioUtil::CreateGpio('D', 5);
	GpioUtil::MakeOutput(pd5);
	GpioUtil::SetLevel(pd5, GpioUtil::High);

	while(true);
}

void ProcessMessage(const char *message)
{
}

void vApplicationIdleHook()
{
}
