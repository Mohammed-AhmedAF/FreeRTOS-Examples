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

void vidShowInfo(void *);

TaskHandle_t BlinkOne_Handle = NULL;

static u16 u16Count = 0;

int main(void)
{
	/*System initialization*/

	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN2,DIO_OUTPUT);

	LCD_vidInit();

	/*Creating two tasks with one function*/
	xTaskCreate(vidShowInfo,"one",200,"Task priority ", 3,&BlinkOne_Handle);

	
	vTaskStartScheduler();


}

void vidShowInfo(void * pt)
{
	u8 * pcName = (u8 *) pt;
	
	const TickType_t xFrequency = 2000;
	TickType_t xLastWakeTime;
	vTaskDelay(2000);
	while (1)
	{
		DIO_vidTogglePin(DIO_PORTD,DIO_PIN2);
	
		LCD_vidSendCommand(LCD_CLEAR_SCREEN);
		LCD_vidSendCommand(LCD_RETURN_HOME);
		LCD_vidWriteString(pcName);
		LCD_vidWriteNumber(uxTaskPriorityGet(BlinkOne_Handle));
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}

}

