/***********************************************
公司：轮趣科技(东莞)有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com
版本V1.0
修改时间：2020-06-30
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

int   TargetVelocity=100, Encoder, PWM;  //目标速度、编码器读数、PWM控制变量
float Velcity_Kp=20,  Velcity_Ki=5,  Velcity_Kd; //相关速度PID参数
int   MortorRun;  //允许电机控制标志位

/**************************************************************************
函数功能：OLED显示屏显示内容
入口参数：无
返回  值：无
**************************************************************************/
void Oled_Show(void)
{  
		OLED_Refresh_Gram(); //刷新显示屏
		OLED_ShowString(00,00,"VelocityFeedback"); //速度闭环控制
			
		//显示目标速度，分正负
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
		
		//显示当前速度，即编码器读数，分正负
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
		
		//显示速度控制值，即PWM，分正负
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
函数功能：主函数
入口参数：无
返回  值：无
**************************************************************************/
int main(void)
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组
	 delay_init();                //延迟函数初始化
	 JTAG_Set(JTAG_SWD_DISABLE);  //关闭JTAG才能开启OLED显示屏 
	 LED_Init();                  //LED灯初始化
   OLED_Init();                 //OLED显示屏初始化
   uart_init(9600);           //串口初始化	 
   MotorEncoder_Init();	        //编码器初始化 使用定时器4
   TB6612_Init(7199, 0);        //电机驱动外设初始化 使用定时器3 
	 EncoderRead_TIM2(7199, 99);  //10ms读取一次编码器(即100HZ)，电机减速比为20，霍尔编码器精度13，AB双相组合得到4倍频，
	                              //则转1圈编码器读数为20*13*4=1040，电机转速=Encoder*100/1040r/s 使用定时器2
	 delay_ms(2000);              //延迟等待初始化完成
	 while(1)
	  {		
			delay_ms(5);
			LED=0;    //LED灯闪烁
      if(KEY_Scan())MortorRun=!MortorRun; //按下按键MortorRun取反
								
			Oled_Show(); //OLED显示屏显示内容
//			PWM=Velocity_FeedbackControl(TargetVelocity, Encoder); //速度环闭环控制
//				
//				SetPWM(PWM); //设置PWM
      //串口打印目标速度、当前速度、转速			
			printf("TargetVelocity：%d\r\n",TargetVelocity);
			printf("Encoder：%d\r\n",Encoder);
			printf("转速：%.3fr/s\r\n", Encoder/1.04);
	  }
}

