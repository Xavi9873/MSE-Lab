/**
 * @file SPI.c
 * @brief SPI driver for STM32F411RE (Master mode, polling)
 *
 * This driver provides initialization, transmit, receive, and chip select control
 * for the SPI2 peripheral. Uses software slave management (GPIO controlled CS).
 *
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 */

#include "SPI.h"

/**
 * @brief Initialize SPI2 peripheral with default configuration.
 * @details Default state:
 *          - Master mode
 *          - Baud rate divider = 16 (SCK = PCLK / 16)
 *          - 8-bit data frame
 *          - CPOL = 0, CPHA = 0 (Mode 0)
 *          - Software slave management (SSM + SSI)
 *          - Peripheral enabled
 */
void spi_init(void)
{
    // Enable SPI2 clock (APB1 bus)
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    // Reset control registers
    SPI2->CR1 = 0;
    SPI2->CR2 = 0;

    // Master mode
    SPI2->CR1 |= SPI_CR1_MSTR;

    // Baud rate: BR[2:0] = 011 -> divider = 16
    SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1;

    // Ensure 8-bit data frame (DFF = 0) – already 0 after reset, explicit clear
    SPI2->CR1 &= ~SPI_CR1_DFF;

    // Software slave management: enable SSM and set SSI (internal NSS high)
    SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

    // Enable SPI peripheral
    SPI2->CR1 |= SPI_CR1_SPE;
}

/**
 * @brief Transmit multiple bytes over SPI (blocking).
 * @param data  Pointer to buffer containing data to send.
 * @param len   Number of bytes to transmit.
 */
void spi_transmit(uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        // Wait for transmit buffer empty
        while (!(SPI2->SR & SPI_SR_TXE));

        // Send data byte
        SPI2->DR = data[i];
    }

    // Wait for last byte to be fully transmitted
    while (SPI2->SR & SPI_SR_BSY);

    // (Optional) Clear overrun flag if present – read SR then DR
    if (SPI2->SR & SPI_SR_OVR) {
        (void)SPI2->SR;
        (void)SPI2->DR;
    }
}

/**
 * @brief Receive multiple bytes over SPI (blocking).
 * @param data  Buffer to store received data.
 * @param len   Number of bytes to receive.
 * @note Sends dummy bytes (0x00) to generate clock.
 */
void spi_receive(uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        // Send dummy byte to generate clock
        SPI2->DR = 0;

        // Wait for receive buffer not empty
        while (!(SPI2->SR & SPI_SR_RXNE));

        // Read received byte
        data[i] = SPI2->DR;
    }

    // Wait for last transfer to complete
    while (SPI2->SR & SPI_SR_BSY);
}

/**
 * @brief Enable chip select (drive CS low).
 * @param p    Port index (A, B, C, D, E, H from port_t enum)
 * @param pin  Pin number (0..15)
 * @pre GPIO pin must be configured as output push-pull.
 */
void spi_csEnable(port_t p, uint8_t pin)
{
    if (p < GPIOs_SIZE)
    {
        gpio[p]->BSRR = (1 << (pin + 16));   // Set BRx bit -> output low
    }
}

/**
 * @brief Disable chip select (drive CS high).
 * @param p    Port index (A, B, C, D, E, H from port_t enum)
 * @param pin  Pin number (0..15)
 * @pre GPIO pin must be configured as output push-pull.
 */
void spi_csDisable(port_t p, uint8_t pin)
{
    if (p < GPIOs_SIZE)
    {
        gpio[p]->BSRR = (1 << pin);          // Set BSx bit -> output high
    }
}