/*
 * data_stream.c
 *
 *  Created on: Mar 25, 2023
 *      Author: Nguyen Tran
 */

#include <stdint.h>

uint8_t data_stream[] = "Memory to peripheral data transfer \ 
================================================== \
There are 2 modes: \
	1. Direct mode (without FIFO)\
	2. FIFO mode \
- FIFO mode: With threshold level software selectable between 1/4, 1/2 or 3/4 of the \
FIFO size \
- Direct mode: Each DMA request immediately initiates a transfer from/to the memory";


uint32_t data_len = sizeof(data_stream);
