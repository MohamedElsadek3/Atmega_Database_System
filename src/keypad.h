/*
 * keypad.h
 *
 * Created: 8/12/2025
 * Author: ASUS
 */ 

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>        // AVR I/O operations
#include <util/delay.h>    // Delay functions

// --- Keypad connection ---
// Rows = PA0–PA3 (inputs with pull-ups)
// Cols = PA4–PA7 (outputs, driven low one at a time)
#define KEYPAD_PORT     PORTA
#define KEYPAD_PIN      PINA
#define KEYPAD_DDR      DDRA

#define KEYPAD_ROW_MASK 0x0F    // Rows = lower 4 bits
#define KEYPAD_COL_MASK 0xF0    // Cols = upper 4 bits

// --- Keypad layout ---
static const char keypad[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

// --- Functions ---
char readKeypad(void);    // Blocking: waits for key press
char keypad_scan(void);   // Simple wrapper for readKeypad()

#endif /* KEYPAD_H_ */
