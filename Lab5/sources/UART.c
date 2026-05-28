/**
 * @file UART.c
 * @brief Module that manipulates the USART2 peripheral from a STM32
 * 
 * The user may use this module with multiple functions to:
 * Initialize the USART2 peripheral, configure baud rate, 
 * and transmit data through the UART interface
 *
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 *
 */

#include "UART.h"

/**
 * @brief Calculates the baud rate register value
 * 
 * This function computes the USART baud rate register (BRR) value
 * based on the system clock frequency and desired baud rate.
 * 
 * @param clk System clock frequency in Hz (e.g., 16,000,000 for 16MHz)
 * @param brr Desired baud rate for communication (e.g., 115200)
 * @return The value to be written to the USART BRR register
 */

uint32_t baud_rate(uint32_t clk, uint32_t baud)
{
    return ((clk + (baud / 2U)) / baud);
}

/**
 * @brief Initializes the USART2 peripheral
 * 
 * This function shall configure the USART2 peripheral for serial communication.
 * It enables the USART2 clock, sets the baud rate based on the system clock,
 * and enables the transmitter and the USART peripheral.
 * 
 * @return No return value
 */

void uart_init()
{
    // Enable USART2 Clock 
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 

    // Configure baud rate based on system clock
    USART2->BRR = baud_rate(SYSTEM_CLOCK, STANDARD_BRR); // System clock = 16MHz, Standard BRR = 115,200 
    
    // Enable Transmitter and peripheral 
    USART2->CR1 |= (1U << 3);   // Enable Transmitter (TE bit)
    USART2->CR1 |= (1U << 2);   // Enable Recieven (RE bit)
    USART2->CR1 |= (1U << 13);  // Enable USART (UE bit)
}

/**
 * @brief Transmits a single character via UART
 * 
 * This function shall send one byte of data through the USART2 transmitter.
 * It waits for the transmit data register to be empty before writing the new data,
 * ensuring no data is lost during transmission.
 * 
 * @param c The character to be transmitted
 * @return No return value
 */

void uart_write(char c)
{
    // Wait until the Transmit Data Register is empty
    while(!(USART2->SR & USART_SR_TXE)); 

    // Write the 8 bit character to the data register for transmission
    USART2->DR = c; 
}
