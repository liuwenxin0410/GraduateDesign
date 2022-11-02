#include "sys.h"
#include "delay.h"
#include "led.h"
#include "PWM.h"
#include "car.h"
#include "usart.h"
#include <string.h>
#include <math.h>

//char source[10];  //�洢���ڻ�������

//�����������������ͬ��������ƫ���Ҫ���н���
#define speed_left 100 //��������
#define  speed_right 100  //��������

u16 num=50;  //�洢�ַ�ת���ε�����

#define R   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����1(KEY1)
#define M   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����2(KEY2)
#define L   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����3(KEY3)

int16_t shuju;

//��������ƫ�������ٺ���
void run(void)
{
	  GO();
		if(num<100)  //��⵽��ƫ
		{
					  //TIM_SetCompare1(TIM4,speed_right-num);   //���ּ���
		  TIM_SetCompare1(TIM4,speed_right+num);   //���ּ���
	    TIM_SetCompare2(TIM4,speed_left+num);   //���ּ���
		}
		if(num>=100)  //��⵽��ƫ
		{
			TIM_SetCompare1(TIM4,speed_right+num-100);  //���ּ���
	    TIM_SetCompare2(TIM4,speed_left-num-100);  //���ּ���
		}
}


/*int simple_atoi(char *source) //�ַ���ת����
{
	int length = strlen(source); //���㳤��
	int sum = 0;
	int i;
	
		for(i=0;i<length;i++) 
	  {
		  sum += (source[i]-'0') * pow(10,length-1-i); //�ַ�ת���ֲ��ϲ�
	  }
	
	for(i=0;i<length;i++) source[i]=0;//��������
	return sum;
}*/

 void DELAY(unsigned int xms)  //��������ԭ�ӵ���ʱ������protues�����д������⣬���Բ�����д��ʱ����ԼΪ10ms����ʱ
{
	unsigned int j;
	for( ;xms>0;xms--)
	 for(j=1400;j>0;j--);
}

/*void huidu_init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	//��ʼʱ��GPIOA/GPIOE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//����Ϊ��������
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

}
u8 huidu_Scan(void)  //������
{	 
  

//		if(M==0)return num=50;
//		else if(R==0)return num=20;
//		else if(L==1)return num=10;
	 if(R==1&&M==1&&L==1)num=99; 
   else	num=0;
 	return num;// �ް�������
}*/
void shiijan(int time)
{
	int k;
	for(k=0;k<time;k++);
}

int main(void)
{			 
  u16 t;  
  u16 len;	
	delay_init();  //��ʼ����ʱ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ19200
	GPIO_init_CAR();
	LED_Init();
 // huidu_init();  
	//��ʼ������
		TIM4_PWM_Init(19999,83);  //��ʼ��PWM  8k hz
		TIM_SetCompare1(TIM4,speed_left);  
	  TIM_SetCompare2(TIM4,speed_right);  
	
	while(1)
    {
			 /*if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				
				for(t=0;t<len;t++)
			  {
				  source[t]=USART_RX_BUF[t];  //��BUF�Ĵ�������ת�Ƶ�source����
			  }
				num = simple_atoi(source);  //��������
				USART_RX_STA=0;
			}	*/
			/*huidu_Scan();*/
     if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				
				for(t=0;t<len;t++)
			  {
				  shuju=USART_RX_BUF[t];  //��BUF�Ĵ�������ת�Ƶ�source����
					Openmv_Receive_Data(shuju);		
			  }
				
				Openmv_Data();	  //��������
				USART_RX_STA=0;
			}
     openmv_control();			
			shiijan(5000);
			run();  //��������ƫ��

	}
}
