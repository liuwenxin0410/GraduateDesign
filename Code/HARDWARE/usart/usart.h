#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>

void My_USART2_Init(void);
void USART2_IRQHandler(void);
int fputc(int ch, FILE *f);

extern u8 res;

#endif /* __USART1_H */
