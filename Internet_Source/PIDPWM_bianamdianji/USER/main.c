/***********************************************
��˾����Ȥ�Ƽ�(��ݸ)���޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com
�汾V1.0
�޸�ʱ�䣺2020-06-30
Allrightsreserved
***********************************************/

#include "delay.h"
#include "usart.h"			
#include "TB6612.h"
#include "motorencoder.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "key.h"

int   TargetVelocity=100, Encoder, PWM;  //Ŀ���ٶȡ�������������PWM���Ʊ���
float Velcity_Kp=20,  Velcity_Ki=5,  Velcity_Kd; //����ٶ�PID����
int   MortorRun;  //���������Ʊ�־λ

/**************************************************************************
�������ܣ�OLED��ʾ����ʾ����
��ڲ�������
����  ֵ����
**************************************************************************/
void Oled_Show(void)
{  
		OLED_Refresh_Gram(); //ˢ����ʾ��
		OLED_ShowString(00,00,"VelocityFeedback"); //�ٶȱջ�����
			
		//��ʾĿ���ٶȣ�������
		OLED_ShowString(00,10,"Target_V :"); 		
		if(TargetVelocity>=0)
		{
			OLED_ShowString(80,10,"+");
			OLED_ShowNumber(90,10,TargetVelocity,5,12);
		}
		else
		{
			OLED_ShowString(80,10,"-");
			OLED_ShowNumber(90,10,-TargetVelocity,5,12);
		}
		
		//��ʾ��ǰ�ٶȣ���������������������
		OLED_ShowString(00,20,"Current_V:"); 		
		if(Encoder>=0)
		{
			OLED_ShowString(80,20,"+");
			OLED_ShowNumber(90,20,Encoder,5,12);
		}
		else
		{
			OLED_ShowString(80,20,"-");
			OLED_ShowNumber(90,20,-Encoder,5,12);
		}
		
		//��ʾ�ٶȿ���ֵ����PWM��������
		OLED_ShowString(00,30,"PWM      :"); 		
		if(PWM>=0)
		{
			OLED_ShowString(80,30,"+");
			OLED_ShowNumber(90,30,PWM,5,12);
		}
		else
		{
			OLED_ShowString(80,30,"-");
			OLED_ShowNumber(90,30,-PWM,5,12);
		}
}

/**************************************************************************
�������ܣ�������
��ڲ�������
����  ֵ����
**************************************************************************/
int main(void)
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�ж����ȼ�����
	 delay_init();                //�ӳٺ�����ʼ��
	 JTAG_Set(JTAG_SWD_DISABLE);  //�ر�JTAG���ܿ���OLED��ʾ�� 
	 LED_Init();                  //LED�Ƴ�ʼ��
   OLED_Init();                 //OLED��ʾ����ʼ��
   uart_init(9600);           //���ڳ�ʼ��	 
   MotorEncoder_Init();	        //��������ʼ�� ʹ�ö�ʱ��4
   TB6612_Init(7199, 0);        //������������ʼ�� ʹ�ö�ʱ��3 
	 EncoderRead_TIM2(7199, 99);  //10ms��ȡһ�α�����(��100HZ)��������ٱ�Ϊ20����������������13��AB˫����ϵõ�4��Ƶ��
	                              //��ת1Ȧ����������Ϊ20*13*4=1040�����ת��=Encoder*100/1040r/s ʹ�ö�ʱ��2
	 delay_ms(2000);              //�ӳٵȴ���ʼ�����
	 while(1)
	  {		
			delay_ms(5);
			LED=0;    //LED����˸
      if(KEY_Scan())MortorRun=!MortorRun; //���°���MortorRunȡ��
								
			Oled_Show(); //OLED��ʾ����ʾ����
//			PWM=Velocity_FeedbackControl(TargetVelocity, Encoder); //�ٶȻ��ջ�����
//				
//				SetPWM(PWM); //����PWM
      //���ڴ�ӡĿ���ٶȡ���ǰ�ٶȡ�ת��			
			printf("TargetVelocity��%d\r\n",TargetVelocity);
			printf("Encoder��%d\r\n",Encoder);
			printf("ת�٣�%.3fr/s\r\n", Encoder/1.04);
	  }
}

