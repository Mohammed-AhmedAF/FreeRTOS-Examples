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

void vidBlinkOne(void *);
void vidBlinkTwo(void *);
void vidOneForTwo(void *);

TaskHandle_t BlinkOne_Handle = NULL;
TaskHandle_t BlinkTwo_Handle = NULL;

static u16 u16Count = 0;

int main(void)
{
	/*System initialization*/

	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN2,DIO_OUTPUT);
	DIO_vidSetPinDirection(DIO_PORTD,DIO_PIN3,DIO_OUTPUT);


	LCD_vidInit();

	xTaskCreate(vidOneForTwo,"one",200,(void*)"Kilo", 3,&BlinkOne_Handle);

	xTaskCreate(vidOneForTwo,"two",200,(void*)"Hello", 3,&BlinkTwo_Handle);
	
	vTaskStartScheduler();


}

void vidOneForTwo(void * pt)
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
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
	}

}

void vidBlinkOne(void * pt)
{
	const TickType_t xFrequency = 1000;
	TickType_t xLastWakeTime;
	while (1)
	{
		DIO_vidTogglePin(DIO_PORTD,DIO_PIN2);
		LCD_vidSendCommand(LCD_RETURN_HOME);
		LCD_vidWriteNumber(u16Count);
		u16Count++;
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		LCD_vidSendCommand(LCD_CLEAR_SCREEN);
	}
}

void vidBlinkTwo(void * pt)
{
	TickType_t xLastWakeTime;
	while(1)
	{
		DIO_vidTogglePin(DIO_PORTD,DIO_PIN3);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime,500);
	}
}
