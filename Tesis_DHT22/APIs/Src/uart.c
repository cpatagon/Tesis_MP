/*
 * uart.c
 *
 *  Created on: Mar 4, 2024
 *      Author: lgomez
 */
#include "stm32f4xx_hal.h"
#include "uart.h"
#include <string.h> // Para usar memset y strlen
#include <stdio.h>  // Para usar snprintf

extern UART_HandleTypeDef huart3; // Asume que huart3 está definido en otro lugar

// Función personalizada para enviar mensajes a través de UART3
void uart_print(const char *message) {
    HAL_UART_Transmit(&huart3, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
}

void uart_vector_print(uint16_t data_len, const uint8_t* data) {
    char buffer[512]; // Aumenta el tamaño si es necesario
    memset(buffer, 0, sizeof(buffer)); // Limpia el buffer
    char *ptr = buffer; // Puntero para la posición actual en el buffer
    int remaining = sizeof(buffer); // Espacio restante en el buffer
    for (uint16_t i = 0; i < data_len; ++i) {
        int written = snprintf(ptr, remaining, "%02X ", data[i]);
        ptr += written; // Avanza el puntero
        remaining -= written; // Decrementa el espacio restante
        if (remaining <= 0) break; // Evita desbordamiento del búfer
    }
    uart_print(buffer); // Imprimir todos los bytes formateados
}

