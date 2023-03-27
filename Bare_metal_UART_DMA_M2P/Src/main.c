/*
 * main.c
 *
 *  Created on: Mar 26, 2023
 *      Author: Nguyen Tran
 */

#include "main.h"

static void Uart1_Set_Baudrate(uint32_t peripher_clk, uint32_t baudrate);
static uint16_t Caculate_Uart_Baudrate(uint32_t peripher_clk, uint32_t baudrate);

char dma_data[] = "Some DMA data...\r\n";

int main(void)
{
	Button_Init();
	UserLed_Init();
	UART1_Init();
	DMA2_Init();
	UART1_Transmit_Data();


	while(1);
	return 0;
}


void Button_Init(void)
{
	/* User button connect to PA0 */
	/* Enable the clock for GPIOA, SYSCFG */
	RCC->AHB1ENR |= GPIOA_CLK_EN;
	RCC->APB2ENR |= SYSCFG_CLK_EN;

	/* Configure mode input */
	GPIOA->MODER &= GPIOA_MODE_INPUT;

	/* Configure the edge detection on PA0 */
	EXTI->FTSR |= FALLING_EDGE_LINE0_ENBALE;

	/* Enable the interrupt over PA0 */
	EXTI->IMR |= IRQ_LINE0_ENBALE;

	/* Configure the SYSCFG CR1*/
	SYSCFG->EXTICR[0] &= ~(0xF << 0);

	/* Enable the IRQ relate to PA0 in NVIC of the Cortex-M4 */
	NVIC->ISER[0] |= (1U << EXTI0_IRQn);
}


void UserLed_Init(void)
{
	/* Enable clock */
	RCC -> AHB1ENR |= GPIOG_CLK_EN;

	/* Set mode output */
	GPIOG -> MODER |= (1U << 26);
	GPIOG -> MODER &= ~(1U << 27);
}



void UART1_Init(void)
{
	/***Configure UART_TX and RX GPIO pin PA9 PA10***/
	/* Enable clock */
	RCC->AHB1ENR |= GPIOA_CLK_EN;

	/* UART TX pin PA9 */
	/* Set AF mode (AF07)*/
	GPIOA->MODER &= ~(0x3 << 18); //clear
	GPIOA->MODER |= GPIOA_PIN9_MODE_AF;

	GPIOA->AFR[1] &= ~(0xF << 4); //clear
	GPIOA->AFR[1] |= GPIOA_PIN9_AF7; //AFRHigh register

	/* Pull up/ Pull down control for UART1 TX (PA9) */
	GPIOA->PUPDR |= GPIOA_PIN9_PU; //Pull-up

	/* UART RX pin PA10 */
	/* Set AF mode (AF07)*/
	GPIOA->MODER &= ~(0x3 << 20); //clear
	GPIOA->MODER |= GPIOA_PIN10_MODE_AF;

	GPIOA->AFR[1] &= ~(0xF << 8); //clear
	GPIOA->AFR[1] |= GPIOA_PIN10_AF7; //AFRHigh register

	/* Pull up/ Pull down control for UART1 RX (PA10) */
	GPIOA->PUPDR |= GPIOA_PIN10_PU; //Pull-up

	/***Configure UART***/
	/* Enable clock UART1 */
	RCC->APB2ENR |= UART1_CLK_EN;
	/* Configure baudrate */
	Uart1_Set_Baudrate(APB2_CLK, UART_BAUDRATE);

	/* Configure transfer direction */
	USART1->CR1 = UART1_CR1_TE; //clear anything and set TE is 1
	/* Enable UART module */
	USART1->CR1 |= UART1_CR1_UE;
}



static void Uart1_Set_Baudrate(uint32_t peripher_clk, uint32_t baudrate)
{
	USART1->BRR = Caculate_Uart_Baudrate(peripher_clk, baudrate);
}


static uint16_t Caculate_Uart_Baudrate(uint32_t peripher_clk, uint32_t baudrate)
{
	return ((peripher_clk + (baudrate/2U))/baudrate);
}


void UART1_Transmit_Data(void)
{
	char somedata[] = "Application is running...\r\n";

	uint32_t data_len = sizeof(somedata);

	/* Write to the transmit data register*/
	for( uint32_t i = 0 ; i < data_len ; i++)
	{
		/* Make sure transmit data register is empty */
		while(!(USART1->SR & UART1_SR_TXE)); // check TXE field
		USART1->DR = somedata[i];
	}
}


void DMA2_Init(void)
{
	/* Identify the stream and channel mapped with peripheral:
	   	   UART1_TX Stream 7, Channel 4 */
	/* Enable clock for DMA2 */
	RCC->AHB1ENR |= DMA2_CLK_EN;

	/* Select channel */
	DMA2_Stream7->CR &= ~(0x7 << 25); //clear CHSEL field
	DMA2_Stream7->CR |= DMA2_CHANNEL_4;
	/* Source address */
	DMA2_Stream7->M0AR = (uint32_t)dma_data;
	/* Destination address */
	DMA2_Stream7->PAR = (uint32_t)&USART1->DR;
	/* Number of data items to send */
	uint32_t len = sizeof(dma_data);
	DMA2_Stream7->NDTR = len;
	/* The direction of data transfer m2p, p2m, m2m	 */
	DMA2_Stream7->CR |= DMA2_DIRECTION_M2P;
	/* The source and destination data width */
	DMA2_Stream7->CR &= ~(0x3U << 13); //clear MSIZE field (Byte)
	DMA2_Stream7->CR &= ~(0x3U << 11); //clear PSIZE field (Byte)
	/* Enable memory auto increment */
	DMA2_Stream7->CR |= DMA2_AUTO_INC_ADDR;
	/* Direct mode or FIFO mode	*/
	DMA2_Stream7->FCR |= DMA2_DIRECT_MODE_DIS; //Disable direct mode
	/* Select the FIFO threshold */
	DMA2_Stream7->FCR &= ~(0x3U << 0); //clear
	DMA2_Stream7->FCR |= DMA2_FULL_FIFO_MODE; //Full FIFO
	/* Enable the stream */
	DMA2_Stream7->CR |= DMA2_STREAM_ENABLE;
}




