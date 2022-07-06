/******************************************************************************
 *
 * File Name: mcu1.h
 *
 * Description: header file for the door lock system application
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/
#ifndef MCU1_H_
#define MCU1_H_
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define array_size      5
#define MC2_READY     0x10
#define MC1_READY     0x20
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * 1.Functional responsible for creating password  for first time to be saved in the in EEPROM
 * 2. when changing password
 * 3.enter password to compare it with password saved in the EEPROM
 */
void new_Password();
/*
 * Description :
 * 1.Functional responsible for creating confirming password before saved in the in EEPROM
 */
void Confirm_Password();
/*
 * Description :
 * 1.Functional responsible for sending password  for first time to be saved in the in EEPROM in MCU2
 * 2. when changing password to be compared with password saved in EEPROM
 * 3.enter password to compare it with password saved in the EEPROM
 */
void send_Password(uint8 *pass_Ptr);
/* Description :
 * Functional responsible for comparing the new password and the confirming password
 * to be saved in EEPROM if the two password match or renter password again if the two
 * passwords mismatch
 */
uint8 chek_Password(uint8 *pass, uint8 *confirm_Pass);
/* Description :
 * Functional responsible for displaying the main options of the door
 */
void display();
/* Description :
 * Functional responsible for displaying the opening of the door if the password math the password saved in the EEPROM
 * calling close door function
 */
void open_door();
/* Description :
 * Functional responsible for displaying the closing of the door
 */
void close_door();
/* Description :
 * 1. Call back Functional responsible for counting time required by the timer
 */
void Timer_function(void);
#endif /* MCU1_H_ */
