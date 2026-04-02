/* USER CODE BEGIN Header */
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "lwip.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "usbd_cdc_if.h"
#include "25Q64FVSIG.h"
#include "RS485.h"
/**
 * ST32407 test code for ethernet transceiver DP83848
 */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUF_SIZE 16

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[8];
uint32_t TxMailBox;

CAN_RxHeaderTypeDef RxHeader1;
uint8_t RxData1[8];

CAN_RxHeaderTypeDef RxHeader2;
uint8_t RxData2[8];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);
void try_flash_25Q64FVSIG();
void testing_RS485();
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t usb_rx_buffer[64];
extern uint32_t usb_rx_len;
extern uint8_t usb_rx_flag;
volatile uint32_t adc_value = 0;
volatile float adc_voltage_reading = 0;
volatile long last_adc_time = 0;
volatile long last_usb_time = 0;
volatile long last_clock_time = 0;
static uint32_t last_can_time = 0;
static uint32_t last_rs485_time = 0;
volatile uint16_t adc_buffer[ADC_BUF_SIZE];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
   //HAL_ADC_Start_IT(&hadc1);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();


  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_RTC_Init();
  MX_SDIO_SD_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_LWIP_Init();
  MX_USB_DEVICE_Init();
  MX_FATFS_Init();
  MX_CRC_Init();
  MX_USB_HOST_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  //MX_ETH_Init();

  	  /* USER CODE BEGIN SysInit */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, ADC_BUF_SIZE);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_Start(&hcan2);

  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

  	  //configure TX Message
  TxHeader.StdId = 0xAAA;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 8;
  TxHeader.TransmitGlobalTime = DISABLE;

  	  //init data
  TxData[0] = 0xAA;
  TxData[1] = 0xAA;
  TxData[2] = 0xAA;
  TxData[3] = 0xAA;
  TxData[4] = 0x11;
  TxData[5] = 0xAA;
  TxData[6] = 0xAA;
  TxData[7] = 0xAA;

  	  /* USER CODE END SysInit */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();


    /* USER CODE BEGIN 2 */
    if (HAL_GetTick() - last_can_time > 500) // every 500 ms
    {
        last_can_time = HAL_GetTick();
    	//Sending CAN Message
        HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailBox);
    }

               //test the function of LED Here
           	//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
           	//HAL_Delay(1000);
           	//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);
           	//HAL_Delay(1000);
           	//HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
           	//HAL_Delay(1000);

           	//Turn LED on and OFF
           	//this code block print message to Serial port, monitor it from serial
           	//include this  #include "usbd_cdc_if.h"
           	//init inside main
           	//reading ADC A0

           	//if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK){
           		//adc_value = HAL_ADC_GetValue(&hadc1);
           	//}

           		//This code block reads ADC voltage and print it to serial
           	/*
           	HAL_ADC_Start(&hadc1);

           	if (HAL_ADC_PollForConversion(&hadc1, 200) == HAL_OK)
           	{
           		adc_value = HAL_ADC_GetValue(&hadc1);
           	}
           	HAL_ADC_Stop(&hadc1);
           	*/

      		if ((HAL_GetTick() - last_usb_time) > 10000) {
      			int32_t sum_A0 = 0;
				int32_t sum_A4 = 0;
				uint16_t count = ADC_BUF_SIZE / 2;
  				uint16_t sum = 0;
  				last_usb_time = HAL_GetTick();

  				for(int i = 0; i < ADC_BUF_SIZE; i+=2){
  					sum_A0 += adc_buffer[i]; //Sum of A0
  					sum_A4 += adc_buffer[i+1]; //Sum of A4
  				}

  				uint32_t adc_A0 = sum_A0 / count;
  				uint32_t adc_A4 = sum_A4 / count;

  				float voltage_A0 = ((float)adc_A0 / 4095.0) * 3.3f;
  				float voltage_A4 = ((float)adc_A4 / 4095.0) * 3.3f;



  				char adc_reading_m[50];
  				sprintf(adc_reading_m, "ADC A0: %0.2f V\r\n", voltage_A0);
  				CDC_Transmit_FS((uint8_t*)adc_reading_m, strlen(adc_reading_m));
  				sprintf(adc_reading_m, "ADC A4: %0.2f V\r\n", voltage_A4);
  				CDC_Transmit_FS((uint8_t*)adc_reading_m, strlen(adc_reading_m));

      		}
           	//end of ADC reading code
    /* USER CODE END 2 */

    /* USER CODE BEGIN 3 */

   //Display the time
   if ((HAL_GetTick() - last_clock_time) > 5000) {
	   RTC_TimeTypeDef gTime;
	   RTC_DateTypeDef gDate;
	   last_clock_time = HAL_GetTick();

	   HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	   HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

	   char rtc_msg[64];

	   sprintf(rtc_msg,
			   "Time: %02d:%02d:%02d Date %02d-%02d-20%02d\r\n",
			   gTime.Hours,
			   gTime.Minutes,
			   gTime.Seconds,
			   gDate.Date,
			   gDate.Month,
			   gDate.Year);

	   CDC_Transmit_FS((uint8_t*)rtc_msg, strlen(rtc_msg));

	   //uint8_t flash_id;
	   //Flash_ReadID(&flash_id);

	   //char flash_text[50];
	   //sprintf(flash_text, "The flash ID is: 0x%02X\r\n", (uint8_t)flash_id);
	   //CDC_Transmit_FS((uint8_t*)flash_text, strlen(flash_text));
	   try_flash_25Q64FVSIG();


   }

   testing_RS485();

	/*
    //turn on off LED using  serial command
        if (usb_rx_flag)
        {
    		   usb_rx_flag = 0;

    		   if (strncmp((char*)usb_rx_buffer, "eat", 3) == 0)
    		   {
    			   //HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    			   // String found
    			   char *reply = "Yes, you said eat\n";
    			   CDC_Transmit_FS(reply, strlen(reply)); // Send data via USB

    		   }else if(strncmp((char*)usb_rx_buffer, "LED ON", 6) == 0){
    			   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);

    		   }else if(strncmp((char*)usb_rx_buffer, "LED OFF", 7) == 0){
    			   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
    		   }
        }

        	//Using momentory buttons S1 to toggle LED, reading GPIO
        if(!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10)){
        		//reading S2 button press event
        	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
        }


    if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK){
    	adc_value = HAL_ADC_GetValue(&hadc1);
    }

    HAL_Delay(100);
    */
   		//re calling interrupts
   	//if (HAL_GetTick() - last_adc_time > 100) { // 100 ms
   	    //HAL_ADC_Start_IT(&hadc1);
   	    //last_adc_time = HAL_GetTick();
   	//}

  /* USER CODE END 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){

	if(hcan->Instance == CAN1){
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader1, RxData1);
	}else if(hcan->Instance == CAN2){
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader2, RxData2);
		if(RxHeader2.StdId == 0x321){
			char can_message[50];
			sprintf(can_message, "\nCan message received\r\n");
			CDC_Transmit_FS((uint8_t*)can_message, strlen(can_message));
		}
	}
}

	//this function is here for us to test 25Q64FV on-board flash functionality, fully working code
void try_flash_25Q64FVSIG(){
		//reading ID
	char flash_text[50];
	uint8_t id[3];
	Flash_ReadID(id);

	sprintf(flash_text, "The Flash ID: %02X %02X %02X\r\n", id[0], id[1], id[2]);
		//USB none blocking
	while(CDC_Transmit_FS((uint8_t*)flash_text, strlen(flash_text)) == USBD_BUSY);

	uint8_t writeData[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	Flash_WriteEnable();
	Flash_SectorErase(0x000000);
	while(Flash_ReadStatus() & 0x01);

	HAL_Delay(1);

		//programming data
	Flash_PageProgram(0x000000, writeData, 16);

		//reading data to buffer
	uint8_t readData[16];
	Flash_ReadData(0x000000, readData, 16);


		//printing the content we read
	for(int i=0; i < 16; i +=4){
		sprintf(flash_text, "%02X %02X %02X %02X\r\n", readData[i], readData[i+1], readData[i+2], readData[i+3]);
			//avoid blocking of USB
		while(CDC_Transmit_FS((uint8_t*)flash_text, strlen(flash_text)) == USBD_BUSY);
	}

	  //next step for FLASH file Writing
		   //circular data logger for your ADC + timestamp
}
/*
	//ADC callback function
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){

	if(hadc->Instance == ADC1){
		adc_value = HAL_ADC_GetValue(hadc);
		adc_voltage_reading = ((float)adc_value / 4095.0) * 3.3f;
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
		//HAL_ADC_Start_IT(hadc);
	}
}

*/

void testing_RS485(){
	uint8_t tx_data[] = "Hello RS485\r\n";

	if ((HAL_GetTick() - last_rs485_time) > 5000){

		last_rs485_time = HAL_GetTick();

			//sending RS485 data
		RS485_Send(tx_data, sizeof(tx_data) - 1);
			//adding slight delay
		HAL_Delay(100);

			//Receiving RS485 data
		RS485_Receive();

			//monitoring on the USB
		CDC_Transmit_FS(rs_485_rx_buffer, strlen((char*)rs_485_rx_buffer));

			//adding slight delay
		HAL_Delay(100);
	}
}


//Pin interrupt call back function
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_5)
    {
        // Read interrupt flags
    }
}


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
  while (1)
  {
	  /* USER CODE BEGIN 5 */
	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
	  HAL_Delay(1000);
	  /* USER CODE END 5 */
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
