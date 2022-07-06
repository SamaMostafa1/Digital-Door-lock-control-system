/******************************************************************************
 *
 * Module: DC-MOTOR
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the DC-Motor driver
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/

#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */
#include "dc_motor.h"
#include"std_types.h"
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * The Function responsible for setup the direction for the two
 motor pins through the GPIO driver.
 *  Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void) {
	/* configure the motor pins as output pins */
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN1_ID, PIN_OUTPUT);
	/* Motor is stop at the beginning */
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);

}
/*
 * Description :
 * The function responsible for rotate the DC Motor CW/ or A-CW or
 stop the motor based on the state input state value.
 Send the required duty cycle to the PWM driver based on the
 required speed value.
 */
void DcMotor_Rotate(DC_state state, uint8 speed) {
	//PWM_Timer0_Start(speed);
	if (state == clockwise) {
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
	} else if (state == anticlockwise) {
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_HIGH);
	} else {
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
	}
}
