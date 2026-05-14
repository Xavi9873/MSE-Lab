/**
 * @file UART.h
 * @brief Header file for UART peripheral management on STM32
 * 
 * This module provides function prototypes and definitions for initializing
 * and using the USART2 peripheral for serial communication.
 * Functions include baud rate calculation, UART initialization, and data transmission.
 *
 * @author David Mijares, Ximena Cedillo, Xavier Clemente
 */

#ifndef UART_H  // If UART_H is not defined
#define UART_H  // define

#include <stdint.h>
#include "GPIO.h"
#include "stm32f4xx.h" 

#define SYSTEM_CLOCK 16000000  /**< System clock frequency in Hz (16MHz) */
#define STANDARD_BRR 115200    /**< Standard baud rate for UART communication */

/**
 * @brief Calculates the baud rate register value
 * 
 * This function computes the USART baud rate register (BRR) value
 * based on the system clock frequency and desired baud rate.
 * 
 * @param clk System clock frequency in Hz
 * @param brr Desired baud rate for communication
 * @return The value to be written to the USART BRR register
 */

uint32_t baud_rate(uint32_t clk, uint32_t brr); 

/**
 * @brief Initializes the USART2 peripheral
 * 
 * This function shall configure the USART2 peripheral for serial communication.
 * It enables the USART2 clock, sets the baud rate based on the system clock,
 * and enables the transmitter and the USART peripheral.
 * 
 * @return No return value
 */

void uart_init(); 

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

void uart_write(char c); 


#endif /* UART_H */