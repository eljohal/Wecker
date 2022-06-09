#include <msp430.h>
#include <stdint.h>

#include "led_matrix.h"

static const uint8_t MCP_ADDRESS = 0x20;    //!> 7-bit Adresse des MCP23008
static const uint8_t MCP_IODIR = 0x00;      //!> Adresse des MCP23008-Registers IODIR
static const uint8_t MCP_GPIO = 0x09;       //!> Adresse des MCP23008-Registers GPIO

static char pattern[10][4] = { {0x0,0xF,0x9,0xF}, {0x0,0xA,0xF,0x8}, {0x0,0xD,0xB,0xB}, {0x0,0x9,0xD,0xF}, {0x0,0x7,0x4,0xF}, {0x0,0xB,0xB,0xD}, {0x0,0xF,0xD,0xC}, {0x0,0x1,0x5,0xF}, {0x0,0xF,0xD,0xF}, {0x0,0xB,0xB,0xF} };
static char music[8] = {0x0,0x8,0xC,0xE,0xF,0xE,0xC,0x8};
static char i2c_data[2];             //!> Datenpuffer
static volatile uint8_t i2c_counter; //!> Index der Daten
static uint8_t matrix_buffer[4];         //!> Werte von GPIO beim Multiplexing
static uint8_t current_row;          //!> Aktueller Index von matrix_buffer
unsigned int o = 0;

/**
 * Initialisierung des I2C-Moduls
 * Initialsierung des MCP23008
 * Löschen des Matrix-Puffers
 */
void matrix_init() {
    P3DIR |= BIT1 + BIT2;
    P3OUT &= ~(BIT1 + BIT2);

    P3SEL |= BIT1 + BIT2;   // I2C-Pins

    UCB0CTL1 |= UCSWRST;                // Soft-Reset des I2C-Moduls
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // Master, I2C, Synchron
    UCB0BR0 = 2;    // Teiler: 1 MHz /2 = 500 kHz
    UCB0BR1 = 0;

    UCB0CTL1 = UCSSEL_2;

    IE2 |= UCB0TXIE;

    uint8_t row;
    // Löschen der Anzeige
    for (row = 0; row < 4; row++){
        matrix_write_row(row, 0);
    }
    current_row = 0;

    matrix_mcp_write(MCP_IODIR, 0);     // Alle Pins des MCP23008 als Ausgang
}

/**
 * Senden von Daten an den MCP23008
 *
 * @param reg Register, das geschrieben werden soll
 * @param data Daten, die ins Register geschrieben werden sollen
 */
void matrix_mcp_write(uint8_t reg, uint8_t data) {

    if (i2c_counter == 0)
    {
        // Warten, bis Modul frei
        while((UCB0STAT & UCBBUSY));

        // Puffer füllen
        i2c_data[0] = reg;
        i2c_data[1] = data;
        //i2c_counter = 0;

        UCB0I2CSA = MCP_ADDRESS; // Slave Adresse setzen
        UCB0CTL1 |= UCTR;        // W-Mode
        UCB0CTL1 |= UCTXSTT;     // Transmition Start

    }


}


/**
 * Schreiben des Buffers einer Spalte
 *
 * @param row Spalte 0...3
 * @param data Daten, Zustand der LEDs [3:0]
 */
void matrix_write_row(uint8_t row, uint8_t data) {
    matrix_buffer[row] = ((~data) << 4) + (1 << row);
}


/**
 * Funktion, die durch eine Timer aufgerufen werden muss, um das Multiplexing der Matrix druchzuführen
 */
void matrix_update() {
    matrix_mcp_write(MCP_GPIO, matrix_buffer[current_row]);
    current_row = (current_row + 1) & 0x03;
}

//Laufschrift auf LED Matrix

void matrix_pattern(int i,char stunde,char minute){

    char Ziffer_1 = stunde/10;
    char Ziffer_2 = stunde - Ziffer_1*10;
    char Ziffer_3 = minute/10;
    char Ziffer_4 = minute - Ziffer_3*10;


    int j = i-4;

    if (i >= 3 && i <= 6) matrix_write_row(0,pattern[Ziffer_1][i-3]);
    if (i >= 2 && i <= 5) matrix_write_row(1,pattern[Ziffer_1][i-2]);
    if (i >= 1 && i <= 4) matrix_write_row(2,pattern[Ziffer_1][i-1]);
    if (i >= 0 && i <= 3) matrix_write_row(3,pattern[Ziffer_1][i]);

    if (i >= 7 && i <= 10) matrix_write_row(0,pattern[Ziffer_2][j-3]);
    if (i >= 6 && i <= 9) matrix_write_row(1,pattern[Ziffer_2][j-2]);
    if (i >= 5 && i <= 8) matrix_write_row(2,pattern[Ziffer_2][j-1]);
    if (i >= 4 && i <= 7) matrix_write_row(3,pattern[Ziffer_2][j]);

    if(i == 8) matrix_write_row(3, 0x0);
    if (i == 9) {
        matrix_write_row(2, 0x0);
        matrix_write_row(3, 0xA);
    }
    if (i == 10) {
        matrix_write_row(1, 0x0);
        matrix_write_row(2, 0xA);
    }
    if (i == 11) {
        matrix_write_row(0, 0x0);
        matrix_write_row(1, 0xA);
    }
    if(i == 12) matrix_write_row(0, 0xA);

    int k = i-10;
    int h = k-4;

    if (k >= 3 && k <= 6) matrix_write_row(0,pattern[Ziffer_3][k-3]);
    if (k >= 2 && k <= 5) matrix_write_row(1,pattern[Ziffer_3][k-2]);
    if (k >= 1 && k <= 4) matrix_write_row(2,pattern[Ziffer_3][k-1]);
    if (k >= 0 && k <= 3) matrix_write_row(3,pattern[Ziffer_3][k]);

    if (k >= 7 && k <= 10) matrix_write_row(0,pattern[Ziffer_4][h-3]);
    if (k >= 6 && k <= 9) matrix_write_row(1,pattern[Ziffer_4][h-2]);
    if (k >= 5 && k <= 8) matrix_write_row(2,pattern[Ziffer_4][h-1]);
    if (k >= 4 && k <= 7) matrix_write_row(3,pattern[Ziffer_4][h]);

    if(k == 8) matrix_write_row(3, 0x0);
    if (k == 9) {
        matrix_write_row(2, 0x0);
        matrix_write_row(3, 0x0);
    }
    if (k == 10) {
        matrix_write_row(1, 0x0);
        matrix_write_row(2, 0x0);
        matrix_write_row(3, 0x0);
    }
    if (k == 11) {
        matrix_write_row(0, 0x0);
        matrix_write_row(1, 0x0);
        matrix_write_row(2, 0x0);
        matrix_write_row(3, 0x0);
    }






}

// Anzeige auf LED Matrix während Alarm

void matrix_music(unsigned int a,unsigned int b,unsigned int c,unsigned int d) {

    matrix_write_row(0, music[a]);
    matrix_write_row(1, music[b]);
    matrix_write_row(2, music[c]);
    matrix_write_row(3, music[d]);

}

/**
 * I2C-Transmit-Interrupt
 */
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
    if (i2c_counter < 2) {
        // Nächtes Byte senden
        UCB0TXBUF = i2c_data[i2c_counter];
        i2c_counter++;
    } else {
        // Alles Bytes versendet
        UCB0CTL1 |= UCTXSTP;    // Stop-Senden
        IFG2 &= ~UCB0TXIFG;     // Interrupt-Flag zurücksetzen
        i2c_counter = 0;
    }
}
