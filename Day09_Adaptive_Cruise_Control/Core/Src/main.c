/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Day9 Adaptive Cruise Control
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

float front_dist;

char msg[200];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */

float read_distance(GPIO_TypeDef *trigPort,
                    uint16_t trigPin,
                    GPIO_TypeDef *echoPort,
                    uint16_t echoPin);

void delay_us(uint16_t us);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    while(__HAL_TIM_GET_COUNTER(&htim2) < us);
}

float read_distance(GPIO_TypeDef *trigPort,
                    uint16_t trigPin,
                    GPIO_TypeDef *echoPort,
                    uint16_t echoPin)
{
    uint32_t time = 0;

    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_RESET);
    delay_us(2);

    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_SET);
    delay_us(10);

    HAL_GPIO_WritePin(trigPort, trigPin, GPIO_PIN_RESET);

    while(!(HAL_GPIO_ReadPin(echoPort, echoPin)));

    __HAL_TIM_SET_COUNTER(&htim2, 0);

    while(HAL_GPIO_ReadPin(echoPort, echoPin));

    time = __HAL_TIM_GET_COUNTER(&htim2);

    return time * 0.034 / 2;
}

/* USER CODE END 0 */

/**
  * @brief  Main program
  * @retval int
  */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
      /* RESET LEDs + BUZZER */

      HAL_GPIO_WritePin(GPIOA,
                        GPIO_PIN_0 |
                        GPIO_PIN_1 |
                        GPIO_PIN_2 |
                        GPIO_PIN_3 |
                        GPIO_PIN_4,
                        GPIO_PIN_RESET);

      /* READ FRONT SENSOR */

      front_dist = read_distance(GPIOB,
                                 GPIO_PIN_0,
                                 GPIOB,
                                 GPIO_PIN_1);

      /* NORMAL SPEED */

      if(front_dist > 70)
      {
          HAL_GPIO_WritePin(GPIOA,
                            GPIO_PIN_0,
                            GPIO_PIN_SET);

          sprintf(msg,
          "DISTANCE:%d cm STATUS:NORMAL SPEED\r\n",
          (int)front_dist);
      }

      /* SLOW DOWN */

      else if(front_dist > 40)
      {
          HAL_GPIO_WritePin(GPIOA,
                            GPIO_PIN_1,
                            GPIO_PIN_SET);

          sprintf(msg,
          "DISTANCE:%d cm STATUS:SLOW DOWN\r\n",
          (int)front_dist);
      }

      /* APPLY BRAKE */

      else if(front_dist > 20)
      {
          HAL_GPIO_WritePin(GPIOA,
                            GPIO_PIN_2,
                            GPIO_PIN_SET);

          HAL_GPIO_WritePin(GPIOA,
                            GPIO_PIN_4,
                            GPIO_PIN_SET);

          sprintf(msg,
          "DISTANCE:%d cm STATUS:APPLY BRAKE\r\n",
          (int)front_dist);
      }

      /* EMERGENCY STOP */

      else
      {
          HAL_GPIO_WritePin(GPIOA,
                            GPIO_PIN_3,
                            GPIO_PIN_SET);

          HAL_GPIO_WritePin(GPIOA,
                            GPIO_PIN_4,
                            GPIO_PIN_SET);

          sprintf(msg,
          "DISTANCE:%d cm STATUS:EMERGENCY STOP\r\n",
          (int)front_dist);
      }

      /* UART TRANSMIT */

      HAL_UART_Transmit(&huart1,
                        (uint8_t*)msg,
                        strlen(msg),
                        HAL_MAX_DELAY);

      HAL_Delay(300);
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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                          FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;

  htim2.Init.Prescaler = 71;

  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;

  htim2.Init.Period = 65535;

  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  htim2.Init.AutoReloadPreload =
  TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource =
  TIM_CLOCKSOURCE_INTERNAL;

  if (HAL_TIM_ConfigClockSource(&htim2,
                                &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;

  sMasterConfig.MasterSlaveMode =
  TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim2,
                                            &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;

  huart1.Init.BaudRate = 115200;

  huart1.Init.WordLength = UART_WORDLENGTH_8B;

  huart1.Init.StopBits = UART_STOPBITS_1;

  huart1.Init.Parity = UART_PARITY_NONE;

  huart1.Init.Mode = UART_MODE_TX_RX;

  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* OUTPUT PINS */

  HAL_GPIO_WritePin(GPIOA,
                    GPIO_PIN_0 |
                    GPIO_PIN_1 |
                    GPIO_PIN_2 |
                    GPIO_PIN_3 |
                    GPIO_PIN_4,
                    GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB,
                    GPIO_PIN_0,
                    GPIO_PIN_RESET);

  /* PA0 PA1 PA2 PA3 PA4 */

  GPIO_InitStruct.Pin = GPIO_PIN_0 |
                        GPIO_PIN_1 |
                        GPIO_PIN_2 |
                        GPIO_PIN_3 |
                        GPIO_PIN_4;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PB0 */

  GPIO_InitStruct.Pin = GPIO_PIN_0;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB1 */

  GPIO_InitStruct.Pin = GPIO_PIN_1;

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief Error Handler
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();

  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file,
                   uint32_t line)
{
}

#endif
