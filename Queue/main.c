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
	/*System initialization*/

	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN2,DIO_OUTPUT);
	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN3,DIO_OUTPUT);


	LCD_vidInit();

	/*Creating two tasks with one function*/
	//xTaskCreate(vidOneForTwo,"one",200,(void*)&taskData_1, 1,&BlinkOne_Handle);

	//xTaskCreate(vidOneForTwo,"two",200,(void*)&taskData_2, 1,&BlinkTwo_Handle);

	xQueue = xQueueCreate(5,sizeof(u8));

	/*One periodic task*/
	xTaskCreate(vidSender,"sender",200,(void*)&taskData_sender,2,&SenderHandle);
	xTaskCreate(vidReceiver,"receiver",200,(void*)&taskData_receiver,2,&ReceiverHandle);

	vTaskStartScheduler();


}


void vidSender(void * pvParameters)
{

	u8 val = 'a';
	u8 arr[] = {'h','e','l','l','o'};
	u8 i;
	while(1)
	{
		for (i = 0; i < 5; i++)
		{
		val = arr[i];
		xQueueSend(xQueue,&val,100);

		vTaskDelay(pdMS_TO_TICKS(2000));
		}
	}
}

void vidReceiver(void * pvParameters)
{

	u8 val;

	while (1)
	{
		xQueueReceive(xQueue,&val,100);
		LCD_vidWriteCharacter(val);
		
	}
}
