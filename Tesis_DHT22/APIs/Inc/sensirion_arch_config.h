/*
 * sensirion_arch_config.h
 *
 *  Created on: Mar 4, 2024
 *      Author: lgomez
 */

#ifndef INC_SENSIRION_ARCH_CONFIG_H_
#define INC_SENSIRION_ARCH_CONFIG_H_


#ifndef SENSIRION_ARCH_CONFIG_H
#define SENSIRION_ARCH_CONFIG_H

/*
 * AVR Arduinos have a 32 byte I2C receive buffer in the Wire library
 * Use alternative I2C implementation to work around that
 *
 * TODO: change this to a more targetted selection, i.e. __AVR_ATmega328P__ etc vs __AVR__
 */
#ifdef __AVR__
  #ifndef ARDUINO_ARCH_MEGAAVR
     #define SPS30_USE_ALT_I2C
  #endif
#endif /* __AVR__ */

/*
 * ESP8266 core < 2.5.0 has a 32 byte I2C receive buffer in the Wire library
 * Limit readout to PM mass concentrations
 */
#ifdef ESP8266
#include <core_version.h>
#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || \
    defined(ARDUINO_ESP8266_RELEASE_2_4_0) || \
    defined(ARDUINO_ESP8266_RELEASE_2_4_1) || \
    defined(ARDUINO_ESP8266_RELEASE_2_4_2)
#define SPS30_LIMITED_I2C_BUFFER_SIZE
#endif
#endif /* ESP8266 */

/**
 * If your platform does not provide the library stdlib.h you have to remove the
 * include and define NULL yourself (see below).
 */
#include <stdlib.h>

/**
 * #ifndef NULL
 * #define NULL ((void *)0)
 * #endif
 */

/**
 * If your platform does not provide the library stdint.h you have to
 * define the integral types yourself (see below).
 */
#include <stdint.h>

/**
 * Typedef section for types commonly defined in <stdint.h>
 * If your system does not provide stdint headers, please define them
 * accordingly. Please make sure to define int64_t and uint64_t.
 */
/* typedef unsigned long long int uint64_t;
 * typedef long long int int64_t;
 * typedef long int32_t;
 * typedef unsigned long uint32_t;
 * typedef short int16_t;
 * typedef unsigned short uint16_t;
 * typedef char int8_t;
 * typedef unsigned char uint8_t; */

#ifndef __cplusplus

/**
 * If your platform doesn't define the bool type we define it as int. Depending
 * on your system update the definition below.
 */
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif /* bool */

#endif /* __STDC_VERSION__ */

#endif /* __cplusplus */

/**
 * The clock period of the i2c bus in microseconds. Increase this, if your GPIO
 * ports cannot support a 200 kHz output rate. (2 * 1 / 10usec == 200Khz)
 *
 * This is only relevant for the sw-i2c HAL (bit-banging on GPIO pins). The
 * pulse length is half the clock period, the number should thus be even.
 */
#define SENSIRION_I2C_CLOCK_PERIOD_USEC 10

#endif /* SENSIRION_ARCH_CONFIG_H */

#endif /* INC_SENSIRION_ARCH_CONFIG_H_ */
