/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR Timer driver
 *
 * Author: Sama mostafa
 *
 *******************************************************************************/
#include "timer.h"
#include"std_types.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr2)(void) = NULL_PTR;

/*******************************************************************************/
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect) {
	if (g_callBackPtr0 != NULL_PTR) {
		/* Call the Call Back function in the application after interrupt by normal mode in timer0 is handled */
		(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER0_COMP_vect) {
	if (g_callBackPtr0 != NULL_PTR) {
		/* Call the Call Back function in the application after interrupt by compare mode in timer0 is handled */
		(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER1_OVF_vect) {
	if (g_callBackPtr1 != NULL_PTR) {
		/* Call the Call Back function in the application after interrupt by normal mode in timer1 is handled */
		(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER1_COMPA_vect) {
	if (g_callBackPtr1 != NULL_PTR) {
		/* Call the Call Back function in the application after interrupt by compare mode in timer1 using channelA  is handled */
		(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER2_OVF_vect) {
	if (g_callBackPtr2 != NULL_PTR) {
		/* Call the Call Back function in the application after interrupt by normal mode in timer2 is handled */
		(*g_callBackPtr2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER2_COMP_vect) {
	if (g_callBackPtr2 != NULL_PTR) {
		/* Call the Call Back function in the application after interrupt by compare mode intimer2 is handled */
		(*g_callBackPtr2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
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
 */
void Timer_init(const Timer_ConfigType *Config_Ptr) {
	switch (Config_Ptr->type) {
	case Timer0:
		if ((Config_Ptr->mode) == Normal) {
			TCNT0 = (Config_Ptr->Value);
			SET_BIT(TIMSK, TOIE0);	// Enable Timer0 Overflow Interrupt
			SET_BIT(TCCR0, FOC0); // Non PWM mode FOC0=1
			/*
			 * insert the required clock value in the first three bits (CS00, CS01 and CS02)
			 * of TCCR0 Register
			 */
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);
		} else if ((Config_Ptr->mode) == CTC) {
			TCNT0 = (Config_Ptr->Value);
			SET_BIT(TIMSK, OCIE0); // Enable Timer0 Compare Interrupt
			/* 1.Non PWM mode FOC0=1
			 * 2. compare mode WGM01=1
			 * */
			TCCR0 = (1 << FOC0) | (1 << WGM01);
			/*
			 * insert the required clock value in the first three bits (CS00, CS01 and CS02)
			 * of TCCR0 Register
			 */
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);
			OCR0 = (Config_Ptr->compareValue); //set Timer Compare value
		}
		break;
	case Timer1:
		if ((Config_Ptr->mode) == Normal) {
			TCNT1=  (Config_Ptr->Value);
			SET_BIT(TIMSK, TICIE1);	// Enable Timer1 Overflow Interrupt
			TCCR1A = (1 << FOC1A) | (1 << FOC1B);// Non PWM mode FOC1A=1, FOC1B=1
			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xFC) | (Config_Ptr->clock);
		} else if ((Config_Ptr->mode) == CTC) {
			TCNT1 = (Config_Ptr->Value);
			SET_BIT(TIMSK, OCIE1A); // Enable Timer1 Compare Interrupt
			TCCR1A = (1 << FOC1A) | (1 << FOC1B); // Non PWM mode FOC1A=1, FOC1B=1
			/*
			 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
			 * of TCCR1B Register
			 */
			TCCR1B = (TCCR1B & 0xFC) | (Config_Ptr->clock);
			if (Config_Ptr->channel == ChannelA_ID) {
				OCR1A = (Config_Ptr->compareValue); //set Timer Compare value
				SET_BIT(TCCR1B, WGM12);
			} else if (Config_Ptr->channel == ChannelB_ID) {
				ICR1 = (Config_Ptr->compareValue); //set Timer Compare value
				TCCR1B = (1 << WGM12) | (1 << WGM13);
			}
		}
		break;
	case Timer2:
		if ((Config_Ptr->mode) == Normal) {
			TCNT2 = (Config_Ptr->Value);
			SET_BIT(TIMSK, TOIE2);	// Enable Timer0 Overflow Interrupt
			SET_BIT(TCCR2, FOC2); // Non PWM mode FOC0=1
			/*
			 * insert the required clock value in the first three bits (CS00, CS01 and CS02)
			 * of TCCR0 Register
			 */
			TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->clock);
		} else if ((Config_Ptr->mode) == CTC) {
			TCNT2 = (Config_Ptr->Value);
			SET_BIT(TIMSK, OCIE2); // Enable Timer0 Compare Interrupt
			/* 1.Non PWM mode FOC0=1
			 * 2. compare mode WGM01=1
			 * */
			TCCR2 = (1 << FOC2) | (1 << WGM21);
			/*
			 * insert the required clock value in the first three bits (CS00, CS01 and CS02)
			 * of TCCR0 Register
			 */
			TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->clock);
			OCR2 = (Config_Ptr->compareValue); //set Timer Compare value
		}
		break;
	}
}

/*
 * Description: Function to set the Call Back function address using timer0.
 */
void Timer0_setCallBack(void (*a_ptr0)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr0 = a_ptr0;
}
/*
 * Description: Function to set the Call Back function address using timer1.
 */
void Timer1_setCallBack(void (*a_ptr1)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr1 = a_ptr1;
}
/*
 * Description: Function to set the Call Back function address using timer2.
 */
void Timer2_setCallBack(void (*a_ptr2)(void)) {
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr2 = a_ptr2;
}
/*
 * Description: Function to disable the Timer
 */
void Timer_DeInit(void) {
	TCNT0 = 0;
	TIMSK = 0;
	TCCR0 = 0;
	TCCR1A=0;
	TCNT1=0;
}
