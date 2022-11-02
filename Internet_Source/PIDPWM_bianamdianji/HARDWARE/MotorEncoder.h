#ifndef __MOTORENCODER_H 
#define __MOTORENCODER_H 
#include "sys.h"
#include "TB6612.h"

void MotorEncoder_Init(void); 
int Read_Encoder(void);
void EncoderRead_TIM2(u16 arr, u16 psc);
int Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity);

#endif
