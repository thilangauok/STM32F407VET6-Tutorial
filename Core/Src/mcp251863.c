/*
 * mcp251863.c
 *
 *  Created on: 29/03/2026
 *      Author: thila
 */

#include "mcp251863.h"
#include "spi.h"

	//pin select operation, set PIN low for SPI to start
static void MCP_Select() {
    HAL_GPIO_WritePin(MCP_CS_PORT, MCP_CS_PIN, GPIO_PIN_RESET);
}

//pin select operation, set PIN high for SPI to stop
static void MCP_Unselect() {
    HAL_GPIO_WritePin(MCP_CS_PORT, MCP_CS_PIN, GPIO_PIN_SET);
}

void MCP251863_Reset(void)
{
	//to reset we need to write 0x0 to address 0x000, datasheet page 70
    uint8_t cmd[2];
    cmd[0] = MCP_CMD_RESET << 4 | 0x00;
    cmd[1] = 0x00;

    MCP_Select();
    HAL_SPI_Transmit(&hspi2, cmd, 2, HAL_MAX_DELAY);
    MCP_Unselect();

    HAL_Delay(10);
}

void MCP251863_WriteReg(uint16_t addr, uint32_t data)
{
    uint8_t tx[6];

    tx[0] = MCP_CMD_WRITE << 4 | ((addr >> 8) & 0x0F);
    tx[1] = addr & 0xFF;

    tx[2] = (data >> 0) & 0xFF;
    tx[3] = (data >> 8) & 0xFF;
    tx[4] = (data >> 16) & 0xFF;
    tx[5] = (data >> 24) & 0xFF;

    MCP_Select();
    HAL_SPI_Transmit(&hspi2, tx, 6, HAL_MAX_DELAY);
    MCP_Unselect();
}

uint8_t MCP251863_SetMode(uint8_t mode)
{
    uint32_t reg;
    uint32_t timeout = MCP_MODE_TIMEOUT;

    // 1. Read current CiCON
    reg = MCP251863_ReadReg(OPMODE_CONFIG);

    // 2. Clear REQOP bits [26:24]
    reg &= ~(MCP_OP_MASK << MCP_REQOP_POS);

    // 3. Set new mode
    reg |= ((mode & MCP_OP_MASK) << MCP_REQOP_POS);

    // 4. Write back
    MCP251863_WriteReg(MCP_REG_CiCON, reg);

    // 5. Wait until OPMOD matches REQOP
    do {
        reg = MCP251863_ReadReg(MCP_REG_CiCON);
        if (((reg >> MCP_OPMOD_POS) & MCP_OP_MASK) == mode)
            return 0; // SUCCESS
    } while (timeout--);

    return 1; // TIMEOUT ERROR
}

uint32_t MCP251863_ReadReg(uint16_t addr)
{
    uint8_t tx[6] = {0};
    uint8_t rx[6] = {0};

    tx[0] = (MCP_CMD_READ << 4) | ((addr >> 8) & 0x0F);
    tx[1] = addr & 0xFF;

    MCP_Select();
    HAL_SPI_TransmitReceive(&hspi2, tx, rx, 6, HAL_MAX_DELAY);
    MCP_Unselect();

    return (rx[2] |
           (rx[3] << 8) |
           (rx[4] << 16) |
           (rx[5] << 24));
}

void MCP251863_Init(void)
{
    // Reset device
    MCP251863_Reset();

    // Enter configuration mode
    if (MCP251863_SetMode(OPMODE_CONFIG) != 0)
    {
        // handle error
    }

    // Nominal bit timing (500 kbps)
    MCP251863_WriteReg(MCP_REG_NBTCFG, MCP_NOMINAL_CFG);

    // Data bit timing (2 Mbps)
    MCP251863_WriteReg(MCP_REG_DBTCFG, MCP_DATA_CFG);

    // Transmitter Delay Compensation
    MCP251863_WriteReg(MCP_REG_TDC, 0x00000010);

    // Switch to normal mode
    if (MCP251863_SetMode(OPMODE_NORMAL) != 0)
    {
        // handle error
    }

}


// ---------------- SEND TEST FRAME ----------------
void MCP251863_SendTestFrame(void)
{
    uint32_t addr = MCP251863_ReadReg(MCP_REG_CiFIFOUA1); // FIFO0 base

    // ---------------- WORD0: ID + flags ----------------
    // Standard 11-bit ID 0x123, IDE=0 (standard), RTR=0
    uint32_t word0 = (0x123 << 21);
    MCP251863_WriteReg(addr + 0, word0);

    // ---------------- WORD1: DLC ----------------
    uint32_t word1 = (8 << 16); // DLC = 8 bytes
    MCP251863_WriteReg(addr + 4, word1);

    // ---------------- WORD2+: DATA ----------------
    MCP251863_WriteReg(addr + 8, 0x11223344);
    MCP251863_WriteReg(addr + 12, 0x55667788);

    // ---------------- REQUEST TRANSMIT ----------------
    uint32_t fifoCtrl = MCP251863_ReadReg(MCP_REG_CiFIFOCON1);
    fifoCtrl |= (1 << 7);  // TXEN , might need to do earlier
    fifoCtrl |= (1 << 8); // TXREQ
    fifoCtrl |= (1 << 9); // UINC → increment FIFO pointer
    MCP251863_WriteReg(MCP_REG_CiFIFOCON1, fifoCtrl);
}



