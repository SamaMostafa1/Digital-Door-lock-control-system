/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.
 *
 * Description: header file for the buzzer driver
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_
#include "gpio.h"
#include "avr/io.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
#define Buzzer_port   PORTC_ID
#define Buzzer_bin     PIN5_ID
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the buzzer
 */
void Buzzer_init();
/*
 * Description :
 * Functional responsible for setting buzzer bin to 1 buzzer on
 */
void Buzzer_on();
/*
 * Description :
 * Functional responsible for setting buzzer bin to 0 buzzer off
 */
void Buzzer_off();

#endif /* BUZZER_H_ */
