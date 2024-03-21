/*
 * sensirion_uart.c
 *
 *  Created on: Mar 4, 2024
 *      Author: lgomez
 */

#include "sensirion_uart.h"
#include "stm32f4xx_hal.h"

// Definir aquí el handle de UART que se utilizará para la comunicación.
// Por ejemplo, si se está utilizando USART2, se podría tener:

extern UART_HandleTypeDef huart3; // Asume que tienes una instancia de UART_HandleTypeDef para UART3
extern UART_HandleTypeDef huart5; // Asume que tienes una instancia de UART_HandleTypeDef para UART5

int16_t sensirion_uart_select_port(uint8_t port) {
    // En un sistema con un único UART utilizado para SPS30,
    // esta función podría no ser necesaria.
    return 0;
}

int16_t sensirion_uart_open() {
    // Verificar el estado de UART5
    if (HAL_UART_GetState(&huart5) == HAL_UART_STATE_READY) {
        // UART5 está listo, procedemos a enviar mensaje por UART3
        const char * msg = "UART5 listo, inicializacion exitosa.\r\n";
        HAL_UART_Transmit(
            &huart3, (uint8_t *)msg, strlen(msg),
            HAL_MAX_DELAY); // HAL_MAX_DELAY para esperar hasta que la transmisión esté completa
        return 0;           // Éxito
    } else {
        // UART5 no está listo, enviar mensaje de error por UART3
        const char * errorMsg = "Error: UART5 no esta listo.\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)errorMsg, strlen(errorMsg), HAL_MAX_DELAY);
        return -1; // Error
    }
}

int16_t sensirion_uart_close() {
    // Cerrar el UART si es necesario.
    // En muchos proyectos, el UART se deja abierto todo el tiempo.
    return 0;
}

int16_t sensirion_uart_tx(uint16_t data_len, const uint8_t * data) {

    // Transmitir datos utilizando HAL_UART_Transmit. Asumiendo huart2 como el handle de UART.
    if (HAL_UART_Transmit_IT(&huart5, (uint8_t *)data, data_len) == HAL_OK) {
        uart_print("si transmite  2\n");
        //    	uart_print("-->");
        //    	uart_vector_print(data_len, data);
        //    	uart_print("\n");
        return data_len;
    } else {
        uart_print("no transmite 2\n");
        return -1; // Error en la transmisión
    }
}

int16_t sensirion_uart_rx(uint16_t max_data_len, uint8_t * data) {
    uart_print("<--2\n");
    uart_vector_print(max_data_len, data);
    //	uart_print("\n");
    // Recibir datos utilizando HAL_UART_Receive. Asumiendo huart5 como el handle de UART.
    if (HAL_UART_Receive_IT(&huart5, data, max_data_len) == HAL_OK) {
        //	uart_print("si recive\n");
        return max_data_len;
    } else {
        uart_print("no recive 2\n");
        return -1; // Error en la recepción
    }
}

void sensirion_sleep_usec(uint32_t useconds) {
    // Utilizar una función de delay que se ajuste a la precisión deseada. HAL_Delay trabaja en
    // milisegundos.
    HAL_Delay(useconds / 1000); // Convertir microsegundos a milisegundos
}
