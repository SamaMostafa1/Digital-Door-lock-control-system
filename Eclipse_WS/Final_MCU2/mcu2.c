/******************************************************************************
 *
 * File Name: mcu2.c
 *
 * Description:  door lock system application using to password of 5 digits to open the door if the password matched
 * the door open for 15 second then close and if the password mismatch for 3 time buzzer is on.
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/
#include "buzzer.h"
#include "external_eeprom.h"
#include "gpio.h"
#include "util/delay.h"
#include "twi.h"
#include "std_types.h"
#include "mcu2.h"
#include "uart.h"
#include "timer.h"
#include "dc_motor.h"
#include "buzzer.h"
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 count_flag, count, result, counter_trial = 0;
uint8 time, g_step, checker, sym;
uint8 new_pass[array_size];
uint8 pass_1[array_size];
/*******************************************************************************
 *                               APPlication                                 *
 *******************************************************************************/
int main(void) {
	SREG |= (1 << 7); //enable global interrupt enable
	UART_ConfigType uart = { Disable, one_bit, Eight_bits, rate7 };
	TWI_ConfigType twi = { Fast, 1 };
	TWI_init(&twi); //initialize I2C
	UART_init(&uart); //initialize UART
	DcMotor_Init(); //initialize motor
	DC_state state = { stop }; //motor is stoped at first
	DcMotor_Rotate(state, 100);
	Buzzer_init(); //initialize buzzer
	g_step = 0;
	while (1) {
		switch (g_step) {
		/*step 0:
		 * The project opened for the first time and there is no password saved in EEPROM or when changing password
		 */
		case 0:
			recieve_confirmPassword();
			UART_sendByte(MC2_READY);
			/*receive the step from MCU1 determines in which step the code go */
			checker = UART_recieveByte();
			g_step = checker;
			break;
			/*step 1:
			 * The two password matched so password is saved in EEPROM
			 */
		case 1:
			save_Password(pass_1);
			g_step = 2;
			break;
			/*step 2:
			 * Receive the choosed option
			 * go to the required step
			 */
		case 2:
			/*receive the symbol from MCU1 determines in which step the code go */
			UART_sendByte(MC2_READY);
			sym = UART_recieveByte();

			if (sym == '+') {
				g_step = 3;
			} else if (sym == '-') {
				g_step = 4;
			}
			break;
			/* step 3 :
			 *  To open the door check password with eeprom
			 * if unmatched for 3 time go to step 5
			 * if matched open the door
			 * */
		case 3:
			recieve_Password();
			result = check_with_EEPROM(new_pass, pass_1); // check password with the EEPROM
			UART_sendByte(result); // result of check match or mismatch
			if (result == 0) { // password mismatch
				counter_trial++;
				if (counter_trial == 3) {
					g_step = 5; // step of buzzer on when password mismatch for 3 times
				}
				UART_sendByte(g_step);
			} else if (result == 1) { // password match
				open_door();
				counter_trial = 0;
				while (g_step != 2)
					;
			}
			break;
			/* step 3 :
			 *  To open the door check password with eeprom
			 * if unmatched for 3 time go to step 5
			 * if matched open the door
			 * */
		case 4:
			recieve_Password();
			result = check_with_EEPROM(new_pass, pass_1); // check password with the EEPROM
			UART_sendByte(result); // result of check match or mismatch
			if (result == 0) { //password unmatch
				counter_trial++;
				/*the user run out the trials and go to step 5 */
				if (counter_trial == 3) {
					counter_trial = 0;
					g_step = 5;

				}
				UART_sendByte(g_step);
			} else if (result == 1) { //password match
				counter_trial = 0;
				g_step = 0;
				UART_sendByte(g_step);
			}
			break;
			/* step 5 :
			 * The password  unmatched for 3 time.
			 */
		case 5:
			time = 60; /*the timer will count 60 seconds */
			Timer_ConfigType timer = { Timer0, CTC, F_CPU_1024, No_Channel, 0,
					245 };
			Timer_init(&timer);
			Timer0_setCallBack(Timer_function); /* Set the Call back function pointer in the timer0 driver*/
			Buzzer_on();
			while (g_step != 2)
				;
			break;
		}
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/* Description :
 * 1.Functional responsible for receiving password  for first time to be saved in the in EEPROM
 * 2. Receive password when changing password to be compared with password saved in EEPROM
 * 3 .Receive password to be compare  with password saved in the EEPROM*/
void recieve_Password() {
	uint8 i;
	/* Send MC2_READY byte to MC1 to ask it to send the password */
	UART_sendByte(MC2_READY);
	for (i = 0; i < 5; i++) {
		new_pass[i] = UART_recieveByte();
	}
}
/* Description :
 * 1.Functional responsible for receiving confirming password
 */
void recieve_confirmPassword() {
	uint8 i;
	/* Send MC2_READY byte to MC1 to ask it to send the password */
	UART_sendByte(MC2_READY);
	for (i = 0; i < 5; i++) {
		pass_1[i] = UART_recieveByte();
	}

}
/* Description :
 * 1.Functional responsible for saving confirming password in external EEPROM
 */
void save_Password(uint8 *password) {
	uint8 j;
	for (j = 0; j < 5; j++) {
		EEPROM_writeByte(0x0311 + j, password[j]);
		_delay_ms(10);
	}
}
/* Description :
 * 1.Functional responsible for checking input password with the saved password in the EEPROM
 */
uint8 check_with_EEPROM(uint8 *ptr1, uint8 *ptr2) {
	uint8 k;
	for (k = 0; k < 5; k++) { // password of 5 numbers
		/*reads the saved value of address 0x0311 and next 4 addresses in EEPROM and check sit with the input password */
		EEPROM_readByte(0x0311 + k, &ptr2[k]);
		if (ptr1[k] != ptr2[k]) {
			return 0;
		}
	}
	return 1;
}
/* Description :
 * 1.Functional responsible for opening the door if the entered password match the saved password
 */
void open_door() {
	time = 15; // the motor will rotate for 15 seconds
	Timer_ConfigType timer = { Timer0, CTC, F_CPU_1024, No_Channel, 0, 245 }; //timer0 using compare mode
	Timer_init(&timer); // initialize timer
	Timer0_setCallBack(Timer_function);
	DC_state state = { clockwise };
	DcMotor_Rotate(state, 100); // door is opening
	_delay_ms(3000);
	close_door();	  //close door function
}
/* Description :
 * 1.Functional responsible for closing the door after the door is opened
 */
void close_door() {
	time = 15; // the motor will rotate for 15 seconds
	Timer_ConfigType timer = { Timer0, CTC, F_CPU_1024, No_Channel, 0, 245 };
	Timer_init(&timer); // initialize timer
	Timer0_setCallBack(Timer_function); // using timer0 in compare mode
	DC_state state = { anticlockwise }; // door is closing
	DcMotor_Rotate(state, 100);
}
/* Description :
 * 1. Call back Functional responsible for counting time required by the timer
 */
void Timer_function(void) {
	count_flag++; // increase count flag each interrupt
	if (count_flag == 32) { // check if the count flag become 1 sec
		count_flag = 0; // clear flag counter to start counting again
		count++; //increase second counter
		/*checks if seconds counter reached the required seconds
		 * clears the seconds counter*/
		if (count == time) {
			count = 0;
			DC_state state = { stop }; // door is closing
			DcMotor_Rotate(state, 0);
			Timer_DeInit();
			Buzzer_off();
			g_step = 2;
		}
	}
}
