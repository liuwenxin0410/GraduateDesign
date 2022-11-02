#include "TB6612.h"  

/**************************************************************************
函数功能：TB6612初始化函数
入口参数：定时器3计数上限 定时器3预分频系数
返回  值：无
**************************************************************************/
void TB6612_Init(int arr, int psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体	
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义一个PWM输出的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟，GPIOB挂载在APB2时钟下，在STM32中使用IO口前都要使能对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能通用定时器3时钟
	
	//TB6612控制方向引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //引脚输入输出模式为推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //引脚输出速度为50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //根据上面设置好的GPIO_InitStructure参数，初始化引脚
	
	//GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13); //初始化设置引脚低电平

	//TB6612PWM输出引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;//引脚0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出模式，定时器功能为B1引脚复用功能
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //定义该引脚输出速度为50MHZ
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化引脚GPIOB1
	
	TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时，定时器从0开始计数，计数超过到arr时触发定时中断服务函数
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数，决定每一个计数的时长
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式：向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用，默认TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStrue); //根据TIM_TimeBaseInitStrue的参数初始化定时器TIM3
	
	TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM模式1，当定时器计数小于TIM_Pulse时，定时器对应IO输出有效电平
	TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高电平
	TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能PWM输出
	TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OC4Init(TIM3, &TIM_OCInitTypeStrue); //根TIM_OCInitTypeStrue参数初始化定时器3通道4

  //TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE 主输出使能

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable); //CH4预装载使能  使能后改变TIM_Pulse(即PWM)的值立刻生效，不使能则下个周期生效
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //TIM3预装载使能
	
	TIM_Cmd(TIM3, ENABLE); //使能定时器TIM3
}

/**************************************************************************
函数功能：设置TIM3通道4PWM值
入口参数：PWM值
返回  值：无
**************************************************************************/
void SetPWM(int pwm)
{
  if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) 正转
  {
		PBout(13)=0; //BIN1=0
		PBout(12)=1; //BIN2=1
		TIM3->CCR4=pwm;
		TIM_SetCompare4(TIM3, pwm);
  }
  else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) 反转
  {
		PBout(13)=1; //BIN1=1
		PBout(12)=0; //BIN2=0
		TIM3->CCR4=-pwm;
		TIM_SetCompare4(TIM3, -pwm);
  }
}

