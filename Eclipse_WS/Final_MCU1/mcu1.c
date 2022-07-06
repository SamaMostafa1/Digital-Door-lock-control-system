/******************************************************************************
 *
 * File Name: mcu1.c
 *
 * Description:  door lock system application using to password of 5 digits to open the door if the password matched
 * the door open for 15 second then close and if the password mismatch for 3 time buzzer is on.
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/
#include"lcd.h"
#include "uart.h"
#include "mcu1.h"
#include "util/delay.h"
#include "timer.h"
#include "keypad.h"
#include <avr/io.h> /* To use ICU/Timer1 Registers */
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 confirm_Key, count_flag = 0, count, checker, send_key, pass_checker,
		counter_trial = 0, time = 0, g_step = 0;
uint8 pass[array_size];
uint8 confirm_Password[array_size];
/*******************************************************************************
 *                               APPlication                                 *
 *******************************************************************************/
int main(void) {
	SREG |= (1 << 7); //enable global interrupt enable
	UART_ConfigType uart = { Disable, one_bit, Eight_bits, rate7 };
	UART_init(&uart); //initialize UART
	LCD_init(); //initialize LCD
	g_step = 0;
	while (1) {
		switch (g_step) {
		/*step 0:
		 * The project opened for the first time and there is no password saved in EEPROM or when changing password
		 */
		case 0:
			new_Password();
			Confirm_Password();
			send_Password(confirm_Password); ///send the confirmed password
			checker = chek_Password(pass, confirm_Password); // check the two password
			g_step = checker;
			while (UART_recieveByte() != MC2_READY)
				;
			UART_sendByte(g_step); // send the result of the check to MCU2
			LCD_clearScreen();
			if (g_step == 0) {
				LCD_displayString("unmatched!!"); // if password unmatched repeat step 0 again
				_delay_ms(500);
				LCD_clearScreen();
				break;
				/*step 1:
				 * The two password matched
				 */
				case 1:
				LCD_displayString("matched!"); // if password matched go to step2
				_delay_ms(500);
				g_step = 2;
				break;
				/*step 2:
				 * Display main options
				 * choose one of the main options and send it to MCU2
				 * go to the required step
				 */
				case 2:
				LCD_clearScreen();
				display(); //main options
				send_key = KEYPAD_getPressedKey();
				while (UART_recieveByte() != MC2_READY)
					;
				UART_sendByte(send_key); //send the option chhosed
				LCD_clearScreen();
				LCD_displayCharacter(send_key);
				_delay_ms(50);
				if (send_key == '+') {
					g_step = 3; // open door step

				} else if (send_key == '-') {

					g_step = 4; //change password step
				}
				break;
				/* step 3 :
				 *  To open the door enter password and send password to be checked with saved password in eeprom
				 * if unmatched for 3 time go to step 5
				 * if matched open the door
				 * */
				case 3:
				new_Password(); //password to bechecked with the saved password
				send_Password(pass);
				pass_checker = UART_recieveByte();
				if (pass_checker == 0) {
					LCD_clearScreen();
					LCD_displayString("unmatched!!");
					_delay_ms(1000);
					LCD_clearScreen();
					g_step = UART_recieveByte();
				} else if (pass_checker == 1) {
					open_door();
					while (g_step != 2);
				}
				break;

				/* step 4 :
				 *  To change enter password and send password to be checked with saved password in eeprom
				 * if unmatched for 3 time go to step 5
				 * if matched change password
				 * */
				case 4:
				new_Password();
				send_Password(pass);
				pass_checker = UART_recieveByte();
				if (pass_checker == 0) {
					LCD_displayString("unmatched");
					_delay_ms(1000);
					LCD_clearScreen();
					g_step = UART_recieveByte(); //Receive step after checking
				} else if (pass_checker == 1) {
					LCD_displayString("matched");
					_delay_ms(1000);
					LCD_clearScreen();
					g_step = UART_recieveByte();
				}
				break;
				/* step 5 :
				 * The password  unmatched for 3 time.
				 */
				case 5:
				time = 60; /*the timer will count 60 seconds */
				Timer_ConfigType timer = { Timer0, CTC, F_CPU_1024, No_Channel,
						0, 245 };
				Timer0_setCallBack(Timer_function); /* Set the Call back function pointer in the timer0 driver*/
				Timer_init(&timer);
				LCD_displayString("ERROR!");
				_delay_ms(6000);
				LCD_clearScreen();
				break;
			}
		}
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * 1.Functional responsible for creating password  for first time to be saved in the in EEPROM
 * 2. when changing password
 * 3.enter password to compare it with password saved in the EEPROM
 */
void new_Password() {
	uint8 i;
	LCD_displayString("Enter password");
	LCD_moveCursor(1, 0);
	for (i = 0; i < 5; i++) { // password of 5 digits
		_delay_ms(600);
		pass[i] = KEYPAD_getPressedKey(); // get pressed key number
		if ((pass[i] <= 9) && (pass[i] >= 0)) {
			LCD_displayCharacter('*'); /* display * for the  pressed key */
		}
	}
	_delay_ms(500); // wait for half second to clear the screen
	LCD_clearScreen();
}
/*
 * Description :
 * 1.Functional responsible for creating confirming password before saved in the in EEPROM
 */
void Confirm_Password() {
	uint8 i;
	LCD_displayString("Renter password");
	LCD_moveCursor(1, 0);
	for (i = 0; i < 5; i++) { //password of 5 digits
		_delay_ms(600);
		confirm_Password[i] = KEYPAD_getPressedKey(); // get pressed key number
		if ((confirm_Password[i] <= 9) && (confirm_Password[i] >= 0)) {
			LCD_displayCharacter('*'); /* display * for the  pressed key */
		}
	}
	_delay_ms(500); // wait for half second to clear the screen
	LCD_clearScreen();
}
/*
 * Description :
 * 1.Functional responsible for sending password  for first time to be saved in the in EEPROM in MCU2
 * 2. when changing password to be compared with password saved in EEPROM
 * 3.enter password to compare it with password saved in the EEPROM
 */
void send_Password(uint8 *pass_Ptr) {
	uint8 i;
	do {
		LCD_displayString("press = to send");
		confirm_Key = KEYPAD_getPressedKey(); //key used to send password
	} while (confirm_Key != '=');
	LCD_clearScreen(); // clear screen after sending password
	while (UART_recieveByte() != MC2_READY) {
	}
	/* wait for MCU2 is ready to receive data*/
	for (i = 0; i < 5; i++) { // send 5 digit of the password
		_delay_ms(50);
		UART_sendByte(pass_Ptr[i]);
	}
	LCD_clearScreen();
	LCD_displayString("password sent");
	_delay_ms(500);
	LCD_clearScreen();
}
/* Description :
 * Functional responsible for comparing the new password and the confirming password
 * to be saved in EEPROM if the two password match or renter password again if the two
 * passwords mismatch
 */
uint8 chek_Password(uint8 *pass, uint8 *confirm_Pass) {
	uint8 i;
	for (i = 0; i < 5; i++) {
		/*checking each element of array is matched*/
		if (pass[i] != confirm_Pass[i]) {
			return 0;
		}
	}
	return 1;
}
/* Description :
 * Functional responsible for displaying the main options of the door
 */
void display() {
	LCD_displayString("Open door: + ");
	LCD_moveCursor(1, 0);
	LCD_displayString("Change pass:-");
}
/* Description :
 * Functional responsible for displaying the opening of the door if the password math the password saved in the EEPROM
 * calling close door function
 */
void open_door() {
	LCD_displayString("matched!");
	_delay_ms(500);
	LCD_clearScreen();
	time = 15; // time needed to open the door
	Timer_ConfigType timer = { Timer0, CTC, F_CPU_1024, No_Channel, 0, 245 }; // timer0 in compare mode
	Timer_init(&timer); // initialize timer
	Timer0_setCallBack(Timer_function); /* Set the Call back function pointer in the timer0 driver*/
	LCD_displayString("door is opening!");
	_delay_ms(3000);
	close_door();
}
/* Description :
 * Functional responsible for displaying the closing of the door
 */
void close_door() {
	LCD_clearScreen();
	time = 15; // time needed to close the door
	Timer_ConfigType timer = { Timer0, CTC, F_CPU_1024, No_Channel, 0, 245 }; // timer0 in compare mode
	Timer_init(&timer); // initialize timer
	Timer0_setCallBack(Timer_function); /* Set the Call back function pointer in the timer0 driver*/
	LCD_displayString("door is closing!");
}
/* Description :
 * 1. Call back Functional responsible for counting time required by the timer
 */
void Timer_function(void) {
	count_flag++; // increase count flag each interrupt
	if (count_flag == 32) { // check if the count flag become 1 sec
		count_flag = 0; // clear flag counter to start counting again
		count++; //increase second counter
		/*checks if seconds counter reached the required secounds
		 * clears the seconds counter*/
		if (count == time) {
			count = 0;
			Timer_DeInit();
			LCD_clearScreen();
			g_step = 2;
		}
	}
}

