#ifndef MFRC522_H
#define MFRC522_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// Register map
#define MFRC522_REG_COMMAND      0x01
#define MFRC522_REG_COM_IRQ      0x04
#define MFRC522_REG_DIV_IRQ      0x05
#define MFRC522_REG_ERROR        0x06
#define MFRC522_REG_STATUS2      0x08
#define MFRC522_REG_FIFO_DATA    0x09
#define MFRC522_REG_FIFO_LEVEL   0x0A
#define MFRC522_REG_CONTROL      0x0C
#define MFRC522_REG_BIT_FRAMING  0x0D
#define MFRC522_REG_MODE         0x11
#define MFRC522_REG_TX_CONTROL   0x14
#define MFRC522_REG_TX_ASK       0x15
#define MFRC522_REG_CRC_RESULT_H 0x21
#define MFRC522_REG_CRC_RESULT_L 0x22
#define MFRC522_REG_VERSION      0x37

// Komutlar
#define MFRC522_CMD_IDLE         0x00
#define MFRC522_CMD_TRANSCEIVE   0x0C
#define MFRC522_CMD_SOFTRESET    0x0F
#define MFRC522_CMD_CALC_CRC     0x03

// PICC komutları
#define PICC_CMD_REQA            0x26
#define PICC_CMD_ANTICOLL        0x93

typedef struct {
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
} MFRC522_HandleTypeDef;

typedef struct {
    uint8_t uid[5];
    uint8_t size;
} MFRC522_UID;

void MFRC522_Init(MFRC522_HandleTypeDef* dev);
bool MFRC522_IsNewCardPresent(MFRC522_HandleTypeDef* dev);
bool MFRC522_ReadCardSerial(MFRC522_HandleTypeDef* dev, MFRC522_UID* uid);

#endif
