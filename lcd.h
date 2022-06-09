/*
 * lcd.h
 *
 *  Created on: 06.06.2019
 *      Author: jhaugk1
 */

#ifndef LCD_H_
#define LCD_H_

void lcd_init();
void lcd_gotoxy(uint8_t x, uint8_t y);
void blink(unsigned char sec_takt, unsigned char c);
void lcd_put_char(char c);
void lcd_zahl(char zahl);
void lcd_write(const char *str);




#endif /* LCD_H_ */
