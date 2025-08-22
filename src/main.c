#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "keypad.h"
#include "LCD.h"

#define MAX_USERS 5
char count_user = 2;
char name_count = 2;
struct user {
	char name[20];
	char pass[7];
	} admins[MAX_USERS] = {
	{"user1", "987654"},
	{"user2", "123524"}
};

void enter(char *pass) {
	LCD_command(0x01);
	LCD_command(0x80);
	_delay_ms(2);
	LCD_display_string("Enter Password:");
	char counter = 0;
	char pos = 0;
	for (; counter < 6; counter++) {
		pass[counter] = keypad_scan();
		LCD_command(0xC0 + pos);
		pos++;
		LCD_display_char('*');
	}
	pass[counter] = '\0';
}

int check() {
	char pass[7];
	char tries = 0;
	while (tries < 3) {
		enter(pass);
		for (int i = 0; i < count_user; i++) {
			if (strcmp(pass, admins[i].pass) == 0) {
				LCD_command(0x01);
				_delay_ms(2);
				LCD_display_string("Welcome ");
				LCD_display_string(admins[i].name);
				_delay_ms(2000);
				return 1;
			}
		}
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string("Wrong password!");
		_delay_ms(1500);
		tries++;
	}
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("Access Denied!");
	_delay_ms(2000);
	return 0;
}

void add_user() {
	if (count_user >= MAX_USERS) {
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string("Max admins is 5!");
		_delay_ms(1500);
		return;
	}
	sprintf(admins[count_user].name, "User%d", name_count + 1);
	char pass[7];
	char unique;
	do {
		unique = 1;
		enter(pass);
		for (int i = 0; i < count_user; i++) {
			if (strcmp(pass, admins[i].pass) == 0) {
				LCD_command(0x01);
				_delay_ms(2);
				LCD_display_string("Pass exists!");
				_delay_ms(1000);
				unique = 0;
				break;
			}
		}
	} while (!unique);
	strcpy(admins[count_user].pass, pass);
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("Admin Added!");
	_delay_ms(1000);
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string(admins[count_user].name);
	_delay_ms(1000);
	count_user++;
	name_count++;
}

void edit_user() {
	if (count_user == 0) {
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string("No users yet!");
		_delay_ms(1500);
		return;
	}
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("Enter user index:");
	_delay_ms(1000);
	char idx_char = keypad_scan();
	int idx = idx_char - '0';
	if (idx < 1 || idx > count_user) {
		LCD_command(0x01);
		LCD_display_string("Invalid index!");
		_delay_ms(1500);
		return;
	}
	char pass[7];
	char unique;
	do {
		unique = 1;
		enter(pass);
		for (int i = 0; i < count_user; i++) {
			if (i != (idx - 1) && strcmp(pass, admins[i].pass) == 0) {
				LCD_command(0x01);
				_delay_ms(2);
				LCD_display_string("Pass exists!");
				_delay_ms(1000);
				unique = 0;
				break;
			}
		}
	} while (!unique);
	strcpy(admins[idx - 1].pass, pass);
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("Password Updated!");
	_delay_ms(1500);
}

void delete_user() {
	if (count_user == 0) {
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string("No users yet!");
		_delay_ms(1500);
		return;
	}
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("Enter user index:");
	_delay_ms(1000);
	char idx_char = keypad_scan();
	int idx = idx_char - '0';
	if (idx < 1 || idx > count_user) {
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string("Invalid index!");
		_delay_ms(1500);
		return;
	}
	for (int i = idx - 1; i < count_user - 1; i++) {
		strcpy(admins[i].name, admins[i + 1].name);
		strcpy(admins[i].pass, admins[i + 1].pass);
	}
	count_user--;
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("User Deleted!");
	_delay_ms(1500);
}

void display_user() {
	if (count_user == 0) {
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string("No users yet!");
		_delay_ms(1500);
		return;
	}
	for (int i = 0; i < count_user; i++) {
		LCD_command(0x01);
		_delay_ms(2);
		LCD_display_string(admins[i].name);
		LCD_command(0xC0);
		LCD_display_string(admins[i].pass);
		_delay_ms(2000);
	}
}

void show_menu() {
	LCD_command(0x01);
	_delay_ms(2);
	LCD_display_string("A:Add  B:Edit");
	LCD_command(0xC0);
	LCD_display_string("C:Del D:Disp #");
	_delay_ms(2000);
}

int main(void) {
	LCD_init();
	char choice;
	while (1) {
		if (check()) {
			while (1) {
				show_menu();
				LCD_command(0x01);
				_delay_ms(2);
				LCD_display_string("Enter your choice:");
				_delay_ms(1000);
				choice = keypad_scan();
				if (choice == '#') {
					LCD_command(0x01);
					_delay_ms(2);
					LCD_display_string("Goodbye!");
					_delay_ms(2000);
					break;
				}
				switch (choice) {
					case 'A': add_user(); break;
					case 'B': edit_user(); break;
					case 'C': delete_user(); break;
					case 'D': display_user(); break;
					default:
					LCD_command(0x01);
					_delay_ms(2);
					LCD_display_string("Invalid choice");
					_delay_ms(1500);
				}
			}
		}
	}
}
