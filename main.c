#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "led_matrix.h"
#include "time.h"
#include "alarm_sound.h"

//Zeit Variablen
unsigned char sec = 55;
unsigned char min = 59;
unsigned char std = 23;
unsigned char day = 12;
const unsigned char month[]={30,27,30,29,30,29,30,29,30,30,29,30,29,30};
unsigned char month_index = 0;
unsigned char jahrhundert = 20;
unsigned char jahrzehnt = 18;

//Alarm Variablen
unsigned char alarm_std = 12;
unsigned char alarm_min = 0;
char status[2] = {73,79}; //Anzeige I,O
unsigned char z = 0;
unsigned char snooze = 0;
unsigned char s_delay = 0;
unsigned char s_min;
unsigned int a = 1;
unsigned int b = 5;
unsigned int c = 3;
unsigned int d = 7;

//Stell Variablen
unsigned char sec_stell;
unsigned char std_stell;
unsigned char min_stell;
unsigned char mi_stell;
unsigned char day_stell;
unsigned char jahrze_stell;
unsigned int i = 0;
unsigned char sekundentakt = 0;
unsigned char ssec_stell = 0;
unsigned char smin_stell = 1;

//Melodie
unsigned char Melodie[256] = {};
const unsigned char Melodie_1[256] = {B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,
                                      F5S,B4,D5S,B4,F5S,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,
                                      B4,G5,B4,E5,B4,G5,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,D5S,
                                      B4,F5S,B4,D5S,B4,F5S,B4,D5S,B4,F5S,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,
                                      B4,E5,B4,G5,B4,E5,B4,G5,B4,E5,B4,G5,B4,B5,B4,A5,B4,G5S,B4,A5,B4,G5S,B4,F5S,B4,G5S,B4,E5,B4,F5S,
                                      B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,B5,B4,A5,B4,G5S,B4,A5,B4,G5S,B4,F5S,B4,G5S,B4,
                                      E5,B4,F5S,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,B5,B4,A5,B4,G5S,B4,A5,B4,G5S,B4,F5S,
                                      B4,G5S,B4,E5,B4,F5S,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,B5,B4,A5,B4,G5S,B4,A5,B4,G5S,
                                      B4,F5S,B4,G5S,B4,E5,B4,F5S,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S,B4,E5,B4,D5S};

const unsigned char Melodie_2[256] = {E5,E5,D5S,D5S,E5,E5,D5S,D5S,E5,E5,B4,B4,D5,D5,C5,C5,A4,A4,A4,A4,A4,A4,C4,C4,E4,E4,A4,A4,B4,B4,B4,
                                B4,B4,B4,E4,E4,G4S,G4S,B4,B4,C5,C5,C5,C5,C5,C5,E4,E4,E5,E5,D5S,D5S,E5,E5,D5S,D5S,E5,E5,B4,B4,D5,
                                D5,C5,C5,A4,A4,A4,A4,A4,A4,C4,C4,E4,E4,A4,A4,B4,B4,B4,B4,B4,B4,E4,E4,C5,C5,B4,B4,A4,A4,A4,A4,A4,
                                A4,B4,B4,C5,C5,D5,D5,E5,E5,E5,E5,E5,E5,G4,G4,F5,F5,E5,E5,D5,D5,D5,D5,D5,D5,F4,F4,E5,E5,D5,D5,C5,
                                C5,C5,C5,C5,C5,E4,E4,D5,D5,C5,C5,B4,B4,E4,E4,E5,E5,E4,E4,E5,E5,E4,E4,E5,E5,E4,E4,E5,E5,D5S,D5S,E5,
                                E5,D5S,D5S,E5,E5,D5S,D5S,E5,E5,B4,B4,D5,D5,C5,C5,A4,A4,A4,A4,A4,A4,C4,C4,E4,E4,A4,A4,B4,B4,B4,B4,
                                B4,B4,E4,E4,G4S,G4S,B4,B4,C5,C5,C5,C5,C5,C5,E4,E4,E5,E5,D5S,D5S,E5,E5,D5S,D5S,E5,E5,B4,B4,D5,D5,
                                C5,C5,A4,A4,A4,A4,A4,A4,C4,C4,E4,E4,A4,A4,B4,B4,B4,B4,B4,B4,E4,E4,C5,C5,B4,B4,A4,A4,A4,A4,A4,A4,
                                A4,A4,A4,A4,A4,A4};
unsigned char u;
unsigned int w;
unsigned char p;

//Taster Variablen
unsigned char menu_taster = 0;
unsigned char richtung = 0;
unsigned char puffer;

//Funktionen
void zeit_stellen();
void menu();
void datum_stellen();
void alarm_stellen();
void alarm();
void snooze_dauer();
void play();
void play_off();
void sound_stellen();

// Hauptprogramm
// Initialisierung Taster, Drehencoder, TimerB, TimerA, LCD, LED Matrix
// Zeitausgabe, Menüeinstellungen, Alarmausgabe

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    P2SEL |= BIT4;              
    P2DIR |= BIT4;                                              //Ausgang
    

    //TimerA
    TACTL = TASSEL_2 + MC_2 + TACLR + TAIE + ID_3;

    //CCR und Interrupt aktiv fuer TimerB
    TBCCR0 = 32767;
    TBCCTL0 = CCIE;
    TBCCR1 = MATRIX_UPDATE_INTERVAL;
    TBCCTL1 = CCIE;
    TBCCR2 = 0x3000;
    TBCCTL2 = CCIE;
    TBCTL = TBSSEL_1 + MC_2 + TBCLR + TBIE;

    //Interrupt fuer Taster
    P2IES |= BIT0 + BIT1 + BIT2 + BIT3 + BIT5;
    P2IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT5);
    P2IE |= BIT0 + BIT1 + BIT2 + BIT3 + BIT5;

    __enable_interrupt();

    //Initialisierung
    lcd_init();
    matrix_init();

    while (1) {
            zeitausgabe(4,0,sec,min,std);
            datumsausgabe(0,1,day,month_index,jahrhundert,jahrzehnt);
            menu();
            matrix_pattern(i,std,min);
            if(i == 22)
                i = 0;
            if(z == 0){
                lcd_gotoxy(15,1);                           
                lcd_put_char(255);
            }else{
                lcd_gotoxy(15,1);
                lcd_put_char(32);
            }
            alarm();
        }




}

// Funktion um Alarmsound auszuwaehlen, durch richtung (P2.1, P2.2) auswaehlbar und mit menu_taster betaetigt
void sound_stellen(){
    richtung = 1;
    while(menu_taster == 2){
        switch(richtung){
        case 1:{
            lcd_gotoxy(0,0);                                    //Ausgabe Melodiemenue
            lcd_write("Sound einstellen");
            lcd_gotoxy(0,1);
            lcd_write("Fuer Elise      ");                      //Ausgabe Melodie
            if(menu_taster == 3){
               for(w = 0;w <= 256;w++){                         //Bestaetigen Auswahl durch menu_taster P2.0
                   Melodie[w] = Melodie_2[w];                   //gewaehlte Melodie wird in leeren Melodie Array kopiert
               }
               menu_taster = 0;
            }
        }
        case 2:{
            while(richtung == 2 && menu_taster == 2){
                lcd_gotoxy(0,1);
                lcd_write("Thunderstruck   ");                  //Ausgabe Melodie
            }if(menu_taster == 3){                              //Bestaetigen Auswahl durch menu_taster P2.0
                for(w = 0;w <= 256;w++){
                    Melodie[w] = Melodie_1[w];                  //gewaehlte Melodie wird in leeren Melodie Array kopiert
                    }
                menu_taster = 0;
            }

        }
        case 3:{                                                //Ueberlauf
            if(richtung <= 0)
                richtung = 2;
            if(richtung >= 3)
                richtung = 1;
        }
        }
    }

}

// Funktion für die Alarmausgabe
void alarm(){
    snooze = 0;
    u=0;
    if(z == 0 && alarm_std == std && alarm_min == min && sec == 0 && snooze == 0){          //Wenn Alarmzeit und Alarmstatus(z) aktiviert und Snooze deaktiviert
    while(z == 0){
        if(z == 0 && snooze == 0){                                                          //wenn Alarmstatus aktiviert und Snooze deaktiviert
            lcd_init();
            play();                                                                             //Tongeber Interruptfaehig
            while(z == 0 && snooze == 0){
                zeitausgabe(4,1,sec,min,std);                                                   //Ausgabe Uhrzeit
                if(p == 1) {
                    matrix_music(a,b,c,d);                                                      //LED Matrix Muster
                    a++;
                    b++;
                    c++;
                    d++;
                    if(a == 8) a = 0;
                    if(b == 8) b = 0;
                    if(c == 8) c = 0;
                    if(d == 8) d = 0;
                    p=0;
                }
                lcd_gotoxy(0,0);
                lcd_write("ALARM");
            }
            matrix_write_row(0,0x0);
            matrix_write_row(1,0x0);
            matrix_write_row(2,0x0);
            matrix_write_row(3,0x0);
            lcd_init();
            s_delay = 0;
            s_min = 0;
        }else if(z == 0 && snooze == 1){                             //Wenn Alarmstatus aktiviert und Snooze aktiviert
            play_off();
            while(z == 0 && snooze == 1 && (s_min < smin_stell || s_delay < ssec_stell)){    //Waehrend Alarmstatus aktiviert und Snooze aktiviert und Snoozedauer nicht abgelaufen ist
                zeitausgabe(4,1,sec,min,std);
                lcd_gotoxy(0,0);
                lcd_write("Snooze aktiviert");
            }
            snooze = 0;                                             //Reset Variablen
            s_delay = 0;
            lcd_init();
        }
        menu_taster = 0;
        i = 0;
    }
    z = 0;
    snooze = 0;
    play_off();                                                     //Tongeber nicht Interruptfaehig
    }
}

// Menueausgabe zum Einstellen von Uhrzeit, Datum, Alarmzeit, Snoozedauer, Alarmsound
// Durchlaufbar mit richtung, bestaetigt durch menu_taster
void menu(){
    if(menu_taster == 1){                                       //Initialisierung
            lcd_init();
            richtung = 1;
            i = 0;
            matrix_write_row(0,0x0);
            matrix_write_row(1,0x0);
            matrix_write_row(2,0x0);
            matrix_write_row(3,0x0);

   }
        while(menu_taster==1){                              
            switch (richtung){
            case 1:{
                while(richtung == 1 && menu_taster == 1){       //Allgemeine Ausgabe
                    lcd_gotoxy(0,0);
                    lcd_write("Einstellungen   ");
                    lcd_gotoxy(0,1);
                    lcd_put_char(65);                               //Anzeige A
                    lcd_gotoxy(2,1);
                    lcd_put_char(84);                               //Anzeige T
                    lcd_gotoxy(4,1);
                    lcd_put_char(68);                               //Anzeige D
                    lcd_gotoxy(6,1);
                    lcd_put_char(83);                               //Anzeige S
                    lcd_gotoxy(8,1);
                    lcd_put_char(90);                               //Anzeige Z
                    lcd_gotoxy(15,1);
                    lcd_put_char(69);                               //Anzeige E Exit
                }
                if(menu_taster == 2)                                //Auswahl durch menu_taster bestaetigt ---> Funktion verlassen
                    lcd_init();
                break;

            }
            case 2:{
                while(richtung == 2 && menu_taster == 1){       //Alarm stellen
                    lcd_gotoxy(0,0);
                    lcd_write("Alarm stellen   ");
                    lcd_gotoxy(0,1);
                    blink(sekundentakt,65);                     //A blinkt
                }
                if(menu_taster == 2){                           //Auswahl durch menu_taster bestaetigt ---> alarm_stellen()
                    lcd_init();
                    alarm_stellen();
                }
                lcd_gotoxy(0,1);
                lcd_put_char(65);
                i=0;
                break;
            }
            case 3:{
                while(richtung == 3 && menu_taster == 1){       //Zeit stellen
                    lcd_gotoxy(0,0);
                    lcd_write("Zeit einstellen ");
                    lcd_gotoxy(2,1);                    
                    blink(sekundentakt, 84);                        //Z blinkt
                }
                if(menu_taster == 2){                           //Auswahl durch menu_taster bestaetigt ---> zeit_stellen()
                    lcd_init();
                    zeit_stellen();
                }
                lcd_gotoxy(2,1);
                lcd_put_char(84);
                i=0;
                break;
            }
            case 4:{
                while(richtung == 4 && menu_taster == 1){       //Datum stellen
                    lcd_gotoxy(0,0);
                    lcd_write("Datum einstellen");
                    lcd_gotoxy(4,1);            
                    blink(sekundentakt, 68);                        //D blinkt
                }
                if(menu_taster == 2){                           //Auswahl durch menu_taster bestaetigt ---> datum_stellen()
                    lcd_init();
                    datum_stellen();
                }
                lcd_gotoxy(4,1);
                lcd_put_char(68);
                i = 0;
                break;
            }
            case 5:{
                while(richtung == 5 && menu_taster == 1){       //Alarmsound waehlen
                    lcd_gotoxy(0,0);
                    lcd_write("Sound einstellen");
                    lcd_gotoxy(6,1);
                    blink(sekundentakt, 83);                    //S blinkt
                }
                if(menu_taster == 2){                           //Auswahl durch menu_taster bestaetigt ---> sound_stellen()
                    lcd_init();
                    sound_stellen();
                    lcd_init();
                }
                lcd_gotoxy(6,1);
                lcd_put_char(83);
                i = 0;
                break;
            }
            case 6:{
                while(richtung == 6 && menu_taster == 1){       //Snoozedauer stellen
                    lcd_gotoxy(0,0);
                    lcd_write("Snoozedauer     ");      
                    lcd_gotoxy(8,1);    
                    blink(sekundentakt, 90);                    //Z blinkt
                }
                if(menu_taster == 2){                           //Auswahl durch menu_taster bestaetigt ---> snooze_dauer()
                    lcd_init();
                    snooze_dauer();
                    lcd_init();
                }
                lcd_gotoxy(8,1);
                lcd_put_char(90);
                break;
            }
            case 7:{
                while(richtung == 7 && menu_taster == 1){       //Menue verlassen
                    lcd_gotoxy(0,0);
                    lcd_write("Exit            ");
                    lcd_gotoxy(15,1);                           //E blinkt
                    blink(sekundentakt, 69);
                }
                if(menu_taster == 2){                           //Auswahl durch menu_taster bestaetigt ---> Funktion verlassen
                    lcd_init();
                }
                lcd_gotoxy(15,1);
                lcd_put_char(69);
                i = 0;
                break;
            }
            default:                                            //Ueberlauf
                if(richtung >= 8){
                    richtung = 1;
                }else if(richtung <= 0){
                    richtung = 7;
                }
            }

        }
    if(menu_taster == 2){
        lcd_init();
        i=0;
    }
    menu_taster = 0;

}

//Snoozedauer einstellen
//Auswahl ob Minute, Sekunde mit richtung, Einstellen der Werte durch Drehencoder, bestaetigen durch menu_taster

void snooze_dauer(){
    richtung = 1;
    while(menu_taster == 2){
        switch(richtung){
        case 1:{
            lcd_gotoxy(0,0);                                    //Allgemeine Ausgabe
            lcd_write("Snoozedauer     ");
            lcd_gotoxy(0,1);
            lcd_zahl(smin_stell);
            lcd_put_char(109);                                  //Anzeige m
            lcd_zahl(ssec_stell);
            lcd_put_char(115);                                  //Anzeige s
        }
        case 2:{
            puffer = smin_stell;                                //Minuten stellen
            while(richtung == 2 && menu_taster == 2){   
                if(puffer == 60)                                
                    puffer = 0;
                if(puffer == 255)
                    puffer = 59;
                smin_stell = puffer;                            //Minute durch Drehencoder (puffer) einstellbar
                lcd_gotoxy(0,1);
                lcd_zahl(smin_stell);                           //Ausgabe der Einstellung
                lcd_gotoxy(2,1);
                blink(sekundentakt, 109);                       //h blinkt
            }
            lcd_gotoxy(2,1);
            lcd_put_char(109);
        }
        case 3:{
            puffer = sec_stell;                                 //Sekunden stellen
            while(richtung == 3 && menu_taster == 2){           
                if(puffer == 60)
                    puffer = 0;
                if(puffer == 255)
                    puffer = 59;
                ssec_stell = puffer;                            //Sekunde durch Drehencoder (puffer) einstellbar
                lcd_gotoxy(3,1);
                lcd_zahl(ssec_stell);                           //Ausgabe Einstellung
                lcd_gotoxy(5,1);
                blink(sekundentakt,115);                        //s blinkt
            }
            lcd_gotoxy(5,1);
            lcd_put_char(115);
        }
        default:
            if(richtung <= 0){                              //Ueberlauf
                richtung = 3;
            }if(richtung >= 4){
                richtung = 2;
        }
        }
    }
}

//Uhrzeit einstellen
//Auswahl ob Stunde, Minute, Sekunde mit richtung, Einstellen der Werte durch Drehencoder, bestaetigen durch menu_taster
void zeit_stellen(){
    richtung = 1;
    std_stell = std;                                            //Stellvariablen mit aktuellen Wert
    min_stell = min;
    sec_stell = sec;
    while(menu_taster == 2){
       switch(richtung){
       case 1: while(richtung == 1 && menu_taster == 2){        //Allgemeine Ausgabe
               lcd_gotoxy(0,0);
               lcd_write("Zeit einstellen ");
               lcd_gotoxy(3,1);
               lcd_zahl(std_stell);
               lcd_put_char(104);                               //Anzeige h
               lcd_zahl(min_stell);
               lcd_put_char(109);                               //Anzeige m
               lcd_zahl(sec_stell);
               lcd_put_char(115);                               //Anzeige s
               break;
       }
       case 2:  puffer = std_stell;                             //Stunde stellen
           while(richtung == 2 && menu_taster == 2){
               if(puffer == 24)
                   puffer = 0;
               if(puffer == 255)
                   puffer = 23;
               std_stell = puffer;                              //Stunde durch Drehencoder (puffer) einstellbar
               lcd_gotoxy(3,1);
               lcd_zahl(std_stell);                             //Ausgabe Einstellung
               blink(sekundentakt, 104);                        //h blinkt
           }
           lcd_gotoxy(5,1);
           lcd_put_char(104);                                   //Anzeige h
           std = std_stell;                                     //Stunde hat nun Stellwert
           break;
       case 3:   puffer = min_stell;                            //Minute stellen
           while(richtung == 3 && menu_taster == 2){
               if(puffer == 60)
                   puffer = 0;
               if(puffer == 255)
                   puffer = 59;
               min_stell = puffer;                              //Minute durch Drehencoder (puffer) einstellbar
               lcd_gotoxy(6,1);
               lcd_zahl(min_stell);                             //Ausgabe Einstellung
               blink(sekundentakt, 109);                        //m blinkt
           }
           lcd_gotoxy(8,1);
           lcd_put_char(109);                                   //Anzeige m
           min = min_stell;                                     //Minute hat nun Stellwert
           break;   
       case 4: puffer = sec_stell;                              //Sekunde stellen
           while(richtung == 4 && menu_taster == 2){
               if(puffer == 60)
                 puffer = 0;
               if(puffer == 255)
                 puffer = 59;
               sec_stell = puffer;                              //Sekunde durch Drehencoder (puffer) einstellbar
               lcd_gotoxy(9,1);
               lcd_zahl(sec_stell);                             //Ausgabe Einstellung
               blink(sekundentakt, 115);                        //s blinkt
           }
           lcd_gotoxy(11,1);
           lcd_put_char(115);                                   //Anzeige s
           sec = sec_stell;                                     //Sekunde hat nun Stellwert
           break;
       default:                                                 //Ueberlauf
       if(richtung >= 5){
           richtung = 2;
       }else if(richtung <= 1){
           richtung = 4;
       }
       }
    }
    lcd_init();
}
//Datum einstellen
//Auswahl ob Jahr, Monat, Tag mit richtung, Einstellen der Werte durch Drehencoder, bestaetigen durch menu_taster
void datum_stellen(){
    richtung = 1;
    day_stell = day;
    mi_stell = month_index;
    jahrze_stell = jahrzehnt;
    while(menu_taster == 2){
        switch(richtung){
        case 1:                                                 //Allgemeine Ausgabe
            while(richtung == 1 && menu_taster == 2){           
                lcd_gotoxy(0,0);    
                lcd_write("Datum einstellen");
                lcd_gotoxy(0,1);
                lcd_zahl(day+1);                                //Ausgabe Tag
                lcd_put_char(46);                               //Anzeige .
                lcd_gotoxy(3,1);    
                lcd_zahl(month_index+1);                        //Ausgabe Monat
                lcd_put_char(46);                               //Ausgabe .
                lcd_gotoxy(6,1);
                lcd_zahl(jahrhundert);                          //Ausgabe Jahr (Jahrhundert)
                lcd_zahl(jahrzehnt);                            //Ausgabe Jahr (Jahrzehnt)
            }
        case 2:{
            puffer = jahrze_stell;                          //Jahr einstellen
            while(richtung == 2 && menu_taster == 2){
                if(puffer == 100){
                    puffer = 0;
                    jahrhundert++;
                }
                if(puffer == 255){
                    puffer = 99;
                    jahrhundert--;
                }
                jahrze_stell = puffer;                          //Jahr (Jahrzehnt) durch Drehencoder einstellbar
                lcd_gotoxy(0,1);
                lcd_write("Jahr: ");
                lcd_gotoxy(6,1);
                lcd_zahl(jahrhundert);                          //Ausgabe Jahr (Jahrhundert)
                lcd_zahl(jahrze_stell);                         //Ausgabe Jahr (Jahrzehnt)
            }
            jahrzehnt = jahrze_stell;                           //Jahr hat nun Stellwert

        }
        case 3:{                                                //Monat einstellen
            puffer = mi_stell;
            while(richtung == 3 && menu_taster == 2){
                if(puffer == 12){
                    puffer = 0;
                }
                if(puffer == 255){
                    puffer = 11;
                }
                mi_stell = puffer;                              //Monatsindex durch Drehencoder einstellbar
                lcd_gotoxy(0,1);
                lcd_write("Monat: ");
                lcd_gotoxy(7,1);
                lcd_zahl(mi_stell+1);                           //Ausgabe Monat
                lcd_write("       ");
            }       
            month_index = mi_stell;                             //Monatsindex hat nun Stellwert
        }
        case 4:{                    
            puffer = day_stell;                                 //Tag einstellen
            while(richtung == 4 && menu_taster == 2){
                if(puffer == month[month_index]+1){
                    puffer = 0;
                }
                if(puffer == 255){
                    puffer = month[month_index];
                }
                day_stell = puffer;                             //Tag durch Drehencoder einstellbar
                lcd_gotoxy(0,1);
                lcd_write("Tag: ");
                lcd_gotoxy(5,1);
                lcd_zahl(day_stell+1);                          //Ausgabe Tag
                lcd_write("         ");
            }
            day = day_stell;                                    //Tag hat nun Stellwert
            break;
        }
        default: 
            if(richtung >= 5){                             //Ueberlauf
                richtung = 2;
            }
            if(richtung <= 1){
                richtung = 4;
            }
        }
    }
    lcd_init();
}
//Alarmzeit stellen
//Auswahl ob Stunde, Minute mit richtung, Einstellen der Werte durch Drehencoder, bestaetigen durch menu_taster
void alarm_stellen(){
    richtung = 1;
    while(menu_taster == 2){
        switch(richtung){
        case 1:{                                                //Allgemeine Ausgabe
            while(richtung==1 && menu_taster == 2){
                lcd_gotoxy(0,0);
                lcd_write("Alarm stellen   ");
                lcd_gotoxy(0,1);
                lcd_zahl(alarm_std);                            //Ausgabe Alarmstunde
                lcd_put_char(104);                              //Anzeige h
                lcd_zahl(alarm_min);                            //Ausgabe Minute
                lcd_put_char(109);                              //Anzeige m
                lcd_gotoxy(7,1);
                lcd_write("Status: ");                          
                lcd_gotoxy(15,1);
                lcd_put_char(status[z]);                        //Alarmstatus
            }
        }
        case 2:{                                                //Alarmminute stellen
            puffer = alarm_std;
            while(richtung == 2 && menu_taster == 2){
                if(puffer == 24)
                    puffer = 0;
                if(puffer == 255)
                    puffer = 23;
                alarm_std = puffer;                             //Alarmstunde duchr Drehencoder einstellbar
                lcd_gotoxy(0,1);
                lcd_zahl(alarm_std);                            //Ausgabe Alarmstunde
                blink(sekundentakt, 104);                       //h blinkt
            }
            lcd_gotoxy(2,1);
            lcd_put_char(104);                                  
        }
        case 3:{                                                //Alarmminute einstellen
            puffer = alarm_min;
            while(richtung == 3 && menu_taster == 2){
                if(puffer == 60)
                    puffer = 0;
                if(puffer == 255)
                    puffer = 59;    
                alarm_min = puffer;                             //Alarmminute durch Drehencoder einstellbar
                lcd_gotoxy(3,1);
                lcd_zahl(alarm_min);                            //Ausgabe Alarmminute
                blink(sekundentakt, 109);                       //s blinkt
            }
            lcd_gotoxy(5,1);
            lcd_put_char(109);
        }
        case 4:{                                                //Alarmstatus stellen
            while(richtung == 4 && menu_taster == 2){
                if(puffer <= 254 && puffer >= 250)
                    z = 0;
                else if(puffer <= 249 && puffer >= 245)
                    z = 1;
                    else if(puffer <= 244)
                        puffer = 254;
                        else if(puffer == 255)
                            puffer = 245;
                lcd_gotoxy(15,1);
                blink(sekundentakt, status[z]);                 //Alarmstatus blinkt
            }
            lcd_gotoxy(15,1);
            lcd_put_char(status[z]);                        
        }
        default:                                                //Ueberlauf
            if(richtung >= 5){
                richtung = 1;
            }else if(richtung < 1){
                richtung = 4;
            }
        }
    }
    lcd_init();
}

//Aktivierung Toongeber Interrupt
void play(){
    CCTL0 = CCIE;
    CCTL2 = CCIE + OUTMOD_4;
    CCR0 = 16900;
    CCR2 = Melodie[u];
}
//Deaktivierung Tongeber Interrupt
void play_off(){
    CCTL0 &= ~CCIE;
    CCTL2 &= ~(CCIE + OUTMOD_4);
}


//TimerB Interrupt CCR0
#pragma vector=TIMERB0_VECTOR
__interrupt void TimerB0_ISR(void)
{
    TBCCR0 += 32767;
    i++;                                                        //hochzählen von Zeit und i und Snoozedauer
    if(s_delay <= 58){
        s_delay++;
    }
        else{
            s_delay = 0;
            if(s_min <= 58){
                s_min++;
            }else{
                s_min = 0;
            }
        }

    if (sec<=58){
        sec++;
    }else{
        sec = 0;
        if(min<=58){
            min++;
        }else{
            min = 0;
        if(std<=22){
            std++;
        }
            else{
                std = 0;
            if(day <= month[month_index]-1)
            {
                day++;
            }else {
                day = 0;
                if(month_index <= 10){
                    month_index++;
                }
                else{
                    month_index = 0;
                    if(jahrzehnt <= 98){
                        jahrzehnt++;
                    }
                    else{
                        jahrhundert++;
                    }
                }
            }
            }
        }
    }
}

//TimerB Interrupt CCR1, CCR2
#pragma vector=TIMERB1_VECTOR
__interrupt void TimerB1_ISR(void)
{   switch (TBIV){
    case 2:{
        TBCCR1 += MATRIX_UPDATE_INTERVAL;                       //Matrix Multiplexing
        matrix_update();                                            
        break;
        }
    case 4:{
       TBCCR2 += 0x3000;                                        
       sekundentakt ^= 1;                                       //toggelt
        break;
    }
    case 14:{
        break;
        }
    }
}

//Taster, Drehencoder Interrupt
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR() {
    if(P2IFG & BIT3){                                           //Drehencoder
            P2IFG &= ~BIT3;                                     //Reset Interruptflag

            if(P4IN & BIT1 && !(P4IN & BIT2)){
                puffer--;
            }else if(P4IN & BIT2 && !(P4IN & BIT1)){
                puffer++;
            }
        }else{
            __delay_cycles(1000);
            if (P2IFG & BIT0) {
                P2IFG &= ~BIT0;                                 //Reset Interruptflag
                snooze ^= 1;                                    //Snoozetaster
                menu_taster++;                                  //Menuetaster
                if(menu_taster == 4){
                    menu_taster = 0;
                    }
            }       
            if(P2IFG & BIT1){                                   //Richtungstaster
                P2IFG &= ~BIT1;                                 //Reset Interruptflag
                richtung++;                 
            }
            if(P2IFG & BIT2){
                P2IFG &= ~BIT2;                                 //Reset Interruptflag
                richtung--;
            }
            if(P2IFG & BIT5){                                   //Alarmtaster
                P2IFG &= ~BIT5;                                 //Reset Interruptflag
                z ^= 1;
            }
        }
}

//TimerA Interrupt CCR0
#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0_ISR(void)
{
    CCR0 += 16900;  
    u++;
    p++;
    if(u==256) u=0;
}

//TimerA Interrupt CCR2
#pragma vector=TIMERA1_VECTOR
__interrupt void TimerA1_ISR() {
    switch (TAIV) {
    case 2:
        // CCR1 Interrupt
        break;
    case 4:
        // CCR2 Interrupt
        CCR2 += Melodie[u];
        break;
    case 10:
        // Timer Overflow
        break;
    }
}
