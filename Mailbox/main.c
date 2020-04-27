/*
 * First RTOS.c
 *
 * Created: 4/13/2020 9:28:13 PM
 * Author : Mohammed
 * Example: vTaskDelayUntil
 */
#include <stdio.h>
#include "std_types.h"
#include "Macros.h"
#include "Std_Types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "DIO_interface.h"
#include "LCD_interface.h"

/*Functions of tasks*/
void vidSender(void * pvParameters);
void vidReceiver(void * pvParameters);

TaskHandle_t SenderHandle = NULL;
TaskHandle_t ReceiverHandle = NULL;

static u16 u16Count = 0;

typedef struct taskData_t {
	u8 u8ID;
	u8 * message;
} taskData_type;

struct taskData_t taskData_sender = {1, "Hello"};
struct taskData_t taskData_receiver = {2, "Kilo"};

volatile u32 u32Count;

QueueHandle_t xQueue;

int main(void)
{

	LCD_vidInit();

	xQueue = xQueueCreate(1,sizeof(u8));

	/*One periodic task*/
	xTaskCreate(vidSender,"sender",200,(void*)&taskData_sender,2,&SenderHandle);
	xTaskCreate(vidReceiver,"receiver",200,(void*)&taskData_receiver,2,&ReceiverHandle);

	vTaskStartScheduler();


}


void vidSender(void * pvParameters)
{

	u8 val = 'a';
	struct taskData_t * senderData = (struct taskData_t *) pvParameters;
	u8 i;
	while(1)
	{
		for (i = 0; i < 5; i++)
		{
			val = senderData->message[i];
			xQueueOverwrite(xQueue,&val);

		}
	}
}

void vidReceiver(void * pvParameters)
{

	u8 val;
	u8 res;
	while (1)
	{
		res = xQueueReceive(xQueue,&val,2000);
		if (res == pdTRUE)
		{
			LCD_vidWriteCharacter(val);
		}
		else
		{
			continue;
		}
	}
}
