#include "sys.h"
#include "usart.h"	
#include "car.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
/*int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}*/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}
#endif 

int openmv[13];
int16_t OpenMV_Q;
int16_t OpenMV_W;
int16_t OpenMV_E;
int16_t OpenMV_R;
int16_t OpenMV_T;
int16_t OpenMV_Y;
int16_t OpenMV_U;
int16_t OpenMV_I;
int16_t isten;

unsigned int centre=0,left1=0,left2=0,right11=0,right2=0,left3=0,right3=0;

void Openmv_Data(void)//处理Openmv接收的数据
{
    OpenMV_Q=openmv[2];
    OpenMV_W=openmv[3];
	  OpenMV_E=openmv[4];
    OpenMV_R=openmv[5];
	  OpenMV_T=openmv[6];//centre
    OpenMV_Y=openmv[7];//centre
	  OpenMV_U=openmv[8];//left
    OpenMV_I=openmv[9];//left
	  isten = openmv[10];
}
void openmv_control(void)
{
	 
	if(OpenMV_T&&OpenMV_Y) //中值 直行
	{		
		centre=1;
	}
	else
	{
		centre=0;
	}
	if(OpenMV_T&&OpenMV_R)//左微偏 微向右调
	{
		right11=1;
		if(OpenMV_E||OpenMV_W)//左偏 向右调
		{
			right2=1;
		}
	}
	if(OpenMV_W)  //偏离赛道 
	{
		right3=1;
	//	right1=0;
	}
	if(OpenMV_Y&&OpenMV_U)//右微偏 微调		
	{
		left1=1;
		if(OpenMV_U||OpenMV_I)
		{
			left2=1;
		}
	}
	if(OpenMV_I)	//偏离赛道 
	{
		left3=1;
		//left1=0;
	}
	if(OpenMV_E&&OpenMV_R&&OpenMV_T&&OpenMV_Y)//	十字路口
	{
		
	}

}

void deal(void)
{
if(centre==1&&right11==0&&left1==0)//正常直行
	{
	GO();
	}
if(right11==1&&right2==1)					//向右微转
	{
		right();
	}
if(centre==0&&right2==1)					//向右转
	{
right1();
	}
if(right3==1)											//右转大弯
	{
right();
	} 
if(left1==1&&left2==1)					//向左微转
	{
left();
	}
if(centre==0&&left2==1)					  //向左转
	{
left();
	}
if(left3==1)											//左转大弯
	{
left();
	}
}
u8 i;
void Openmv_Receive_Data(int16_t data)//接收Openmv传过来的数据
{
	static u8 state = 0;
	if(state==0&&data==0xb3)//第一个帧头
	{
		
		state=1;
		openmv[0]=data;
		 
	}
	else if(state==1&&data==0xb3)//第二个帧头
	{
		state=2;
		openmv[1]=data;
	}
	else if(state==2)//第一个有效数据
	{
		state=3;
		openmv[2]=data;
	}
	else if(state==3)//第二个有效数据
	{
		state = 4;
		openmv[3]=data;
	}
  else if(state==4)//第三个有效数据
	{
		state = 5;
		openmv[4]=data;
	} 
	  else if(state==5)
	{
		state = 6;
		openmv[5]=data;
	} 
	  else if(state==6)
	{
		state = 7;
		openmv[6]=data;
	} 
	  else if(state==7)
	{
		state = 8;
		openmv[7]=data;
	} 
	  else if(state==8)
	{
		state = 9;
		openmv[8]=data;
	} 
	  else if(state==9)
	{
		state = 10;
		openmv[9]=data;
	} 
	  else if(state==10)
	{
		state = 11;
		openmv[10]=data;
	} 
	else if(state==11&&data==0x0b)		//检测是否接受到结束标志，检测接收帧尾
	{
		state=12;
		openmv[11]=data;
	}
	else if(state==12)
	{
        		if(data == 0x0a)
      		  {
	 	// led4_on;
           	 state = 0;
           	 openmv[11]=data;
         		  // Openmv_Data();
       		 }
       		 else if(data != 0x0a)
      		  {
            	state = 0;
           	 for(i=0;i<13;i++)
          	  {
              		  openmv[i]=0x00;
           	 }           
       	}    
	else
		{
			state = 0;
            for(i=0;i<13;i++)
            {
                openmv[i]=0x00;
            }
		}
}

}
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

/*void USART_Send_END(void)//发送结束符（串口屏）
{
  USART1_Send_Byte(0xFF);
	USART1_Send_Byte(0xFF);
	USART1_Send_Byte(0xFF);	
}

void USART1_Send_Byte(u8 ch)	   			//发送字节		   
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, (u8)ch);
}
void USART1_Send_Len_Str(u8 *str, u8 len)//发送字符
{
	u8 i;
	for(i=0; i<len; i++)
	USART1_Send_Byte(*str ++);
}
**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
/*void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	
		u8 temp; 	
		USART_ClearFlag(USART1, USART_IT_RXNE); //清除标志位；			
		temp=USART1->DR;
		//led1_on;
		Openmv_Receive_Data(temp);
		Openmv_Data();	
	}  											 
}*/ 

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		  Res =USART_ReceiveData(USART1);	//读取接收到的数据
		  if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}	
		}		 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif	

