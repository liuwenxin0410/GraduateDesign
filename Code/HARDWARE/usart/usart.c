#include "usart.h"
#include <stdarg.h>

u8 res=0;  				  //设置全局变量

void My_USART2_Init(void)  
{  
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO端口使能  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//串口端口使能  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;  //TXD->PA2
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;    //RXD->PA3
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=19200;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART2,&USART_InitStrue);
      
    USART_Cmd(USART2,ENABLE);					//使能串口2  
      
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
  
void USART2_IRQHandler(void)  
{  

     if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
 {  
     res= USART_ReceiveData(USART2); 	 
     USART_SendData(USART2,res);     //串口2发送数据给蓝牙模块接收，也就是手机app接收到的数据
  }  
}  

int fputc(int ch, FILE *f)
{
	 USART_SendData(USART1,(uint8_t)ch);
   while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==0);
 	 return (ch);
}


