/*
 * rs485.c
 *
 *  Created on: 29/03/2026
 *      Author: Thilanga Ariyarathna
 */
#include "rs485.h"

uint8_t rs_485_rx_buffer[32];


void RS485_SetTransmitMode(void){
	HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_SET);
}


void RS485_SetReceiveMode(void){
	HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
}

void RS485_Send(uint8_t *data, uint16_t len){

	RS485_SetTransmitMode();

	HAL_Delay(1);

	HAL_UART_Transmit(&huart2, data, len, 100);

		// wait for transmission complete
	while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET);

	RS485_SetReceiveMode();
}


void RS485_Receive(){
	RS485_SetReceiveMode();

	if(HAL_UART_Receive(&huart2, rs_485_rx_buffer, sizeof(rs_485_rx_buffer), 100) == HAL_OK){
	        rs_485_rx_buffer[31] = '\0';
	}
}
