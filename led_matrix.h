/*
 * led_matrix.h
 *
 *  Created on: 13.06.2019
 *      Author: jhaugk1
 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_

#define MATRIX_UPDATE_INTERVAL 64

void matrix_init();
void matrix_mcp_write(uint8_t reg, uint8_t data);
void matrix_write_row(uint8_t row, uint8_t data);
void matrix_update();
void matrix_pattern(int i,char stunde,char minute);
void matrix_music(unsigned int a,unsigned int b,unsigned int c,unsigned int d);



#endif /* LED_MATRIX_H_ */
