/*
 * rs485.h
 *
 *  Created on: 29/03/2026
 *      Author: thila
 */

#ifndef SRC_RS485_H_
#define SRC_RS485_H_

#include <main.h>
#include <usart.h>

#define RS485_DE_PORT GPIOD
#define RS485_DE_PIN  GPIO_PIN_7

extern uint8_t rs_485_rx_buffer[32];
/**
 * Function Prototypes
 */
void RS485_SetTransmitMode(void);
void RS485_SetReceiveMode(void);
void RS485_Send(uint8_t *data, uint16_t len);
void RS485_Receive();

#endif /* SRC_RS485_H_ */
