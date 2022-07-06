/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the buzzer driver
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/
#include "gpio.h"
#include"buzzer.h"
#include "avr/io.h"
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the buzzer
 */
void Buzzer_init() {
	GPIO_setupPinDirection(Buzzer_port, Buzzer_bin, PIN_OUTPUT);
	GPIO_writePin(Buzzer_port, Buzzer_bin, LOGIC_LOW);
}
/*
 * Description :
 * Functional responsible for setting buzzer bin to 1 buzzer on
 */
void Buzzer_on() {
	GPIO_writePin(Buzzer_port, Buzzer_bin, LOGIC_HIGH);
}
/*
 * Description :
 * Functional responsible for setting buzzer bin to 0 buzzer off
 */
void Buzzer_off() {
	GPIO_writePin(Buzzer_port, Buzzer_bin, LOGIC_LOW);
}
