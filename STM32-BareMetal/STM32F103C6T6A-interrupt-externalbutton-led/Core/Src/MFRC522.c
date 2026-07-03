#include "MFRC522.h"
#include <string.h>

// CS kontrol makroları
#define CS_LOW(dev)  HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_RESET)
#define CS_HIGH(dev) HAL_GPIO_WritePin(dev->csPort, dev->csPin, GPIO_PIN_SET)

static void MFRC522_WriteReg(MFRC522_HandleTypeDef* dev, uint8_t reg, uint8_t val) {
    uint8_t addr = (reg << 1) & 0x7E; // bit7=0 → write
    CS_LOW(dev);
    HAL_SPI_Transmit(dev->hspi, &addr, 1, 10);
    HAL_SPI_Transmit(dev->hspi, &val, 1, 10);
    CS_HIGH(dev);
}

static uint8_t MFRC522_ReadReg(MFRC522_HandleTypeDef* dev, uint8_t reg) {
    uint8_t addr = ((reg << 1) & 0x7E) | 0x80; // bit7=1 → read
    uint8_t val = 0;
    CS_LOW(dev);
    HAL_SPI_Transmit(dev->hspi, &addr, 1, 10);
    HAL_SPI_Receive(dev->hspi, &val, 1, 10);
    CS_HIGH(dev);
    return val;
}

static void MFRC522_SetBitMask(MFRC522_HandleTypeDef* dev, uint8_t reg, uint8_t mask) {
    MFRC522_WriteReg(dev, reg, MFRC522_ReadReg(dev, reg) | mask);
}

static void MFRC522_ClearBitMask(MFRC522_HandleTypeDef* dev, uint8_t reg, uint8_t mask) {
    MFRC522_WriteReg(dev, reg, MFRC522_ReadReg(dev, reg) & ~mask);
}

void MFRC522_Init(MFRC522_HandleTypeDef* dev) {
    // Soft reset
    MFRC522_WriteReg(dev, MFRC522_REG_COMMAND, MFRC522_CMD_SOFTRESET);
    HAL_Delay(10);

    // Temel konfigürasyon
    MFRC522_WriteReg(dev, MFRC522_REG_TX_ASK, 0x40); // 100% ASK modulation
    MFRC522_WriteReg(dev, MFRC522_REG_MODE, 0x3D); // CRC preset 0x6363
    MFRC522_WriteReg(dev, MFRC522_REG_TX_CONTROL,
                     MFRC522_ReadReg(dev, MFRC522_REG_TX_CONTROL) | 0x03); // Anteni aç
}

static uint8_t MFRC522_Transceive(MFRC522_HandleTypeDef* dev,
                                  uint8_t* sendData, uint8_t sendLen,
                                  uint8_t* backData, uint8_t* backLen) {
    // IRQ bitleri temizle
    MFRC522_WriteReg(dev, MFRC522_REG_COM_IRQ, 0x7F);
    // FIFO temizle
    MFRC522_SetBitMask(dev, MFRC522_REG_FIFO_LEVEL, 0x80);
    MFRC522_WriteReg(dev, MFRC522_REG_COMMAND, MFRC522_CMD_IDLE);

    // Veri yükle
    for (uint8_t i = 0; i < sendLen; i++)
        MFRC522_WriteReg(dev, MFRC522_REG_FIFO_DATA, sendData[i]);

    MFRC522_WriteReg(dev, MFRC522_REG_COMMAND, MFRC522_CMD_TRANSCEIVE);
    MFRC522_SetBitMask(dev, MFRC522_REG_BIT_FRAMING, 0x80); // Gönderimi başlat

    // Cevap bekle (timeout ~25ms)
    uint16_t i = 2500;
    uint8_t n;
    do {
        n = MFRC522_ReadReg(dev, MFRC522_REG_COM_IRQ);
        i--;
    }
    while (i && !(n & 0x30)); // RxIRq veya IdleIRq

    MFRC522_ClearBitMask(dev, MFRC522_REG_BIT_FRAMING, 0x80);

    if (!i) return 0xFF; // timeout
    if (MFRC522_ReadReg(dev, MFRC522_REG_ERROR) & 0x1B) return 0xFE; // hata

    if (n & 0x01) return 0xFD; // hata IRQ

    n = MFRC522_ReadReg(dev, MFRC522_REG_FIFO_LEVEL);
    if (n > *backLen) n = *backLen;
    *backLen = n;

    for (uint8_t i = 0; i < n; i++)
        backData[i] = MFRC522_ReadReg(dev, MFRC522_REG_FIFO_DATA);

    return 0x00; // OK
}

bool MFRC522_IsNewCardPresent(MFRC522_HandleTypeDef* dev) {
    uint8_t cmd = PICC_CMD_REQA;
    uint8_t resp[2] = {0};
    uint8_t respLen = 2;

    MFRC522_WriteReg(dev, MFRC522_REG_BIT_FRAMING, 0x07); // 7-bit frame
    return (MFRC522_Transceive(dev, &cmd, 1, resp, &respLen) == 0x00);
}

bool MFRC522_ReadCardSerial(MFRC522_HandleTypeDef* dev, MFRC522_UID* uid) {
    uint8_t cmd[2] = {PICC_CMD_ANTICOLL, 0x20};
    uint8_t resp[5] = {0};
    uint8_t respLen = 5;

    MFRC522_WriteReg(dev, MFRC522_REG_BIT_FRAMING, 0x00);

    if (MFRC522_Transceive(dev, cmd, 2, resp, &respLen) != 0x00)
        return false;

    // BCC kontrolü (XOR checksum)
    uint8_t bcc = resp[0] ^ resp[1] ^ resp[2] ^ resp[3];
    if (bcc != resp[4]) return false;

    memcpy(uid->uid, resp, 4);
    uid->size = 4;
    return true;
}
