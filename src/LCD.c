/*
 * LCD.c
 *
 * Created: 8/9/2025 12:16:24 PM
 *  Author: ASUS
 */ 
#include "LCD.h"

void LCD_command(unsigned char cmnd)
{
	LCD_DATA_PORT = cmnd; // Send the command to the data port
	LCD_CTRL_PORT &= ~(1 << RS_PIN); // RS = 0 for command
	LCD_CTRL_PORT &= ~(1 << RW_PIN); // RW = 0 for write
	LCD_CTRL_PORT |= (1 << E_PIN); // Enable high
	_delay_ms(1);
	LCD_CTRL_PORT &= ~(1 << E_PIN); // Enable low
	_delay_ms(1);
}

void LCD_data(unsigned char data)
{
	LCD_DATA_PORT = data; // Send the data to the data port
	LCD_CTRL_PORT |= (1 << RS_PIN); // RS = 1 for data
	LCD_CTRL_PORT &= ~(1 << RW_PIN); // RW = 0 for write
	LCD_CTRL_PORT |= (1 << E_PIN); // Enable high
	_delay_ms(1);
	LCD_CTRL_PORT &= ~(1 << E_PIN); // Enable low
	_delay_ms(1);
}

void LCD_init()
{
	// Set data and control ports as outputs
	DDRD = 0xFF; // Data port
	DDRB |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN); // Control port
	_delay_ms(20); // LCD Power ON delay
	LCD_command(0x38); // 2 lines, 5x7 matrix
	LCD_command(0x0E); // Display on, cursor on
	LCD_command(0x01); // Clear display
	_delay_ms(2);
}

void LCD_display_string(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		LCD_data(str[i]);//A--> N--> A--> S--> \0 BREAK
		_delay_ms(10);
	}
}
void LCD_display_char( const char *chr )

{
	LCD_data(chr);
}