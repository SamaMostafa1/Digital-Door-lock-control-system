 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: header file for the AVR Timer driver
 *
 * Author: Sama mostafa
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
Timer0,Timer1,Timer2
}Timer_type;
typedef enum
{
	Normal, CTC=2
}Timer_mode;
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;
typedef enum
{
	No_Channel,ChannelA_ID,ChannelB_ID
}Timer1_compareChannel;
typedef struct
{
	Timer_type type;
	Timer_mode mode;
	Timer_Clock clock;
	Timer1_compareChannel channel;
	uint8 Value;
	uint16 compareValue;

}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Set the required clock.
 * 	2. Set the required Timer mode.
 * 	3. Enable the non PWM mode.
 * 	4. Initialize Timer Registers
 * 	5. Set Timer initial value
 * 	6. set Timer Compare value
 *
 */
void Timer_init(const Timer_ConfigType *Config_Ptr);
/*
 * Description: Function to set the Call Back function address using timer0.
 */
void Timer0_setCallBack(void (*a_ptr0)(void));

/*
 * Description: Function to set the Call Back function address using timer1.
 */
void Timer1_setCallBack(void (*a_ptr1)(void));
/*
 * Description: Function to set the Call Back function address using timer2.
 */
void Timer2_setCallBack(void (*a_ptr2)(void));
/*
 * Description: Function to disable the Timer
 */
void Timer_DeInit(void);

#endif /* TIMER_H_ */
