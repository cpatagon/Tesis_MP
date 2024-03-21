################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../APIs/Src/sensirion_common.c \
../APIs/Src/sensirion_shdlc.c \
../APIs/Src/sensirion_uart.c \
../APIs/Src/sensirion_uart_hal.c \
../APIs/Src/sps30.c \
../APIs/Src/sps_git_version.c \
../APIs/Src/uart.c

OBJS += \
./APIs/Src/sensirion_common.o \
./APIs/Src/sensirion_shdlc.o \
./APIs/Src/sensirion_uart.o \
./APIs/Src/sensirion_uart_hal.o \
./APIs/Src/sps30.o \
./APIs/Src/sps_git_version.o \
./APIs/Src/uart.o

C_DEPS += \
./APIs/Src/sensirion_common.d \
./APIs/Src/sensirion_shdlc.d \
./APIs/Src/sensirion_uart.d \
./APIs/Src/sensirion_uart_hal.d \
./APIs/Src/sps30.d \
./APIs/Src/sps_git_version.d \
./APIs/Src/uart.d


# Each subdirectory must supply rules for building sources it contributes
APIs/Src/%.o APIs/Src/%.su APIs/Src/%.cyclo: ../APIs/Src/%.c APIs/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I/home/lgomez/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc -I/home/lgomez/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/home/lgomez/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/home/lgomez/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include -I"/home/lgomez/Documentos/MAGISTER_UBA/TESIS/STM32/Tesis_DHT22/APIs" -I"/home/lgomez/Documentos/MAGISTER_UBA/TESIS/STM32/Tesis_DHT22/APIs/Inc" -I"/home/lgomez/Documentos/MAGISTER_UBA/TESIS/STM32/Tesis_DHT22/APIs/Src" -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -u _scanf_float -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-APIs-2f-Src

clean-APIs-2f-Src:
	-$(RM) ./APIs/Src/sensirion_common.cyclo ./APIs/Src/sensirion_common.d ./APIs/Src/sensirion_common.o ./APIs/Src/sensirion_common.su ./APIs/Src/sensirion_shdlc.cyclo ./APIs/Src/sensirion_shdlc.d ./APIs/Src/sensirion_shdlc.o ./APIs/Src/sensirion_shdlc.su ./APIs/Src/sensirion_uart.cyclo ./APIs/Src/sensirion_uart.d ./APIs/Src/sensirion_uart.o ./APIs/Src/sensirion_uart.su ./APIs/Src/sensirion_uart_hal.cyclo ./APIs/Src/sensirion_uart_hal.d ./APIs/Src/sensirion_uart_hal.o ./APIs/Src/sensirion_uart_hal.su ./APIs/Src/sps30.cyclo ./APIs/Src/sps30.d ./APIs/Src/sps30.o ./APIs/Src/sps30.su ./APIs/Src/sps_git_version.cyclo ./APIs/Src/sps_git_version.d ./APIs/Src/sps_git_version.o ./APIs/Src/sps_git_version.su ./APIs/Src/uart.cyclo ./APIs/Src/uart.d ./APIs/Src/uart.o ./APIs/Src/uart.su

.PHONY: clean-APIs-2f-Src
