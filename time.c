/*
 * time.c
 *
 *  Created on: 14.06.2019
 *      Author: jhaugk1
 */
#include <msp430.h>
#include <stdint.h>
#include "lcd.h"

const char wochentag[7][2]={"So","Mo","Di","Mi","Do","Fr","Sa"};
int wochentag_index;

void zeitausgabe(unsigned char x, unsigned char y, char sekunde, char minute, char stunde){
    lcd_gotoxy(x,y);
    lcd_zahl(stunde);
    lcd_put_char(58);
    lcd_zahl(minute);
    lcd_put_char(58);
    lcd_zahl(sekunde);
    lcd_write(" Uhr");
}

int wochentagberechnung(char day, char month, char cen, char dec){
    uint8_t mon_rech = 0;
        if( month == 0){
            mon_rech = 11;
            dec = dec - 1;
        }else if(month == 1){
            mon_rech = 12;
            dec = dec - 1;
        }else{
            mon_rech = month - 1;
        }
      uint8_t mon = (2.6*(mon_rech)-0.2);
      uint8_t d = dec/4;
      uint8_t ce = cen/4;


      wochentag_index = ((day+1) + mon + dec + d + ce - 2*cen) % 7;
      return (wochentag_index);


}

void datumsausgabe(unsigned char x, unsigned char y, char day, char month, char cen, char dec){
    lcd_gotoxy(x,y);

    wochentagberechnung(day,month,cen,dec);
    lcd_put_char(wochentag[wochentag_index][0]);
    lcd_put_char(wochentag[wochentag_index][1]);
    lcd_put_char(32);
    lcd_zahl(day+1);
    lcd_put_char(46);
    lcd_zahl(month+1);
    lcd_put_char(46);
    lcd_zahl(cen);
    lcd_zahl(dec);
}



