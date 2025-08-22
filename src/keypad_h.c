/*
 * keypad.c
 *
 * Created: 8/12/2025
 * Author: ASUS
 */ 

#include "keypad.h"

// Blocking read: waits until a key is pressed
char readKeypad(void)
{
    while (1)
    {
        for (int col = 0; col < 4; col++)
        {
            // Activate one column low, keep pull-ups on rows
            KEYPAD_DDR = KEYPAD_COL_MASK;
            KEYPAD_PORT = KEYPAD_ROW_MASK | (KEYPAD_COL_MASK & ~(1 << (col + 4)));

            // Check rows
            for (int row = 0; row < 4; row++)
            {
                if (!(KEYPAD_PIN & (1 << row))) // key pressed
                {
                    _delay_ms(20);  // debounce
                    while (!(KEYPAD_PIN & (1 << row))); // wait release
                    return keypad[row][col];
                }
            }
        }
    }
    return 0;  // never reached
}

// Wrapper for blocking scan
char keypad_scan(void)
{
    return readKeypad();
}
