/**
 * @file LED.c
 * @brief Module that controls a LED inside a STM32
 * 
 * The user may use this module to initialize and control the state of a LED inside an STM32
 * 
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 */
#include "LED.h"

/**
 * @brief Initializes the user LED as an output inside a STM32 
 * 
 * This function initializes the LED's GPIO port and pin as an output
 * 
 * @return No return values
 */

void led_init()
{
    gpio_initPort(LED_PORT);                                // THe LED's port is initialized 
    gpio_setPinMode(LED_PORT, LED_PIN, LED_OUTPUT_MODE);    // The LED is configured as an output pin 
    led_on();                                               // THe LED is turned on 
}

/**
 * @brief Turns on the LED
 * 
 * This function sets the LED's pin on HIGH to turn it on
 * 
 * @return No return values
 */

void led_on()
{
    gpio_setPin(LED_PORT, LED_PIN);                         // The LED is set to HIGH
}

/**
 * @brief Turns off the LED
 * 
 * This function sets the LED's pin on LOW to turn it off
 * 
 * @return No return values
 */

void led_off()
{
    gpio_clearPin(LED_PORT, LED_PIN);                       // The LED is set to LOW 
}

/**
 * @brief Toggles the LED
 * 
 * This function swithces the LED's state from HIGH to LOW or from LOW to HIGH
 * 
 * @return No return values
 */

void led_toggle()                                              
{
    gpio_togglePin(LED_PORT, LED_PIN);                      // Se cambia el LED del valor en el que este
}
