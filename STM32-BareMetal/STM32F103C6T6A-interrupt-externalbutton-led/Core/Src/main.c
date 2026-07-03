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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "MFRC522.h"
#define MY_MAP(x, in_min, in_max, out_min, out_max) \
    (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

#define DS3231_ADDR  0x68 << 1  // HAL için 8-bit adres = 0xD0
#define DS3231_EEPROM_AT24C32_ADDR  (0x57 << 1)  // 0xAE
#define LCD_ADDR (0x27 << 1) // veya 0x4E

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
MFRC522_HandleTypeDef rfid;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile int counter = 0;
uint8_t channelIndex = 0;
uint32_t adcChannels[4];
uint32_t myAdcValueNTC = 0;
uint8_t myAdcShortValueCelsius = 0;
uint16_t timerCounter = 0;
uint16_t ledBrightness = 0;

uint8_t rxBufferDMA[20];

uint8_t rxDataChar;
uint8_t rxDataStringBuffer[10];
uint8_t rxReadyString[10];
uint8_t rxIndex = 0;
bool rxDataReadyFlag = 0;
bool txSuccessFlag = 0;


char txMsgDMA[] = "Hello DMA World!\r\n";


int _write(int file, char* ptr, int len) {
    // Veriyi UART üzerinden bloklayıcı modda gönder
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return (status == HAL_OK) ? len : 0;
}

void printBinary(uint8_t v) {
    for (int i = 7; i >= 0; i--)
        printf("%d", (v >> i) & 1);
}


///////////// DS3231 RTC BEGIN ////////////////

uint8_t dec2bcd(uint8_t val) { return ((val / 10) << 4) | (val % 10); }
uint8_t bcd2dec(uint8_t val) { return ((val >> 4) * 10) + (val & 0x0F); }


typedef struct {
    uint8_t sn, dk, sa;
    uint8_t gun, tarih, ay, yil;
} DS3231_t;

void DS3231_GetDateTime(DS3231_t* dt) {
    uint8_t reg = 0x00; // cursor'ı 0x00'a taşı
    uint8_t buf[7];

    // Önce cursor'ı 0x00'a al
    HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR, &reg, 1, HAL_MAX_DELAY);

    // 7 byte oku: sn, dk, sa, gun, tarih, ay, yil
    // Master 7×8 = 56 clock üretir, her byte sonrası ACK gönderir
    // Son byte'tan sonra NACK gönderip STOP ile iletişimi keser
    HAL_I2C_Master_Receive(&hi2c1, DS3231_ADDR, buf, 7, HAL_MAX_DELAY);

    dt->sn = bcd2dec(buf[0] & 0x7F); // bit7 her zaman 0, maskeliyoruz
    dt->dk = bcd2dec(buf[1] & 0x7F);
    dt->sa = bcd2dec(buf[2] & 0x3F); // bit7,6 mod bitleri, maskeliyoruz
    dt->gun = bcd2dec(buf[3] & 0x07); // bit2-0 geçerli
    dt->tarih = bcd2dec(buf[4] & 0x3F);
    dt->ay = bcd2dec(buf[5] & 0x1F); // bit4-0 geçerli
    dt->yil = bcd2dec(buf[6]);
}

void DS3231_SetDateTime(uint8_t sn, uint8_t dk, uint8_t sa,
                        uint8_t gun, uint8_t tarih, uint8_t ay, uint8_t yil) {
    uint8_t data[8];

    data[0] = 0x00; // cursor'ı 0x00'a (saniye register'ına) taşı
    data[1] = dec2bcd(sn); // 0x00 → saniye
    data[2] = dec2bcd(dk); // 0x01 → dakika
    data[3] = dec2bcd(sa); // 0x02 → saat
    data[4] = dec2bcd(gun); // 0x03 → haftanın günü
    data[5] = dec2bcd(tarih); // 0x04 → ayın günü
    data[6] = dec2bcd(ay); // 0x05 → ay
    data[7] = dec2bcd(yil); // 0x06 → yıl (2025 için 25 gönder)

    // 8 byte gönder: 1 byte register adresi + 7 byte veri
    HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR, data, 8, HAL_MAX_DELAY);
}

// Tek byte yazma
void EEPROM_WriteByte(uint16_t adres, uint8_t veri) {
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, DS3231_EEPROM_AT24C32_ADDR, adres, I2C_MEMADD_SIZE_16BIT,
                                                 &veri, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        printf("EEPROM Yazma Hatasi: %d\r\n", status);
    }
    HAL_Delay(10); // EEPROM dahili yazma döngüsü için bekleme (genelde 5ms sürer)
}

// Tek byte okuma
uint8_t EEPROM_ReadByte(uint16_t adres) {
    uint8_t veri;
    HAL_I2C_Mem_Read(&hi2c1, DS3231_EEPROM_AT24C32_ADDR, adres, I2C_MEMADD_SIZE_16BIT, &veri, 1, HAL_MAX_DELAY);
    return veri;
}


///////////// DS3231 RTC END ////////////////

///////////// LCD I2C BEGIN ////////////////

uint8_t lcd_backlight_val = 0x08; // Varsayılan olarak açık (0x08)

void lcd_send_cmd(char cmd) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x04 | lcd_backlight_val; // en=1, rs=0
    data_t[1] = data_u | lcd_backlight_val; // en=0, rs=0
    data_t[2] = data_l | 0x04 | lcd_backlight_val; // en=1, rs=0
    data_t[3] = data_l | lcd_backlight_val; // en=0, rs=0
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t*)data_t, 4, 100);
}

void lcd_send_data(char data) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x05 | lcd_backlight_val; // EN=1, RS=1
    data_t[1] = data_u | 0x01 | lcd_backlight_val; // EN=0, RS=1
    data_t[2] = data_l | 0x05 | lcd_backlight_val; // EN=1, RS=1
    data_t[3] = data_l | 0x01 | lcd_backlight_val; // EN=0, RS=1
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t*)data_t, 4, 100);
}

// LCD Başlatma Ayarları
void lcd_init(void) {
    HAL_Delay(50); // LCD'nin kendine gelmesi için bekleme
    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20); // 4-bit moduna geçiş
    HAL_Delay(10);

    lcd_send_cmd(0x28); // 2 satır, 5x8 font
    HAL_Delay(1);
    lcd_send_cmd(0x08); // Ekran kapalı
    HAL_Delay(1);
    lcd_send_cmd(0x01); // Ekranı temizle
    HAL_Delay(2);
    lcd_send_cmd(0x06); // Giriş modu: Otomatik artır
    HAL_Delay(1);
    lcd_send_cmd(0x0C); // Ekran açık, imleç kapalı
}

// Ekrana string yazdırma fonksiyonu
void lcd_send_string(char* str) {
    while (*str) lcd_send_data(*str++);
}

// Belirli bir koordinata gitme
void lcd_put_cur(int row, int col) {
    switch (row) {
    case 0: col |= 0x80;
        break;
    case 1: col |= 0xC0;
        break;
    }
    lcd_send_cmd(col);
}

void lcd_backlight_on(void) {
    lcd_backlight_val = 0x08;
    lcd_send_cmd(0x00); // Durumu güncellemek için boş komut gönder
}

void lcd_backlight_off(void) {
    lcd_backlight_val = 0x00;
    lcd_send_cmd(0x00); // Durumu güncellemek için boş komut gönder
}

//////////// LCD I2C END ///////////////////


void HAL_SYSTICK_Callback(void) {
    ledBrightness = MY_MAP(adcChannels[1], 0, 4096, 0, 1000);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ledBrightness);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, ledBrightness);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_12) {
        GPIO_PinState btnPinStatus = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, btnPinStatus);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, btnPinStatus);

        strcpy((char*)txMsgDMA, "Trigger inside\r\n");
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)txMsgDMA, strlen(txMsgDMA));

        //DS3231_SetDateTime(0, 29, 22, 3, 9, 4, 26); // 14:30:00, Salı, 01.04.2025
        //printf("Tarih Yazildi");
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM1)
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        if (rxDataChar == '\n') {
            for (int i = 0; i < 10; i++) {
                rxReadyString[i] = 0;
            }
            for (int i = 0; i < rxIndex; i++) {
                rxReadyString[i] = rxDataStringBuffer[i];
            }
            rxIndex = 0;
            rxDataReadyFlag = true;
        }
        else {
            if (rxIndex < 10) {
                rxDataStringBuffer[rxIndex] = rxDataChar;
                rxIndex++;
            }
            rxDataReadyFlag = false;
        }
        printf("%s", rxBufferDMA);
        // Bir sonraki baytı alabilmek için kesmeyi tekrar aktif etmelisiniz.
        //HAL_UART_Receive_IT(&huart1, &rxDataChar, 1);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        // Veri başarıyla gönderildi.
        // Burada bir bayrağı (flag) set edebilir veya yeni bir transfer başlatabilirsiniz.
        txSuccessFlag = true;
    }
}

/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        adcChannels[channelIndex] = HAL_ADC_GetValue(hadc);
        channelIndex++;

        if (channelIndex >= 2) {
            channelIndex = 0;
            // Buraya "Tüm kanallar okundu" bayrağı koyabilirsin
        }
        // HAL_ADC_Start_IT(hadc);
        //myAdcValueNTC = HAL_ADC_GetValue(hadc);
        //myAdcShortValueCelsius = MY_MAP(myAdcValueNTC, 1800, 3000, 15, 65);
        // Eğer Continuous Mode "Disabled" ise, bir sonraki ölçüm için tekrar başlatman gerekir:
        HAL_ADC_Start_IT(hadc);
    }
}
*/


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    /* USER CODE BEGIN 2 */
    HAL_ADC_Start_DMA(&hadc1, adcChannels, 4);
    //HAL_ADCEx_Calibration_Start(&hadc1);
    //HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    //HAL_UART_Receive_IT(&huart1, &rxDataChar, 1);
    HAL_UART_Receive_DMA(&huart1, rxBufferDMA, 20);
    ////////////////////////////////////////////
    rfid.hspi = &hspi1;
    rfid.csPort = GPIOA;
    rfid.csPin = GPIO_PIN_4;


    MFRC522_Init(&rfid);
    ////////////////////////////////////////////


    // printf("Sistem baslatildi...\n");

    // printf("I2C slave cihazlar taraniyor...\n");

    /*
    for (uint8_t addr = 1; addr < 128; addr++) {
        if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 1, 10) == HAL_OK) {
            // printf("Cihaz bulundu: 0x%02X\r\n", addr);
        }
    }
    */
    // DS3231 EEPROM
    EEPROM_WriteByte(0x0000, 0x65); // 0. adrese 65 yaz
    EEPROM_WriteByte(0x0001, 0x33);
    EEPROM_WriteByte(0x0002, 0x22);


    // DS3231_SetDateTime(0, 47, 20, 3, 1, 4, 26); // 14:30:00, Salı, 01.04.2025
    // printf("Tarih Yazildix");


    /* USER CODE BEGIN 2 */
    lcd_init();

    lcd_put_cur(0, 0); // 1. satır başı
    lcd_send_string("STM32 Hazir!");

    lcd_put_cur(1, 0); // 2. satır başı
    lcd_send_string("I2C Basarili");

    char buf[64];
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        /*
        if (rxDataReadyFlag) {
            HAL_UART_Transmit(&huart1, rxReadyString, 10, 0xFFFF);
            rxDataReadyFlag = false;
        }
        */

        DS3231_t zaman;
        DS3231_GetDateTime(&zaman);

        printf("%02d:%02d:%02d  %02d/%02d/20%02d\r\n",
               zaman.sa, zaman.dk, zaman.sn,
               zaman.tarih, zaman.ay, zaman.yil);


        uint8_t eeprom_buffer[3];
        HAL_I2C_Mem_Read(&hi2c1, DS3231_EEPROM_AT24C32_ADDR, 0x0000, I2C_MEMADD_SIZE_16BIT, eeprom_buffer, 3,
                         HAL_MAX_DELAY);
        /*
        printf("EEPROM: 0x%02X, 0x%02X, 0x%02X\r\n", eeprom_buffer[0], eeprom_buffer[1], eeprom_buffer[2]);
        */


        strcpy((char*)txMsgDMA, "While Loop\r\n");
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)txMsgDMA, strlen(txMsgDMA));

        // printf("loop...\n");

        //////////////////////////////////////
        HAL_Delay(500);
        if (MFRC522_IsNewCardPresent(&rfid)) {
            MFRC522_UID uid;
            if (MFRC522_ReadCardSerial(&rfid, &uid)) {
                snprintf(buf, sizeof(buf),
                         "UID: %02X %02X %02X %02X\r\n",
                         uid.uid[0], uid.uid[1], uid.uid[2], uid.uid[3]);
                HAL_UART_Transmit_DMA(&huart1, (uint8_t*)buf, strlen(buf));
            }
        }
        HAL_Delay(200);

        //////////////////////////////////////
        HAL_Delay(500);
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void) {
    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 4;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void) {
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 98;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void) {
    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void) {
    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 14399;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 9999;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void) {
    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 71;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
    HAL_TIM_MspPostInit(&htim2);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void) {
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
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) {
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */

    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_4, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pins : PA0 PA4 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PB12 */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
