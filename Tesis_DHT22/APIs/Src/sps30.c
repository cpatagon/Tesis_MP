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

#include "sps30.h"
#include "sensirion_shdlc.h"
#include "sensirion_uart.h"
#include "sps_git_version.h"
#include "usart.h"

#define SPS30_ADDR 0x00
#define SPS30_CMD_START_MEASUREMENT 0x00
#define SPS30_CMD_STOP_MEASUREMENT 0x01
#define SPS30_SUBCMD_MEASUREMENT_START \
    { 0x01, 0x03 }
#define SPS30_CMD_READ_MEASUREMENT 0x03
#define SPS30_CMD_SLEEP 0x10
#define SPS30_CMD_WAKE_UP 0x11
#define SPS30_CMD_FAN_CLEAN_INTV 0x80
#define SPS30_CMD_FAN_CLEAN_INTV_LEN 5
#define SPS30_SUBCMD_READ_FAN_CLEAN_INTV 0x00
#define SPS30_CMD_START_FAN_CLEANING 0x56
#define SPS30_CMD_DEV_INFO 0xd0
#define SPS30_CMD_DEV_INFO_SUBCMD_GET_SERIAL \
    { 0x03 }
#define SPS30_CMD_READ_VERSION 0xd1
#define SPS30_CMD_RESET 0xd3
#define SPS30_ERR_STATE(state) (SPS30_ERR_STATE_MASK | (state))

const char* sps_get_driver_version(void) {
    return SPS_DRV_VERSION_STR;
}

// Función para iniciar la medición en el SPS30
void SPS30_StartMeasurement(void) {
    // Comando para iniciar la medición en modo UART (Reemplaza con el comando real)
    uint8_t startCmd[] = {0x7E, 0x00, 0x00, 0x02, 0x01, 0x03, 0xF9, 0x7E};
    uart_print("\n Inicio_medicion:");
    uart_vector_print(8,startCmd);
    uint8_t dataBuf[8]; // Asegúrate de que este buffer sea adecuado para los datos
    HAL_UART_Transmit(&huart5, startCmd, sizeof(startCmd), 100);
    HAL_Delay(2000); // Espera para que el sensor procese el comando
    HAL_UART_Receive_IT(&huart5, dataBuf, sizeof(dataBuf)); // Ajusta el timeout según sea necesario
    uart_print("\n Respuesta inicio:");
    uart_vector_print(34,dataBuf);
}

// Función para leer los datos de medición del SPS30
void SPS30_ReadData(void) {
    // Comando para leer los datos de medición (Reemplaza con el comando real)
    uint8_t readCmd[] = {0x7E, 0x00, 0x03, 0x00, 0xFC, 0x7E};
    uint8_t dataBuf[64]; // Asegúrate de que este buffer sea adecuado para los datos
    memset(dataBuf, 0, sizeof(dataBuf));
    uint8_t longdataBuf = sizeof(dataBuf);
    uart_print("\n Solicitar:");
    uart_vector_print(6,readCmd);
    HAL_UART_Transmit(&huart5, readCmd, sizeof(readCmd), 100);
    HAL_UART_Receive(&huart5, dataBuf, 64,100); // Ajusta el timeout según sea necesario
    uart_print("\n Respuesta:");
    uart_vector_print(longdataBuf,dataBuf);
}

/**
 * Intenta comunicarse con el sensor SPS30 para verificar su presencia y funcionamiento.
 *
 * Esta función primero intenta despertar al sensor SPS30, en caso de que esté en modo de bajo consumo,
 * pero ignora cualquier fallo en este paso ya que el sensor podría no estar en modo de sueño.
 * Luego, intenta obtener el número de serie del sensor como una forma de prueba de comunicación.
 *
 * @return int16_t: Retorna 0 si la comunicación fue exitosa (el número de serie se obtuvo correctamente).
 *                  En caso de error en la comunicación, retorna un código de error negativo.
 */
int16_t sps30_probe(void) {
    char serial[SPS30_MAX_SERIAL_LEN];
    // Try to wake up, but ignore failure if it is not in sleep mode
    (void)sps30_wake_up();
    int16_t ret = sps30_get_serial(serial);

    return ret;
}




/**
 * Obtiene el número de serie del sensor SPS30.
 *
 * Esta función envía un comando al sensor SPS30 solicitando su número de serie.
 * La comunicación se realiza a través de SHDLC, un protocolo de comunicación definido por Sensirion.
 *
 * @param serial: Puntero a un buffer donde se almacenará el número de serie obtenido.
 *                El buffer debe ser capaz de almacenar al menos SPS30_MAX_SERIAL_LEN bytes.
 *
 * @return int16_t: Retorna 0 si el número de serie se obtuvo correctamente.
 *                  En caso de error en la comunicación o en el estado del sensor, retorna un código de error negativo.
 */
int16_t sps30_get_serial(char* serial) {
    struct sensirion_shdlc_rx_header header; // Estructura para almacenar la cabecera de la respuesta.
    uint8_t param_buf[] = {SPS30_CMD_DEV_INFO_SUBCMD_GET_SERIAL}; // Buffer con el subcomando para obtener el número de serie.
    int16_t ret;

    // Realiza el intercambio de datos con el sensor utilizando el protocolo SHDLC.
    ret = sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_DEV_INFO, sizeof(param_buf),
                              param_buf, SPS30_MAX_SERIAL_LEN, &header,
                              (uint8_t*)serial);
    // Verifica si hubo un error en la comunicación.
    if (ret < 0){
        return ret; // Retorna el código de error de la comunicación.
        uart_print("ERROR sps30_get_serial");
    }

    // Verifica si el sensor reportó un estado de error.
    if (header.state)
        return SPS30_ERR_STATE(header.state); // Retorna el código de error basado en el estado reportado por el sensor.

    return 0; // Comunicación exitosa y el número de serie se obtuvo correctamente.
}

int16_t sps30_start_measurement(void) {
    struct sensirion_shdlc_rx_header header;
    uint8_t param_buf[] = SPS30_SUBCMD_MEASUREMENT_START;

    return sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_START_MEASUREMENT,
                               sizeof(param_buf), param_buf, 0, &header,
                               (uint8_t*)NULL);
}

int16_t sps30_stop_measurement(void) {
    struct sensirion_shdlc_rx_header header;

    return sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_STOP_MEASUREMENT, 0,
                               (uint8_t*)NULL, 0, &header, (uint8_t*)NULL);
}

int16_t sps30_read_measurement(struct sps30_measurement* measurement) {
    struct sensirion_shdlc_rx_header header;
    int16_t error;
    uint8_t data[10][4];

    error = sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_READ_MEASUREMENT, 0,
                                (uint8_t*)NULL, sizeof(data), &header,
                                (uint8_t*)data);
    if (error) {
        return error;
    }

    if (header.data_len != sizeof(data)) {
        return SPS30_ERR_NOT_ENOUGH_DATA;
    }

    measurement->mc_1p0 = sensirion_common_bytes_to_float(data[0]);
    measurement->mc_2p5 = sensirion_common_bytes_to_float(data[1]);
    measurement->mc_4p0 = sensirion_common_bytes_to_float(data[2]);
    measurement->mc_10p0 = sensirion_common_bytes_to_float(data[3]);
    measurement->nc_0p5 = sensirion_common_bytes_to_float(data[4]);
    measurement->nc_1p0 = sensirion_common_bytes_to_float(data[5]);
    measurement->nc_2p5 = sensirion_common_bytes_to_float(data[6]);
    measurement->nc_4p0 = sensirion_common_bytes_to_float(data[7]);
    measurement->nc_10p0 = sensirion_common_bytes_to_float(data[8]);
    measurement->typical_particle_size = sensirion_common_bytes_to_float(data[9]);

    if (header.state) {
        return SPS30_ERR_STATE(header.state);
    }

    return 0;
}

int16_t sps30_sleep(void) {
    struct sensirion_shdlc_rx_header header;

    return sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_SLEEP, 0, (uint8_t*)NULL,
                               0, &header, (uint8_t*)NULL);
}

int16_t sps30_wake_up(void) {
    struct sensirion_shdlc_rx_header header;
    int16_t ret;
    const uint8_t data = 0xFF;

    ret = sensirion_uart_tx(1, &data);
    if (ret < 0) {
        return ret;
    }
    return sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_WAKE_UP, 0, (uint8_t*)NULL,
                               0, &header, (uint8_t*)NULL);
}

int16_t sps30_get_fan_auto_cleaning_interval(uint32_t* interval_seconds) {
    struct sensirion_shdlc_rx_header header;
    uint8_t tx_data[] = {SPS30_SUBCMD_READ_FAN_CLEAN_INTV};
    int16_t ret;
    uint8_t data[4];

    ret = sensirion_shdlc_xcv(
        SPS30_ADDR, SPS30_CMD_FAN_CLEAN_INTV, sizeof(tx_data), tx_data,
        sizeof(*interval_seconds), &header, (uint8_t*)data);
    if (ret < 0)
        return ret;

    *interval_seconds = sensirion_bytes_to_uint32_t(data);

    if (header.state)
        return SPS30_ERR_STATE(header.state);

    return 0;
}

void sensirion_uint32_t_to_bytes(uint32_t value, uint8_t* bytes_array) {
    bytes_array[0] = (uint8_t)(value >> 24) & 0xFF; // Extrae el byte más significativo
    bytes_array[1] = (uint8_t)(value >> 16) & 0xFF; // Extrae el segundo byte
    bytes_array[2] = (uint8_t)(value >> 8) & 0xFF;  // Extrae el tercer byte
    bytes_array[3] = (uint8_t)(value) & 0xFF;       // Extrae el byte menos significativo
}


int16_t sps30_set_fan_auto_cleaning_interval(uint32_t interval_seconds) {
    struct sensirion_shdlc_rx_header header;
    uint8_t cleaning_command[SPS30_CMD_FAN_CLEAN_INTV_LEN];

    cleaning_command[0] = SPS30_SUBCMD_READ_FAN_CLEAN_INTV;
    sensirion_uint32_t_to_bytes(interval_seconds, &cleaning_command[1]);

    return sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_FAN_CLEAN_INTV,
                               sizeof(cleaning_command), cleaning_command, 0,
                               &header, (uint8_t*)NULL);
}

int16_t sps30_get_fan_auto_cleaning_interval_days(uint8_t* interval_days) {
    int16_t ret;
    uint32_t interval_seconds;

    ret = sps30_get_fan_auto_cleaning_interval(&interval_seconds);
    if (ret < 0)
        return ret;

    *interval_days = interval_seconds / (24 * 60 * 60);
    return ret;
}

int16_t sps30_set_fan_auto_cleaning_interval_days(uint8_t interval_days) {
    return sps30_set_fan_auto_cleaning_interval((uint32_t)interval_days * 24 *
                                                60 * 60);
}

int16_t sps30_start_manual_fan_cleaning(void) {
    struct sensirion_shdlc_rx_header header;

    return sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_START_FAN_CLEANING, 0,
                               (uint8_t*)NULL, 0, &header, (uint8_t*)NULL);
}

int16_t
sps30_read_version(struct sps30_version_information* version_information) {
    struct sensirion_shdlc_rx_header header;
    int16_t error;
    uint8_t data[7];

    error = sensirion_shdlc_xcv(SPS30_ADDR, SPS30_CMD_READ_VERSION, 0,
                                (uint8_t*)NULL, sizeof(data), &header, data);
    if (error) {
        return error;
    }

    if (header.data_len != sizeof(data)) {
        return SPS30_ERR_NOT_ENOUGH_DATA;
    }

    if (header.state) {
        return SPS30_ERR_STATE(header.state);
    }

    version_information->firmware_major = data[0];
    version_information->firmware_minor = data[1];
    version_information->hardware_revision = data[3];
    version_information->shdlc_major = data[5];
    version_information->shdlc_minor = data[6];

    return error;
}

int16_t sps30_reset(void) {
    return sensirion_shdlc_tx(SPS30_ADDR, SPS30_CMD_RESET, 0, (uint8_t*)NULL);
}
