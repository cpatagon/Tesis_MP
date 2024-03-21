/*
 * Nombre del archivo: sps30.h
 * Descripción: Definiciones y declaraciones para el control del sensor SPS30 mediante UART.
 * Autor: Luis Gómez P.
 * Derechos de Autor: (C) 2023 Luis Gómez P. EIRL
 * Licencia: GNU General Public License v3.0
 *
 * Este programa es software libre: puedes redistribuirlo y/o modificarlo
 * bajo los términos de la Licencia Pública General GNU publicada por
 * la Free Software Foundation, ya sea la versión 3 de la Licencia, o
 * (a tu elección) cualquier versión posterior.
 *
 * Este programa se distribuye con la esperanza de que sea útil,
 * pero SIN NINGUNA GARANTÍA; sin siquiera la garantía implícita
 * de COMERCIABILIDAD o APTITUD PARA UN PROPÓSITO PARTICULAR. Ver la
 * Licencia Pública General GNU para más detalles.
 *
 * Deberías haber recibido una copia de la Licencia Pública General GNU
 * junto con este programa. Si no es así, visita <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 *
 */

#ifndef SPS30_H
#define SPS30_H

/** @file
 ** @brief Definiciones y declaraciones para la interacción con el sensor SPS30 a través de UART.
 **/

/* === Inclusión de archivos de cabecera ======================================================== */

#include <stdint.h>

/* === Definiciones de macros públicas ========================================================== */

/** @defgroup SPS30_COMMANDS Comandos del sensor SPS30 */
/** @{ */
#define SPS30_CMD_START_MEASUREMENT 0x00 /**< Comando para iniciar la medición. */
#define SPS30_CMD_STOP_MEASUREMENT  0x01 /**< Comando para detener la medición. */
#define SPS30_CMD_READ_MEASUREMENT  0x03 /**< Comando para leer los datos de medición. */
#define SPS30_CMD_SLEEP             0x10 /**< Comando para poner el sensor en modo de sueño. */
#define SPS30_CMD_WAKE_UP           0x11 /**< Comando para despertar el sensor del modo de sueño. */
#define SPS30_CMD_FAN_CLEAN_INTV                                                                   \
    0x80 /**< Comando para establecer el intervalo de limpieza del ventilador. */
#define SPS30_CMD_START_FAN_CLEANING 0x56 /**< Comando para iniciar la limpieza del ventilador. */
#define SPS30_CMD_DEV_INFO           0xd0 /**< Comando para solicitar información del dispositivo. */
#define SPS30_CMD_READ_VERSION       0xd1 /**< Comando para leer la versión de firmware. */
#define SPS30_CMD_RESET              0xd3 /**< Comando para reiniciar el sensor. */
/** @} */

/**
 * @brief Macro para definir un código de error basado en el estado proporcionado.
 * @param state Estado específico para combinar con la máscara de error.
 */
#define SPS30_ERR_STATE(state) (SPS30_ERR_STATE_MASK | (state))

/** @brief Frame de inicio de medición para comunicación SHDLC. */
#define SPS30_FRAME_START_MEASUREMENT                                                              \
    { 0x7E, 0x00, 0x00, 0x02, 0x01, 0x03, 0xF9, 0x7E }

/** @brief Frame para solicitar la lectura de medición al SPS30. */
#define SPS30_FRAME_READ_MEASUREMENT                                                               \
    { 0x7E, 0x00, 0x03, 0x00, 0xFC, 0x7E }

/** @brief Frame para solicitar el número de serie del SPS30. */
#define SPS30_FRAME_SERIAL_NUMBER                                                                  \
    { 0x7E, 0x00, 0xD0, 0x01, 0x03, 0x2B, 0x7E }

/** @brief Frame para poner el SPS30 en modo de sueño. */
#define SPS30_FRAME_SLEEP                                                                          \
    { 0x7E, 0x00, 0x10, 0x00, 0xEF, 0x7E }

/** @brief Frame para detener la medición en el SPS30. */
#define SPS30_FRAME_STOP_MEASUREMENT                                                               \
    { 0x7E, 0x00, 0x01, 0x00, 0xFE, 0x7E }

/** @brief Frame para despertar el SPS30 del modo de sueño. */
#define SPS30_FRAME_WAKE_UP                                                                        \
    { 0x7E, 0x00, 0x7D, 0x31, 0x00, 0xEE, 0x7E }

/** @brief Pulso para despertar el SPS30 del modo de sueño. */
#define SPS30_PULSE_WAKE_UP 0xFF

/* === Declaraciones de tipos de datos públicos ================================================= */

/* === Declaraciones de variables públicas ====================================================== */

/* === Declaraciones de funciones públicas ====================================================== */

/**
 * @brief Inicia la medición en el sensor SPS30.
 */
void SPS30_StartMeasurement(void);

/**
 * @brief Detiene la medición en el sensor SPS30.
 */
void SPS30_StopMeasurement(void);

/**
 * @brief Lee los datos de medición del sensor SPS30.
 */
void SPS30_ReadData(void);

/**
 * @brief Obtiene el número de serie del sensor SPS30.
 */
void SPS30_SerialNumber(void);

/**
 * @brief Pone el sensor SPS30 en modo de sueño (Sleep mode).
 */
void SPS30_Sleep(void);

/**
 * @brief Despierta el sensor SPS30 del modo de sueño.
 */
void SPS30_WakeUP(void);

#endif /* SPS30_H */
