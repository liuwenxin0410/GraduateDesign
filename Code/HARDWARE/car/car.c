#include "car.h"


//extern u16 num=0;  //存储字符转整形的数据
#define speed_left 200 //左轮速率
#define  speed_right 200  //右轮速率
void GO(void)  //前进
{
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;
	TIM_SetCompare1(TIM4,speed_right);   //左轮减速
		  //TIM_SetCompare1(TIM4,speed_right+num);   //左轮减速
	TIM_SetCompare2(TIM4,speed_left);
}

void stop(void)
{
	AIN1=1;
	AIN2=1;
	BIN1=1;
	BIN2=1;
}

void back(void)
{
	AIN1=0;
	AIN2=1;
	BIN1=1;
	BIN2=0;
}

void left(void)
{
	AIN1=0;
	AIN2=0;
	BIN1=1;
	BIN2=0;
}


void right(void)
{
	AIN1=1;
	AIN2=0;
	BIN1=0;
	BIN2=0;
}

void right1(void)
{
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;
}

void GPIO_init_CAR(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				 //配置PA4~7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化PA4~7
//  GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);						 //PA4~7输出高
}
