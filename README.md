# STM32F407VET6 Discovery – Peripheral Drivers & Examples

This repository provides embedded firmware examples for the STM32F407VET6 microcontroller, demonstrating multiple peripheral interfaces and communication protocols using STM32 HAL/LL drivers.

## Features

This project includes implementations of the following peripherals:

- CAN (Controller Area Network)
- GPIO (Digital Input/Output)
- SD Card (SPI or SDIO interface with FATFS)
- RS485 (UART-based communication)
- USB Device (CDC / optional HID)
- External SPI Flash (W25Q64FVS)
- ADC
  - Interrupt-based conversion
  - DMA-based continuous sampling

## Hardware Platform

- MCU: STM32F407VET6 (Cortex-M4, 168 MHz)
- Board: STM32F407 Discovery or compatible custom board
- External Flash: W25Q64FVS (64Mbit SPI NOR Flash)

## Toolchain

- STM32CubeMX / STM32CubeIDE
- ARM GCC Toolchain
- ST-Link Debugger

## Project Structure

/Core  
  /Src        Application source files  
  /Inc        Header files  

/Drivers  
  /STM32F4xx_HAL_Driver  
  /CMSIS  

/Modules  
  /CAN  
  /GPIO  
  /SD_Card  
  /RS485  
  /USB_Device  
  /W25Q64  
  /ADC  

/Middlewares  
  /FatFS      SD Card filesystem  
  /USB        USB Device stack  

## Peripheral Details

### CAN Bus

- Uses bxCAN peripheral
- Supports standard and extended identifiers
- Interrupt-driven reception
- Example implementations:
  - Loopback test
  - Multi-node communication

### GPIO

- Input (polling and interrupt modes)
- Output (push-pull and open-drain)
- External interrupt (EXTI)

### SD Card

- Interface: SPI or SDIO
- Filesystem: FATFS
- Features:
  - File read/write
  - Mount and unmount
  - Directory handling

### RS485 Communication

- UART-based half-duplex communication
- Direction control via DE/RE pin
- Supports polling and interrupt modes
- Suitable for Modbus RTU applications

### USB Device

- USB CDC (Virtual COM Port)
- Optional HID support
- Enables communication with PC over USB

### W25Q64 SPI Flash

- Manufacturer: Winbond
- Interface: SPI
- Features:
  - Read, write, erase operations
  - Sector and block erase
  - JEDEC ID detection
- Suitable for data storage and logging

### ADC (Analog-to-Digital Converter)

#### Interrupt Mode
- Single or multiple channel conversion
- End-of-conversion interrupt callback

#### DMA Mode
- Continuous conversion
- Circular buffer support
- Reduced CPU usage

## Configuration

Peripheral configuration can be done using:

- STM32CubeMX (.ioc file if included)
- Manual HAL initialization

Key configuration points:

- System clock configured to 168 MHz using PLL
- ADC sampling time adjustment
- CAN bit timing configuration
- SPI clock speed tuning for SD card and flash

## Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/thilangauok/STM32F407VET6-Tutorial.git

## License

This project is licensed under the MIT License.

## Contributing

Contributions are welcome. Please open an issue to discuss major changes before submitting a pull request.

## Contact

- Email: thilanganz@gmail.com
