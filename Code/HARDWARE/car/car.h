#ifndef __CAR_H_
#define __CAR_H_
#include "sys.h"
#include "PWM.h"

#define BIN2 PAout(4)// PA4
#define BIN1 PAout(5)// PA5
#define AIN1 PAout(6)// PA6
#define AIN2 PAout(7)// PA7


#define  Car_Gpio GPIOB
#define  Car_Gpio1 GPIOA
#define  Color_GPIO  GPIOA
#define  Car_Clock  RCC_APB2Periph_GPIOB;
#define  Color_Clock RCC_APB2Periph_GPIOA

void GO(void);
void stop(void);
void back(void);
void left(void);
void right(void);
void right1(void);
void avoid(void);
void GPIO_init_CAR(void);


#endif

