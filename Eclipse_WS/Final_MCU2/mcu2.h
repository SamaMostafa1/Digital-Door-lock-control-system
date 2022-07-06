/******************************************************************************
 *
 * File Name: mcu2.h
 *
 * Description: header file for the door lock system application
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/

#ifndef MCU2_H_
#define MCU2_H_
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define array_size      5
#define MC2_READY      0x10
#define MC1_READY      0x20
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/* Description :
 * 1.Functional responsible for receiving password  for first time to be saved in the in EEPROM
 * 2. Receive password when changing password to be compared with password saved in EEPROM
 * 3.Receive password to be compare  with password saved in the EEPROM*/
void recieve_Password();
/* Description :
 * 1.Functional responsible for receiving confirming password
 */
void recieve_confirmPassword();
/* Description :
 * 1.Functional responsible for saving confirming password in external EEPROM
 */
void save_Password(uint8 *password);
/* Description :
 * 1.Functional responsible for checking input password with the saved password in the EEPROM
 */
uint8 check_with_EEPROM(uint8 *ptr1, uint8 *ptr2);
/* Description :
 * 1.Functional responsible for opening the door if the entered password match the saved password
 */
void open_door();
/* Description :
 * 1.Functional responsible for closing the door after the door is opened
 */
void close_door();
/* Description :
 * 1. Call back Functional responsible for counting time required by the timer
 */
void Timer_function(void);
#endif /* MCU2_H_ */
