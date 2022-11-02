#include "sys.h"
#include "delay.h"
#include "led.h"
#include "PWM.h"
#include "car.h"
#include "usart.h"
#include <string.h>
#include <math.h>

//char source[10];  //存储串口缓存数据

//由于两个电机阻力不同，所以有偏差，需要自行矫正
#define speed_left 100 //左轮速率
#define  speed_right 100  //右轮速率

u16 num=50;  //存储字符转整形的数据

#define R   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键1(KEY1)
#define M   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键2(KEY2)
#define L   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取按键3(KEY3)

int16_t shuju;

//矫正左右偏差电机控速函数
void run(void)
{
	  GO();
		if(num<100)  //检测到右偏
		{
					  //TIM_SetCompare1(TIM4,speed_right-num);   //左轮减速
		  TIM_SetCompare1(TIM4,speed_right+num);   //左轮减速
	    TIM_SetCompare2(TIM4,speed_left+num);   //右轮加速
		}
		if(num>=100)  //检测到左偏
		{
			TIM_SetCompare1(TIM4,speed_right+num-100);  //右轮加速
	    TIM_SetCompare2(TIM4,speed_left-num-100);  //左轮减速
		}
}


/*int simple_atoi(char *source) //字符串转整形
{
	int length = strlen(source); //计算长度
	int sum = 0;
	int i;
	
		for(i=0;i<length;i++) 
	  {
		  sum += (source[i]-'0') * pow(10,length-1-i); //字符转数字并合并
	  }
	
	for(i=0;i<length;i++) source[i]=0;//清零数组
	return sum;
}*/

 void DELAY(unsigned int xms)  //由于正点原子的延时函数在protues运用中存在问题，所以才有手写延时，大约为10ms级延时
{
	unsigned int j;
	for( ;xms>0;xms--)
	 for(j=1400;j>0;j--);
}

/*void huidu_init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	//开始时钟GPIOA/GPIOE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//设置为浮空输入
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

}
u8 huidu_Scan(void)  //检测程序
{	 
  

//		if(M==0)return num=50;
//		else if(R==0)return num=20;
//		else if(L==1)return num=10;
	 if(R==1&&M==1&&L==1)num=99; 
   else	num=0;
 	return num;// 无按键按下
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
	delay_init();  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为19200
	GPIO_init_CAR();
	LED_Init();
 // huidu_init();  
	//初始化车速
		TIM4_PWM_Init(19999,83);  //初始化PWM  8k hz
		TIM_SetCompare1(TIM4,speed_left);  
	  TIM_SetCompare2(TIM4,speed_right);  
	
	while(1)
    {
			 /*if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				
				for(t=0;t<len;t++)
			  {
				  source[t]=USART_RX_BUF[t];  //将BUF的串口数据转移到source数组
			  }
				num = simple_atoi(source);  //处理数据
				USART_RX_STA=0;
			}	*/
			/*huidu_Scan();*/
     if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				
				for(t=0;t<len;t++)
			  {
				  shuju=USART_RX_BUF[t];  //将BUF的串口数据转移到source数组
					Openmv_Receive_Data(shuju);		
			  }
				
				Openmv_Data();	  //处理数据
				USART_RX_STA=0;
			}
     openmv_control();			
			shiijan(5000);
			run();  //矫正左右偏差

	}
}
