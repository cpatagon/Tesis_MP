/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_uart_hal.h"
#include "sensirion_common.h"
#include "sensirion_config.h"

#include "stm32f4xx_hal.h" // Asegúrate de ajustar esto para tu familia STM32 específica
extern UART_HandleTypeDef huart5; // Asegúrate de que esto coincide con tu configuración de UART
extern UART_HandleTypeDef huart3;

int16_t sensirion_uart_hal_init() {
    // La inicialización de UART generalmente se hace en el código de inicio o en main,
    // pero si necesitas configurar algo específico aquí, hazlo.
    // Por ejemplo, verificar que huart3 está listo para ser usado:
    if (HAL_UART_GetState(&huart5) == HAL_UART_STATE_READY) {
        return 0; // UART listo para ser usado
    }
    return -1; // UART no está listo o no disponible
}

int16_t sensirion_uart_hal_free() {
    // Liberar recursos de UART si es necesario. En muchos casos, esto no será necesario.
    // Esta función podría no hacer nada en tu implementación.
    return 0;
}

int16_t sensirion_uart_hal_tx(uint16_t data_len, const uint8_t* data) {
    // Transmitir datos usando HAL_UART_Transmit
	uart_print(" (1) -->");
	uart_vector_print(data_len, data);
//	uart_print("\n");
    if (HAL_UART_Transmit_IT(&huart5, (uint8_t *)data, data_len) == HAL_OK) {
    	//uart_print("si transmite 1\n");
        return data_len; // Retornar el número de bytes transmitidos
    }
    uart_print("no transmite 1\n");
    return -1; // Error en la transmisión
}

int16_t sensirion_uart_hal_rx(uint16_t max_data_len, uint8_t* data) {
	max_data_len=100;
    // Recibir datos usando HAL_UART_Receive
	// memset(data, 0, max_data_len);
    if (HAL_UART_Receive_IT(&huart5, (uint8_t *)data,max_data_len) == HAL_OK) {

   	uart_print("si recive 1\n");
    	uart_print("<-- (1)");
    	uart_vector_print(max_data_len,data);
    	uart_print("\n");
        return max_data_len; // Puede necesitar ajustar basado en bytes reales recibidos
    }
    else{
    uart_print("no recive 1\n");
//    uart_print(HAL_UART_GetError(&huart5));
    return -1; // Error en la recepción
    }
}

void sensirion_uart_hal_sleep_usec(uint32_t useconds) {
    // Convertir microsegundos a milisegundos, ya que HAL_Delay utiliza ms
    uint32_t ms = useconds / 1000;
    HAL_Delay(ms);
}
