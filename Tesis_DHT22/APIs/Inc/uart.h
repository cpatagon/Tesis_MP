/*
 * uart.h
 *
 *  Created on: Mar 4, 2024
 *      Author: lgomez
 */

#ifndef INC_UART_H_
#define INC_UART_H_

// Función personalizada para enviar mensajes a través de UART3
void uart_print(const char * message);
void uart_vector_print(uint16_t data_len, const uint8_t * data);

#endif /* INC_UART_H_ */
