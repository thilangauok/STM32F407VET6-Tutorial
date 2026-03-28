/*
 * 25Q64FVSIG.h
 *
 *  Created on: 28/03/2026
 *      Author: Thilanga Ariyarathna
 */
#include <spi.h>
#include <main.h>

#ifndef INC_25Q64FVSIG_H_
#define INC_25Q64FVSIG_H_

#define CMD_READ_ID        0x9F
#define CMD_READ_DATA      0x03
#define CMD_PAGE_PROGRAM   0x02
#define CMD_SECTOR_ERASE   0x20
#define CMD_WRITE_ENABLE   0x06
#define CMD_READ_STATUS    0x05

#define MAX_TIME_DELAY_FLASH 500

#define FLASH_CS_LOW()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)  // Adjust pin
#define FLASH_CS_HIGH()    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

	//function prototypes
void Flash_ReadID(uint8_t *id);
void Flash_ReadData(uint32_t address, uint8_t *buffer, uint16_t length);
void Flash_PageProgram(uint32_t address, uint8_t *buffer, uint16_t length);
uint8_t Flash_ReadStatus(void);
void Flash_SectorErase(uint32_t address);
void Flash_WriteEnable(void);

#endif /* INC_25Q64FVSIG_H_ */
