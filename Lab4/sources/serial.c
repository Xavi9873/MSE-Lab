/**
 * @file serial.c
 * @brief High-level serial (UART) communication module
 * 
 * This module provides functions to initialize the USART2 peripheral
 * and to print formatted strings over the UART interface. It combines
 * GPIO, UART driver, and utility functions.
 *
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 */

#include "serial.h"

/**
 * @brief Initializes the serial (UART) interface
 * 
 * This function configures a complete serial communication interface.
 * It enables the GPIO port clock, sets the specified pins to alternate function mode,
 * assigns the alternate function (AF7) for both TX and RX pins, and finally
 * initializes the USART2 peripheral.
 * 
 * @param p GPIO port identifier (0 for GPIOA, 1 for GPIOB, etc.)
 * @param pin_tx Pin number to be used for USART2 TX (e.g., 2 for PA2)
 * @param pin_rx Pin number to be used for USART2 RX (e.g., 3 for PA3)
 * @param mode Pin mode to be set (should be ALTERNATE function mode, typically value 2)
 * @return No return value
 * 
 * @note This function assumes that `gpio_initPort`, `gpio_setPinMode`, 
 *       `uart_init_tx`, `uart_init_rx` and `uart_init` are already implemented.
 */
void serial_init(port_t p, uint8_t pin_tx, uint8_t pin_rx, uint8_t mode)
{
    gpio_initPort(p);

    gpio_setAlternateFunction(p, pin_tx, 7);
    gpio_setAlternateFunction(p, pin_rx, 7);

    uart_init();
}

/**
 * @brief Prints a formatted string over the UART interface
 * 
 * This function takes a format string with conversion specifiers (%s, %d, %u, %x, %c, %%)
 * and a variable list of arguments, formats them using utils_snprintf, and sends the
 * resulting string character by character using uart_write.
 * 
 * @param format Format string containing text and conversion specifiers
 * @param ...    Variable list of arguments corresponding to the format specifiers
 * @return No return value
 * 
 * @note The internal buffer size is 256 characters. Longer strings will be truncated.
 * @note This version of utils_snprintf expects a va_list as its third parameter.
 * 
 * @see utils_snprintf
 * @see uart_write
 */
void serial_printf(const char *format, ...)
{
    char buffer[256];
    va_list args;

    va_start(args, format);
    utils_snprintf(buffer, format, args);  // now the third parameter is va_list
    va_end(args);

    // For Loop to send every character to the UART 
    for (char *p = buffer; *p != '\0'; p++) 
    {
        uart_write(*p);
    }
}