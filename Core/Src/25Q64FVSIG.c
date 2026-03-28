/*
 * 25Q64FVSIG.c
 *
 *  Created on: 28/03/2026
 *      Author: Thilanga
 */
#include <25Q64FVSIG.h>


uint8_t SPI_Transfer(uint8_t data) {
    uint8_t received;
    HAL_SPI_TransmitReceive(&hspi2, &data, &received, 1, HAL_MAX_DELAY);
    return received;
}

void Flash_WriteEnable(void) {
    FLASH_CS_LOW();
    uint8_t cmd = CMD_WRITE_ENABLE;
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    FLASH_CS_HIGH();
}

uint8_t Flash_ReadStatus(void) {
    uint8_t status, cmd = CMD_READ_STATUS;
    FLASH_CS_LOW();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, &status, 1, HAL_MAX_DELAY);
    FLASH_CS_HIGH();
    return status;
}

void Flash_ReadID(uint8_t *id) {
    FLASH_CS_LOW();
    uint8_t cmd = CMD_READ_ID;
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, id, 3, HAL_MAX_DELAY); // Manufacturer, Memory Type, Capacity
    FLASH_CS_HIGH();
}

void Flash_ReadData(uint32_t address, uint8_t *buffer, uint16_t length) {
    FLASH_CS_LOW();
    uint8_t cmd[4];
    cmd[0] = CMD_READ_DATA;
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;
    HAL_SPI_Transmit(&hspi2, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, buffer, length, HAL_MAX_DELAY);
    FLASH_CS_HIGH();
}

void Flash_PageProgram(uint32_t address, uint8_t *buffer, uint16_t length) {
    Flash_WriteEnable();
    FLASH_CS_LOW();
    uint8_t cmd[4];
    cmd[0] = CMD_PAGE_PROGRAM;
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;
    HAL_SPI_Transmit(&hspi2, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi2, buffer, length, HAL_MAX_DELAY);
    FLASH_CS_HIGH();

    // Wait until write completes
    while(Flash_ReadStatus() & 0x01);
}

