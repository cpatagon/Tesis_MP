/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h> // Añade esto para sprintf
#include <string.h> // Añade esto para strlen
#include "sps30.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Función para iniciar la medición en el SPS30
//void SPS30_StartMeasurement(void) {
//    // Comando para iniciar la medición en modo UART (Reemplaza con el comando real)
//    uint8_t startCmd[] = {0x7E, 0x00, 0x00, 0x02, 0x01, 0x03, 0xF9, 0x7E};
//    HAL_UART_Transmit(&huart5, startCmd, sizeof(startCmd), 100);
//    HAL_Delay(1000); // Espera para que el sensor procese el comando
//}

// Función para leer los datos de medición del SPS30
//void SPS30_ReadData(void) {
//    // Comando para leer los datos de medición (Reemplaza con el comando real)
//    uint8_t readCmd[] = {0x7E, 0x00, 0x03, 0x00, 0xFC, 0x7E};
//    uint8_t dataBuf[60]; // Asegúrate de que este buffer sea adecuado para los datos
//    memset(dataBuf, 0, sizeof(dataBuf));
//
//    HAL_UART_Transmit(&huart5, readCmd, sizeof(readCmd), 100);
//    HAL_UART_Receive(&huart5, dataBuf, sizeof(dataBuf), 5000); // Ajusta el timeout según sea necesario
//
//    // Supongamos que la concentración de PM2.5 está en los bytes 5 y 6 del dataBuf, esto es solo un ejemplo
//    // Necesitarás ajustar esto según el formato de datos real del SPS30
//    float pm2_5_value = (float)((dataBuf[5] << 8) | dataBuf[6]) / 10.0; // Conversión de ejemplo
//
//    // Buffer para almacenar la cadena de caracteres ASCII
//    char buffer[64];
//    sprintf(buffer, "Valor PM2.5: %.2f ug/m3\r\n", pm2_5_value);
//
//    // Envía la cadena ASCII convertida a través de huart3 para visualización en el PC
//    HAL_UART_Transmit(&huart3, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */




  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

 // SPS30_Init(&huart5);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

	uint32_t counter = 0; // Variable del contador
	char buffer[50]; // Buffer para almacenar el mensaje a enviar
	/* mensaje con los parametros de configuracion de la UART */
	static uint8_t *message = (uint8_t*) "\n\n"
			"-----------------------------------------------------------\n"
			"*** UART port initialization successful !!! ***\n"
			"-----------------------------------------------------------\n";

	HAL_UART_Transmit(&huart3, (uint8_t*) message, strlen((char*) message),
	HAL_MAX_DELAY);

	struct sps30_measurement m;
	    char serial[SPS30_MAX_SERIAL_LEN];
	    const uint8_t AUTO_CLEAN_DAYS = 4;
	    int16_t ret;

	    while (sensirion_uart_open() != 0) {
	           uart_print("UART init failed\n");
	           sensirion_sleep_usec(1000); /* sleep for 1s */
	       }

	       /* Busy loop for initialization, because the main loop does not work without
	        * a sensor.
	        */
	       while (sps30_probe() == 0) {
	    	   uart_print("\nSPS30 probando falla\n");
	           sensirion_sleep_usec(1000000); /* sleep for 1s */
	       }
	    uart_print("SPS30 sensor probing successful\n");

	       struct sps30_version_information version_information;
	       ret = sps30_read_version(&version_information);
	       if (ret) {
	           sprintf(buffer, "error %d reading version information\n", ret);
	           uart_print(buffer);
	       } else {
	           sprintf(buffer,"FW: %u.%u HW: %u, SHDLC: %u.%u\n",
	                  version_information.firmware_major,
	                  version_information.firmware_minor,
	                  version_information.hardware_revision,
	                  version_information.shdlc_major,
	                  version_information.shdlc_minor);
	       uart_print(buffer);
	       }

	       ret = sps30_get_serial(serial);
	       if (ret){
	    	   sprintf(buffer,"error %d reading serial\n", ret);
	           uart_print(buffer);}
	       else{
	    	   sprintf(buffer,"SPS30 Serial: %s\n", serial);
	           uart_print(buffer);
	       }

	       ret = sps30_set_fan_auto_cleaning_interval_days(AUTO_CLEAN_DAYS);
	       if (ret){
	    	   sprintf(buffer,"error %d setting the auto-clean interval\n", ret);
	           uart_print(buffer);
	       }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        ret = sps30_start_measurement();
        ret = sps30_read_measurement(&m);
        if (ret < 0) {
            uart_print("error starting measurement\n");
        }


		// Formatear el mensaje a enviar

		sprintf(buffer, "Counter value: %lu\r\n", counter++);

		// Enviar mensaje a través de UART
         uart_print(buffer);


 //       SPS30_StartMeasurement();
        //SPS30_ReadData();
        HAL_Delay(4000); // Espera 10 segundos antes de la próxima lectura

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
