#include "motor.h"

//�����ֵ��PWM����

int motorLeft     = 0;
int motorRight    = 0;         

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motorLeft,int motorRight)
{
	if(motorLeft>0)     AIN2=0, AIN1=1;
	else 	            AIN2=1,	AIN1=0;
	PWMA=myabs(motorLeft);
	if(motorRight>0)	BIN1=0,	BIN2=1;
	else                BIN1=1,	BIN2=0;
	PWMB=myabs(motorRight);	
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}

