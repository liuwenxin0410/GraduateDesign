#include "motor.h"

//左右轮电机PWM变量

int motorLeft     = 0;
int motorRight    = 0;         

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
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
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
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

