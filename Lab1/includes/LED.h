#ifndef LED_H
#define LED_H
/**
 * @file LED.h
 * @brief Module that controls a LED inside a STM32
 * 
 * The user may use this module to initialize and control the state of a LED inside an STM32
 * 
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 *
 */

#include <stdint.h>
#include "stm32f4xx.h" 

#include "GPIO.h"

#define LED_PORT A
#define LED_PIN 5
#define LED_OUTPUT_MODE 1 

/**
 * @brief Initializes the user LED as an output inside a STM32 
 * 
 * This function initializes the LED's GPIO port and pin as an output
 * 
 * @return No return values
 */

void led_init();

/**
 * @brief Turns on the LED
 * 
 * This function sets the LED's pin on HIGH to turn it on
 * 
 * @return No return values
 */

void led_on();

/**
 * @brief Turns off the LED
 * 
 * This function sets the LED's pin on LOW to turn it off
 * 
 * @return No return values
 */

void led_off();

/**
 * @brief Toggles the LED
 * 
 * This function swithces the LED's state from HIGH to LOW or from LOW to HIGH
 * 
 * @return No return values
 */


void led_toggle();
#endif