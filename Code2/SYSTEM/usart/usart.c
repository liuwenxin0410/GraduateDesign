#include "sys.h"
#include "usart.h"	
#include "car.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
/*int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
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

void Openmv_Data(void)//����Openmv���յ�����
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
	 
	if(OpenMV_T&&OpenMV_Y) //��ֵ ֱ��
	{		
		centre=1;
	}
	else
	{
		centre=0;
	}
	if(OpenMV_T&&OpenMV_R)//��΢ƫ ΢���ҵ�
	{
		right11=1;
		if(OpenMV_E||OpenMV_W)//��ƫ ���ҵ�
		{
			right2=1;
		}
	}
	if(OpenMV_W)  //ƫ������ 
	{
		right3=1;
	//	right1=0;
	}
	if(OpenMV_Y&&OpenMV_U)//��΢ƫ ΢��		
	{
		left1=1;
		if(OpenMV_U||OpenMV_I)
		{
			left2=1;
		}
	}
	if(OpenMV_I)	//ƫ������ 
	{
		left3=1;
		//left1=0;
	}
	if(OpenMV_E&&OpenMV_R&&OpenMV_T&&OpenMV_Y)//	ʮ��·��
	{
		
	}

}

void deal(void)
{
if(centre==1&&right11==0&&left1==0)//����ֱ��
	{
	GO();
	}
if(right11==1&&right2==1)					//����΢ת
	{
		right();
	}
if(centre==0&&right2==1)					//����ת
	{
right1();
	}
if(right3==1)											//��ת����
	{
right();
	} 
if(left1==1&&left2==1)					//����΢ת
	{
left();
	}
if(centre==0&&left2==1)					  //����ת
	{
left();
	}
if(left3==1)											//��ת����
	{
left();
	}
}
u8 i;
void Openmv_Receive_Data(int16_t data)//����Openmv������������
{
	static u8 state = 0;
	if(state==0&&data==0xb3)//��һ��֡ͷ
	{
		
		state=1;
		openmv[0]=data;
		 
	}
	else if(state==1&&data==0xb3)//�ڶ���֡ͷ
	{
		state=2;
		openmv[1]=data;
	}
	else if(state==2)//��һ����Ч����
	{
		state=3;
		openmv[2]=data;
	}
	else if(state==3)//�ڶ�����Ч����
	{
		state = 4;
		openmv[3]=data;
	}
  else if(state==4)//��������Ч����
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
	else if(state==11&&data==0x0b)		//����Ƿ���ܵ�������־��������֡β
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
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

/*void USART_Send_END(void)//���ͽ���������������
{
  USART1_Send_Byte(0xFF);
	USART1_Send_Byte(0xFF);
	USART1_Send_Byte(0xFF);	
}

void USART1_Send_Byte(u8 ch)	   			//�����ֽ�		   
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, (u8)ch);
}
void USART1_Send_Len_Str(u8 *str, u8 len)//�����ַ�
{
	u8 i;
	for(i=0; i<len; i++)
	USART1_Send_Byte(*str ++);
}
**************************************************************************
�������ܣ�����1�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
/*void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{	
		u8 temp; 	
		USART_ClearFlag(USART1, USART_IT_RXNE); //�����־λ��			
		temp=USART1->DR;
		//led1_on;
		Openmv_Receive_Data(temp);
		Openmv_Data();	
	}  											 
}*/ 

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		  Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		  if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}	
		}		 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}
#endif	

