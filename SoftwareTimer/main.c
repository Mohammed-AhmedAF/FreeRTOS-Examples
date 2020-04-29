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
#include "timers.h"
#include "task.h"
#include "queue.h"
#include "DIO_interface.h"
#include "LCD_interface.h"

/*Functions of tasks*/
void vidBlinkOnTimer(TimerHandle_t);
void vidBlinkOnTask(void *);

TaskHandle_t BlinkTaskHandle = NULL;
TimerHandle_t xPeriodicTimer = NULL;


volatile u32 u32Count;


int main(void)
{

	/*Pin initialization*/
	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN0,DIO_OUTPUT);
	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN1,DIO_OUTPUT);

	LCD_vidInit();

	xTaskCreate(vidBlinkOnTask,"Blink on task",200,(void*) 1,1,&BlinkTaskHandle);

	xPeriodicTimer = xTimerCreate("Periodic",500,pdTRUE,0,vidBlinkOnTimer);

	/*One periodic task*/

	xTimerStart(xPeriodicTimer,0);

	vTaskStartScheduler();


}


void vidBlinkOnTask(void * pv)
{
	while(1)
	{
		DIO_vidTogglePin(DIO_PORTD,DIO_PIN1);
		vTaskDelay(1000);
	}
}

void vidBlinkOnTimer(TimerHandle_t xTimer)
{

		DIO_vidTogglePin(DIO_PORTD,DIO_PIN0);	
}
