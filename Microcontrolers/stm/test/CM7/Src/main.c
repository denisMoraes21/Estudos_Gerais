/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "lwip.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "task.h"
#include "lan8742.h"
/* USER CODE END Includes */

#include "logger.h"
#include "ring_buffer.h"
#include "definitions.h"
#include "stm32h7xx_hal.h"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* DUAL_CORE_BOOT_SYNC_SEQUENCE: Define for dual core boot synchronization    */
/*                             demonstration code based on hardware semaphore */
/* This define is present in both CM7/CM4 projects                            */
/* To comment when developping/debugging on a single core                     */
#define DUAL_CORE_BOOT_SYNC_SEQUENCE

#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c4;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
extern struct netif gnetif;

#define SERVER_IP       "192.168.1.10"
#define SERVER_PORT     5000
#define RX_BUFFER_SIZE  128

static void tcp_client_task(void *argument)
{
    int socket_fd;
    int result;
    int received;

    struct sockaddr_in server_address;

    char tx_buffer[128];
    char rx_buffer[RX_BUFFER_SIZE];

    (void)argument;

    /*
     * Aguarda a interface Ethernet e o link físico ficarem ativos.
     */
    while (!netif_is_up(&gnetif) ||
           !netif_is_link_up(&gnetif))
    {
        LOG_INFO("Aguardando interface Ethernet...");
        osDelay(1000);
    }

    LOG_INFO("Ethernet pronta");

    for (;;)
    {
        socket_fd = lwip_socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );

        if (socket_fd < 0)
        {
            LOG_ERROR("Erro ao criar socket");
            osDelay(2000);
            continue;
        }

        memset(&server_address, 0, sizeof(server_address));

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(SERVER_PORT);

        result = inet_aton(
            SERVER_IP,
            &server_address.sin_addr
        );

        if (result == 0)
        {
            LOG_ERROR("IP do servidor invalido");
            lwip_close(socket_fd);
            osDelay(2000);
            continue;
        }

        LOG_INFO(
            "Conectando em %s:%d...",
            SERVER_IP,
            SERVER_PORT
        );

        result = lwip_connect(
            socket_fd,
            (struct sockaddr *)&server_address,
            sizeof(server_address)
        );

        if (result < 0)
        {
            LOG_ERROR(
                "Falha ao conectar. errno = %d",
                errno
            );

            lwip_close(socket_fd);
            osDelay(2000);
            continue;
        }

        LOG_INFO("Conectado ao servidor TCP");

        uint32_t counter = 0;

        while (1)
        {
            int message_length = snprintf(
                tx_buffer,
                sizeof(tx_buffer),
                "Mensagem STM32: %lu\n",
                (unsigned long)counter++
            );

            result = lwip_send(
                socket_fd,
                tx_buffer,
                message_length,
                0
            );

            if (result < 0)
            {
                LOG_ERROR(
                    "Erro no envio. errno = %d",
                    errno
                );
                break;
            }

            LOG_INFO("Enviados %d bytes", result);

            received = lwip_recv(
                socket_fd,
                rx_buffer,
                sizeof(rx_buffer) - 1,
                0
            );

            if (received == 0)
            {
                LOG_WARN("Servidor encerrou a conexão");
                break;
            }

            if (received < 0)
            {
                LOG_ERROR(
                    "Erro na recepcao. errno = %d",
                    errno
                );
                break;
            }

            rx_buffer[received] = '\0';

            LOG_INFO(
                "Servidor respondeu: %s",
                rx_buffer
            );

            osDelay(1000);
        }

        LOG_INFO("Fechando socket");

        lwip_shutdown(socket_fd, SHUT_RDWR);
        lwip_close(socket_fd);

        osDelay(2000);
    }
}

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
extern lan8742_Object_t LAN8742;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C4_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);

osThreadId_t task1Handle;
osThreadId_t task2Handle;
/* USER CODE BEGIN PFP */

void StartTask1(void *argument)
{
    for (;;)
    {
        LOG_INFO("Task 1");
/* USER CODE END PFP */

        osDelay(500);
    }
}
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void StartTask2(void *argument)
{
    for (;;)
    {
        LOG_INFO("Task 2");

        osDelay(1000);
    }
}

typedef struct __attribute__((packed))
{
    uint16_t temperatura;
    uint16_t umidade;
    uint32_t contador;

} SensorData;

SensorData sensor;

void ESP32_ReadSensor(void)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Receive(
        &hi2c4,
        0x42 << 1,              // endereço I2C + bit R/W
        (uint8_t *)&sensor,
        sizeof(sensor),
        HAL_MAX_DELAY
    );
    (void)status;


    // if(status == HAL_OK)
    // {
    //     LOG_ERROR("Temperatura: %.1f C\n",
    //            sensor.temperatura / 10.0);


    //     LOG_ERROR("Umidade: %.1f %%\n",
    //            sensor.umidade / 10.0);

    //     LOG_ERROR("Contador: %lu\n",
    //            sensor.contador);

    //     LOG_INFO("Temperatura raw: %u", sensor.temperatura);
    //     LOG_INFO("Umidade raw: %u", sensor.umidade);
    //     LOG_INFO("Contador: %lu", sensor.contador);
    // }
    // else
    // {
    //     LOG_ERROR("Erro I2C: %d\n", status);
    // }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
  int32_t timeout;
#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */
/* USER CODE END Boot_Mode_Sequence_0 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
#if defined(DUAL_CORE_BOOT_SYNC_SEQUENCE)
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
#endif /* DUAL_CORE_BOOT_SYNC_SEQUENCE */
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

    
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART3_UART_Init();
    MX_USART1_UART_Init();
    MX_SPI2_Init();
    MX_I2C4_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */

      logger_init();
    /* USER CODE END 2 */

    // const osThreadAttr_t task1_attributes = {
    //     .name = "Task1",
    //     .stack_size = 2048,
    //     .priority = osPriorityNormal,
    // };

    // const osThreadAttr_t task2_attributes = {
    //     .name = "Task2",
    //     .stack_size = 2048,
    //     .priority = osPriorityNormal,
    // };

    // task1Handle = osThreadNew(
    //         StartTask1,
    //         NULL,
    //         &task1_attributes
    // );

    // if(task1Handle == NULL)
    // {
    //     Error_Handler();
    // }

    // task2Handle = osThreadNew(
    //         StartTask2,
    //         NULL,
    //         &task2_attributes
    // );

    // if(task2Handle == NULL)
    // {
    //     Error_Handler();
    // }

    // osKernelStart();
    MX_LWIP_Init();

    osKernelInitialize();

    defaultTaskHandle = osThreadNew(
        tcp_client_task,
        NULL,
        &defaultTask_attributes
    );

    if (defaultTaskHandle == NULL)
    {
        LOG_ERROR("Falha ao criar defaultTask");
        Error_Handler();
    }

    LOG_INFO("Iniciando kernel FreeRTOS");
    osKernelStart();

    while (1)
    {
        LOG_ERROR("Olá");
        // ESP32_ReadSensor();
        // uint8_t txData[] = {0xAA, 0x55, 0x12, 0x34};
        // uint8_t rx[4] = {0};
        // LOG_INFO("PT-BR -> Cliente: BYD, Projeto: VOLTA, Descrição: BMS");
        // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

        // HAL_Delay(100);

        // HAL_StatusTypeDef ret = HAL_SPI_TransmitReceive(
        // &hspi2,
        // txData,
        // rx,
        // sizeof(txData),
        // 1000
        // );

        // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

        // HAL_Delay(100);
      
        // LOG_INFO("HAL status = %d", ret);
        // LOG_INFO("SPI2->SR   = 0x%08lX", SPI2->SR);
        // LOG_INFO("SPI2->CR1  = 0x%08lX", SPI2->CR1);
        // LOG_INFO("SPI2->CFG1 = 0x%08lX", SPI2->CFG1);
        // LOG_INFO("SPI2->CFG2 = 0x%08lX", SPI2->CFG2);

        // if (ret == HAL_OK)
        // {
        //     LOG_INFO("Recebidos %d bytes", 4);

        //     LOG_INFO("RX: %02X %02X %02X %02X",
        //      rx[0], rx[1], rx[2], rx[3]);
        // }
        // else
        // {
        //     LOG_ERROR("SPI erro %d", ret);
        // }
        HAL_Delay(1000);   // <-- 1 segundo
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 23;
  RCC_OscInitStruct.PLL.PLLN = 177;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 4;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_0;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x10707DBC;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart2, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  // MX_LWIP_Init();
  // tcp_client_task();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  // LOG_INFO("===== LWIP INICIALIZADO =====");

  // if (socket_init() != osOK)
  // {
  //     LOG_ERROR("Nao foi possivel inicializar o cliente TCP");
  // }

  // // printf("Teste printf\r\n");

  //   for (;;)
  //   {
  //       // LOG_INFO("--------------------------------");

  //       // LOG_INFO("Interface: %c%c%d",
  //       //          gnetif.name[0],
  //       //          gnetif.name[1],
  //       //          gnetif.num);

  //       LOG_INFO("Link: %s",
  //                netif_is_link_up(&gnetif) ? "UP" : "DOWN");

  //       LOG_INFO("Interface: %s",
  //                netif_is_up(&gnetif) ? "UP" : "DOWN");

  //       LOG_INFO("IP      : %s",
  //                ip4addr_ntoa(netif_ip4_addr(&gnetif)));

  //       LOG_INFO("Mascara : %s",
  //                ip4addr_ntoa(netif_ip4_netmask(&gnetif)));

  //       LOG_INFO("Gateway : %s",
  //                ip4addr_ntoa(netif_ip4_gw(&gnetif)));

  //       LOG_INFO("MAC: %02X:%02X:%02X:%02X:%02X:%02X",
  //                gnetif.hwaddr[0],
  //                gnetif.hwaddr[1],
  //                gnetif.hwaddr[2],
  //                gnetif.hwaddr[3],
  //                gnetif.hwaddr[4],
  //                gnetif.hwaddr[5]);

  //       // LOG_INFO("MTU: %u", gnetif.mtu);

  //       // LOG_INFO("Flags = 0x%04X", gnetif.flags);

  //       if (gnetif.flags & NETIF_FLAG_ETHARP)
  //           LOG_INFO("ARP habilitado");

  //       if (gnetif.flags & NETIF_FLAG_BROADCAST)
  //           LOG_INFO("Broadcast habilitado");

  //       if (gnetif.flags & NETIF_FLAG_ETHERNET)
  //           LOG_INFO("Ethernet habilitada");

  //           LOG_INFO("RX packets : %u", lwip_stats.link.recv);
  //       LOG_INFO("TX packets : %u", lwip_stats.link.xmit);
  //       LOG_INFO("RX errors  : %u", lwip_stats.link.drop);
  //       LOG_INFO("RX mem err : %u", lwip_stats.link.memerr);

  //       LOG_INFO("ARP recv   : %u", lwip_stats.etharp.recv);
  //       LOG_INFO("ARP xmit   : %u", lwip_stats.etharp.xmit);

  //       LOG_INFO("IP recv    : %u", lwip_stats.ip.recv);
  //       LOG_INFO("IP sent    : %u", lwip_stats.ip.xmit);

        
  //       LOG_INFO("ICMP recv  : %u", lwip_stats.icmp.recv);
  //       LOG_INFO("ICMP sent  : %u", lwip_stats.icmp.xmit);
  //       int32_t state = LAN8742_GetLinkState(&LAN8742);

  //       LOG_INFO("PHY State = %ld", state);

  //       // LOG_INFO("Kernel tick = %lu", osKernelGetTickCount());

  //       osDelay(2000);

  //       // LOG_INFO("Kernel tick = %lu", osKernelGetTickCount());
  //       LOG_INFO("RX packets : %u", lwip_stats.link.recv);
  //       LOG_INFO("TX packets : %u", lwip_stats.link.xmit);
  //       LOG_INFO("ETH IRQ    : %lu", eth_irq_count);
  //       LOG_INFO("ETH RX cb  : %lu", eth_rx_complete_count);
  //       LOG_INFO("ETH TX cb  : %lu", eth_tx_complete_count);
  //       UBaseType_t watermark = uxTaskGetStackHighWaterMark(NULL);

  //       LOG_INFO("Stack livre = %lu words (%lu bytes)",
  //               watermark,
  //               watermark * sizeof(StackType_t));

  // }

  /* USER CODE END 5 */
}

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  printf("Error_Handler\r\n");

  HAL_Delay(1000);
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
