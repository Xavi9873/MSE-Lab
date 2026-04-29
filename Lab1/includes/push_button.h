#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H
/**
 * @file push_button.h
 * @brief Module that controls a push button inside a STM32
 * 
 * The user may use this module to initialize a push button inside an STM32 and adds debounce logic to it
 * 
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 *
 */

#include <stdint.h>
#include "stm32f4xx.h" 
#include "GPIO.h"

#define BUTTON_PORT C
#define BUTTON_PIN 13
#define BUTTON_INPUT_MODE 0 


/**
 * @brief Initializes the user push button as an inpute inside a STM32 
 * 
 * This function initializes the push button GPIO port and pin as an input
 * 
 * @return No return values
 */
void button_init();

/**
 * @brief Gets the state of the push button  
 * 
 * This function returns HIGH or LOW depending if the button was pressed
 * 
 * @return Returns HIGH or LOW 
 */
uint8_t button_get_state();
#endif