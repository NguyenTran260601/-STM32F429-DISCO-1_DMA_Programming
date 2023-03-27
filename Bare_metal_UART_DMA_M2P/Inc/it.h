/*
 * it.h
 *
 *  Created on: Mar 26, 2023
 *      Author: Nguyen Tran
 */

#ifndef IT_H_
#define IT_H_


#define DMA_ENABLE_TRANSMITTER	(0x1U << 7)

static void Clear_EXTI_Pending_Bit(void);
static void TurnOn_UserLed(void);
static void UART_Send_Request_DMA(void);

#endif /* IT_H_ */
