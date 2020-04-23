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
#include "DIO_interface.h"
#include "LCD_interface.h"

/*Functions of tasks*/
void vidBlinkOne(void *);
void vidBlinkTwo(void *);
void vidOneForTwo(void *);
void vidPeriodicTask(void *);

TaskHandle_t BlinkOne_Handle = NULL;
TaskHandle_t BlinkTwo_Handle = NULL;
TaskHandle_t Periodic_Handle = NULL;

static u16 u16Count = 0;

typedef struct taskData_t {
	u8 u8ID;
	u8 * message;
} taskData_type;

struct taskData_t taskData_1 = {1, "Hello"};
struct taskData_t taskData_2 = {2, "Kilo"};
taskData_type taskData_3 = {3, "Hino"};

volatile u32 u32Count;

int main(void)
{
	/*System initialization*/

	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN2,DIO_OUTPUT);
	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN3,DIO_OUTPUT);


	LCD_vidInit();

	/*Creating two tasks with one function*/
	//xTaskCreate(vidOneForTwo,"one",200,(void*)&taskData_1, 1,&BlinkOne_Handle);

	//xTaskCreate(vidOneForTwo,"two",200,(void*)&taskData_2, 1,&BlinkTwo_Handle);

	/*One periodic task*/
	xTaskCreate(vidPeriodicTask,"periodic",200,(void*)&taskData_3,2,&Periodic_Handle);

	vTaskStartScheduler();


}

/*Idle task hook*/
void vApplicationIdleHook(void)
{
	u32Count++;
}

void vidPeriodicTask(void * pvParameters)
{
	taskData_type * taskData = NULL;
	taskData = (struct taskData *) pvParameters;

	TickType_t xLastTickTime;

	xLastTickTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastTickTime,1000);
		LCD_vidSendCommand(LCD_CLEAR_SCREEN);
		LCD_vidSendCommand(LCD_RETURN_HOME);
		LCD_vidWriteString(taskData->message);
		LCD_vidGoToXY(LCD_XPOS0,LCD_YPOS1);
		LCD_vidWriteNumber(u32Count);
	}

}

/*Continuous execution tasks function*/
void vidOneForTwo(void * pt)
{
	struct taskData_t * taskData =  (struct taskData*) pt;
	u8 * pcName = taskData->message; 
	const TickType_t xFrequency = 5000;
	TickType_t xLastWakeTime;
	if (taskData->u8ID == 3)
	{
		vTaskDelay(pdMS_TO_TICKS(2000));
	}

	xLastWakeTime = xTaskGetTickCount();
	while (1)
	{
		DIO_vidTogglePin(DIO_PORTD,DIO_PIN2);

		LCD_vidSendCommand(LCD_CLEAR_SCREEN);
		LCD_vidSendCommand(LCD_RETURN_HOME);
		LCD_vidWriteString(pcName);
	}

}
