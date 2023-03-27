/*
 * it.c
 *
 *  Created on: Mar 26, 2023
 *      Author: Nguyen Tran
 */


#include "main.h"
#include "it.h"



void EXTI0_IRQHandler(void)
{
	TurnOn_UserLed();
	UART_Send_Request_DMA();

	Clear_EXTI_Pending_Bit();
}



static void Clear_EXTI_Pending_Bit(void)
{
	if((EXTI->PR & (1 << 0)))
	{
		EXTI->PR &= ~(1 << 0);
	}
}


static void TurnOn_UserLed(void)
{
	GPIOG -> ODR |= USER_LED;
}



static void UART_Send_Request_DMA(void)
{
	USART1->CR3 |= DMA_ENABLE_TRANSMITTER;
}
