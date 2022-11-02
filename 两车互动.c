蓝牙通信
#include "stm32f4xx.h"
#include "bluetooth.h"/*蓝牙通信:USART3,所用引脚为PC10,PC11*/
Void bluetooth_U3_Init()
{GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_GPIOC, ENABLE);  //使能GPIOA时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART2时钟//串口1对应引脚复用映射
GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); //GPIOA9复用为USART1
GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3); //GPIOA10复用为USART1//USART1端口配置
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PA9，PA10//USART1 初始化设置USART_InitStructure.USART_BaudRate = 9600;//波特率设置USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式USART_Init(USART3, &USART_InitStructure);//初始化串口1
USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启相关中断//Usart1 NVIC 配置NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  //串口1中断通道NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //子优先级3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ通道使能NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化VIC寄存器、
USART_Cmd(USART3, ENABLE); //使能串口1}/* 蓝牙使能引脚：PA9 */
void bluetooth_IO_Init()
{GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //??GPIOA??
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //??50MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //??????
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //??
GPIO_Init(GPIOC, &GPIO_InitStructure); //???PA9,PA10}

/***********单片机与OpenMV的通信协议*************************/
/* openmv发送过来的字符串转换为数据:帧头为' ',帧尾为'.',转换后只保留整数，中间用小数点判断,函数返回值为0则数据格式出错*/
signed char CharCon(signed char c[],signed char x)
{
int i = 0;
int j = 0;
while(c[i] != '.') {
c[i] = c[i] - '0';
i++;}
j = i;
if(c[0] == ('-'-'0')){
i = 4;c[0] = 0;}
switch(i){
case 1:x = c[i-1];break;
case 2:x = c[i-1] + 10*c[i-2];break;
case 3:x = -(c[i-1] + 10*c[i-2]);break;case 4:x = -(c[j-1] + 10*c[j-2]);break;
default:return 0;break;}    
return x;
}

 OpenMV发送的数据格式：
uart.write(" ")
uart.write(str(int(deflection_angle)))uart.write(".")

MAIN函数
// PB10是红外对管的输入引脚
#include "stm32f4xx.h"#include "sys.h"
#include "delay.h"
#include "duoji.h"
#include "dianji.h"
#include "bluetooth.h"
#include "to_openmv.h"
#include "hongwai.h"
#include "usart.h"
#define Center 88           //小车前轮保持居中不偏时单片机输出的PWN脉宽
#define Hongwai_IO PBin(10) //装载完药品，hongwai_IO = 0
#define Red PAout(14)
#define Green PEout(5)
#define White PAout(13)//以下定义的是状态机的各种状态，在最终的代码中有些并没有用到
#define Start 0 //初始的等待状态
#define Turn_left 1
#define Turn_right 2
#define Turn_no 3
#define Stop 4
#define Wait 5
#define Slower 6
#define Faster 7
#define Forward 8
#define Backward 9
#define Back 10
#define Change 11
#define Stoping 12
#define Changezuo 13
#define Changeyou 14
#define Fuwei 15
#define Finish 16
#define Nfuwei 17
#define Zanwait 18
int t = 0;
int speed = 200;               //马达速度
signed char angle = 0;         //巡线时舵机转的角度
signed char change_angle = 30; //路口停下来识别数字时微调的角度
uint16_t bluetooth = 0;        //接收到的蓝牙消息
uint16_t temp = 0;char state = 0;                  //状态机中的当前状态
char flag = 0;                   //是否已经识别到要去的病房号
signed char rec_flag = 0;        //单片机与OpenMV通信中的命令标志位
signed char openmv_rec[4] = {0}; //OpenMV发送的数据
signed char i = 0;
signed char task = 0; //当前执行的是任务几/* 指示灯引脚初始化 */
void LED_Init()
{    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);   
 GPIO_InitTypeDef GPIO_Instruct;    GPIO_Instruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; 
   GPIO_Instruct.GPIO_Mode = GPIO_Mode_OUT;   
 GPIO_Instruct.GPIO_Speed = GPIO_Speed_100MHz;   
 GPIO_Instruct.GPIO_OType = GPIO_OType_PP;   
 GPIO_Instruct.GPIO_PuPd = GPIO_PuPd_UP;   
 GPIO_Init(GPIOA, &GPIO_Instruct);  
  GPIO_Instruct.GPIO_Pin = GPIO_Pin_5;  
  GPIO_Init(GPIOE, &GPIO_Instruct);
}
void All_Init()
{   
 LED_Init();   
 hongwai_IO_Init(); 
   dianji_IO_Init(); 
   dianji_PWM_Init(1000 - 1, 84 - 1);   
 duoji_PWM_Init(2000 - 1, 840 - 1); //0   
 bluetooth_U3_Init();    //bluetooth_IO_Init();   
 openmv_U5_Init();    // computer_U1_Init();   
 uart_init(9600);   
 delay_init(168);
}
void turn_r(int t)
{    
TIM_SetCompare1(TIM3, 88);   
 PFout(5) = 0;  
  PFout(7) = 1;  
  PCout(4) = 1;  
  PCout(5) = 0;  
  TIM_SetCompare1(TIM5, t); 
   TIM_SetCompare1(TIM13, t);
delay_ms(3000);
}
int main(void)
{    
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   
 All_Init();   
 while (1)    {       
 switch (state)        {       
 case Start:           
 turn(0);           
 while (flag == 0); //已经识别到病房号        
Green = 0;     
       White = 1;        
    delay_ms(1000);      
      White = 0;           
 while (flag == 1)        
    {        
        if (Hongwai_IO == 0)       
         {                   
 delay_ms(1000);      
              if (Hongwai_IO == 0)      
              {       

                 state = Forward;  
                      break;    
                }             
   }        
    }  
          break;   
case Forward:       
     turn(angle);       
     zhengzhuan(speed);           break;      
  case Turn_right:        
    zhengzhuan(speed);      
      turn_right();         
   delay_ms(400);            break; //假设急转弯1.5S已经转过去，具体参数需要调试        case Turn_left:          
  turn_left();        
    zhengzhuan(200);   
    delay_ms(400);            break; //假设急转弯1.5S已经转过去，具体参数需要调试       
 case Stop:        
    stop();    
        delay_ms(20);      
      fanzhuan(speed);      
      delay_ms(300);      
      stop();       
     turn(0);      
      state = Stoping;    break;    
    case Wait:            
stop();          
  turn(0);     
       Red = 1;    
        if (Hongwai_IO == 1)      
      {             
   delay_ms(500);    
            if (Hongwai_IO == 1) 
               {               
     USART_SendData(UART5, 's');        
            Red = 0;              
      state = Back;             
       flag = 0;            
    }         
   }         
   break;     
  case Zanwait:      
      stop();      
      turn(0);    
        Red = 1;  
          if (Hongwai_IO == 1)            {        
        delay_ms(500);      
          if (Hongwai_IO == 1)   
             {         
           USART_SendData(USART3, 'o');             
       delay_ms(10);           
         USART_SendData(UART5, 's');       
             Red = 0;       
             state = Back;            
        flag = 0;          
      }        
}          
  break;     
   case Back:      
      fanzhuan(speed - 50);            // turn(-angle);      
      delay_ms(400);       
     state = Backward;    
        USART_SendData(UART5, 's');      
      break;      
  case Backward:       
     turn(3 - angle);      
      fanzhuan(speed - 50);            break;      
 case Change:           
 turn(angle);       
     zhengzhuan(speed);        
state = Stop;    
        delay_ms(500);            break;       
case Changezuo:           
 fanzhuan(speed);           
 delay_ms(400);           
 stop();            
turn(30);           
 zhengzhuan(speed);           
 change_angle = 30;           
 delay_ms(500);            
state = Stoping;            
USART_SendData(UART5, 's');            break;        
case Changeyou:            
fanzhuan(speed);            
delay_ms(400);            
stop();            
turn(-30);            
zhengzhuan(speed);           
 change_angle = -30;           
 delay_ms(500);           
 state = Stoping;           
 USART_SendData(UART5, 's');            break;        
case Fuwei:           
 turn(change_angle);            
fanzhuan(speed);           
 delay_ms(500);           
 stop();           
 turn(0);           
 zhengzhuan(speed);           
 delay_ms(400);            
stop();            break;        
case Nfuwei:            
turn(change_angle);            
fanzhuan(speed);           
 delay_ms(500);            
stop();           
 turn(0);            break;        
case Stoping:           
 stop();           
 turn(0);            break;        
case Finish:            
stop();            
turn(0);           
 Green = 1;            
flag = 0;            
state = Start;            break;        
default:            break;       
 }    }}/* 蓝牙接收 */
void USART3_IRQHandler(){  
  if (USART_GetITStatus(USART3, USART_IT_RXNE))    {        
bluetooth = USART_ReceiveData(USART3);    
}    
USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}/* openmv接收 */
void UART5_IRQHandler(){   
 if (USART_GetITStatus(UART5, USART_IT_RXNE))    {     
   char j = 0;      
  signed char temp = USART_ReceiveData(UART5);       
 if (temp == ' ')        {      
      rec_flag = 1;       
     i = 0;        }       
 else if (rec_flag == 1)        {    
        if (temp == '.')            {    
            openmv_rec[i] = '.';           
     angle = CharCon(openmv_rec, angle);    
            rec_flag = 0;        
        for (j = 0; j < 4; j++)          
          openmv_rec[j] = 0;      
      }        
    else            {         
       openmv_rec[i] = temp;           
     i++;          
  }      
  }       
 else //rec_flag == 0,接收的为指令        {   
         switch (temp)            {  
          case 's':               
 state = Stop;           
     break;         
   case 'l':             
   state = Turn_left;                break;            
case 'r':             
   state = Turn_right;                break;            
case 'f':         
       state = Forward;                break;            
case 'z':          
      state = Wait;                break;            
case 'b':       
         state = Backward;                break;            
case 'd':        
        state = Finish;                break;            
case 'e':        
        state = Zanwait;                break;            
case 'o':       
         flag = 1;                break;            
case 'c':         
       state = Change;                break;            
case 'x':         
       state = Changezuo;                break;            
case 'y':        
        state = Changeyou;                break;            
case 'w':       
         state = Fuwei;                break;            
case 'n':       
         state = Nfuwei;                break;            
case 't':       
        state = Stoping;                break;            
default:                break;          
  }   
     } 
   }   
 USART_ClearITPendingBit(UART5, USART_IT_RXNE);
}

