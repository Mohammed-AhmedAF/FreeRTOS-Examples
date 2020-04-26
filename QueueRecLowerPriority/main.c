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
TaskHandle_t SenderTwoHandle = NULL;

static u16 u16Count = 0;

typedef struct taskData_t {
	u8 u8ID;
	u8 * message;
} taskData_type;

struct taskData_t taskData_sender = {1, "Hello!"};
struct taskData_t taskData_sender2 = {1, "Akimov"};
struct taskData_t taskData_receiver = {2, "Kilo"};

volatile u32 u32Count;

QueueHandle_t xQueue;

int main(void)
{
	/*System initialization*/

	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN2,DIO_OUTPUT);
	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN3,DIO_OUTPUT);


	LCD_vidInit();

	xQueue = xQueueCreate(5,sizeof(u8));

	/*Sender tasks*/
	xTaskCreate(vidSender,"sender",200,(void*)&taskData_sender,3,&SenderHandle);
	xTaskCreate(vidSender,"sender",200,(void*)&taskData_sender2,3,&SenderTwoHandle);
	
	/*Receiver tasks*/
	xTaskCreate(vidReceiver,"receiver",200,(void*)&taskData_receiver,1,&ReceiverHandle);

	vTaskStartScheduler();


}


void vidSender(void * pvParameters)
{

	u8 val = 'a';
	struct taskData_t * senderData = (struct taskData_t *) pvParameters;
	u8 i;
	while(1)
	{
		for (i = 0; i < 6; i++)
		{
		val = senderData->message[i];
		xQueueSend(xQueue,&val,100);

		}
	}
}

void vidReceiver(void * pvParameters)
{

	u8 val;
	u8 u8Res;
	while (1)
	{
		u8Res = xQueueReceive(xQueue,&val,100);
		if (u8Res == pdPASS)
		{
			LCD_vidWriteCharacter(val);
		}
		else 
		{
			continue;
		}
		
	}
}
