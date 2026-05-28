/**
 * @file SPI.h
 * @brief SPI driver for STM32F411RE (Master mode, polling)
 *
 * This header provides function declarations for initializing the SPI2 peripheral,
 * transmitting/receiving data, and controlling the chip select line via GPIO.
 *
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stm32f4xx.h>   // For STM32F4 register definitions (I2C, RCC, etc.)
#include "GPIO.h"


/**
 * @brief Initialize SPI2 peripheral with default configuration.
 * @details Default settings:
 *          - Master mode
 *          - Baud rate divider = 16 (SCK = PCLK / 16)
 *          - 8-bit data frame
 *          - CPOL = 0, CPHA = 0 (Mode 0)
 *          - Software slave management (SSM + SSI)
 *          - Peripheral enabled
 */
void spi_init(void);

/**
 * @brief Transmit multiple bytes over SPI bus (blocking).
 * @param data  Pointer to buffer containing data to send.
 * @param len   Number of bytes to transmit.
 * @note Waits for TXE flag before each byte and BSY after the last byte.
 */
void spi_transmit(uint8_t *data, uint32_t len);

/**
 * @brief Receive multiple bytes over SPI bus (blocking).
 * @param data  Buffer to store received data.
 * @param len   Number of bytes to receive.
 * @note Sends dummy bytes (0x00) to generate the clock.
 */
void spi_receive(uint8_t *data, uint32_t len);

/**
 * @brief Enable chip select (drive CS low) to select a device.
 * @param p    Port index (A, B, C, D, E, H from port_t enum).
 * @param pin  Pin number (0..15).
 * @pre GPIO pin must be configured as output push-pull.
 */
void spi_csEnable(port_t p, uint8_t pin);

/**
 * @brief Disable chip select (drive CS high) to deselect a device.
 * @param p    Port index (A, B, C, D, E, H from port_t enum).
 * @param pin  Pin number (0..15).
 * @pre GPIO pin must be configured as output push-pull.
 */
void spi_csDisable(port_t p, uint8_t pin);

#endif // SPI_H