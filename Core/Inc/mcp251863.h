/*
 * mcp251863.h
 *
 *  Created on: 29/03/2026
 *      Author: thila
 */

#ifndef INC_MCP251863_H_
#define INC_MCP251863_H_

#include "main.h"

#define MCP_NOMINAL_CFG   0x0000040A  // 500 kbps @ 40MHz
#define MCP_DATA_CFG      0x00000103  // 2 Mbps data phase

	// Select mcp  chip select pin and
#define MCP_CS_PORT GPIOA
#define MCP_CS_PIN  GPIO_PIN_4

// Commands
#define MCP_CMD_RESET        0x0
#define MCP_CMD_READ         0x3
#define MCP_CMD_WRITE        0x2

// Registers
#define MCP_REG_CiCON        0x000
#define MCP_REG_NBTCFG       0x004
#define MCP_REG_DBTCFG       0x008
#define MCP_REG_TDC          0x00C
#define MCP_REG_CiFIFOUA1    0x064
#define MCP_REG_CiFIFOCON1   0x05C


// Bit positions
#define MCP_REQOP_POS   24
#define MCP_OPMOD_POS   21
#define MCP_OP_MASK     0x7

//opmode
#define OPMODE_NORMAL 	0x0
#define OPMODE_CONFIG	0x4

// Timeout (adjust if needed)
#define MCP_MODE_TIMEOUT  100000



void MCP251863_Init(void);
void MCP251863_Reset(void);
void MCP251863_WriteReg(uint16_t addr, uint32_t data);
uint32_t MCP251863_ReadReg(uint16_t addr);
uint8_t MCP251863_SetMode(uint8_t mode);
uint32_t MCP251863_ReadReg(uint16_t addr);



#endif /* INC_MCP251863_H_ */
