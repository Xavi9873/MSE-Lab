/**
 * @file push_button.c
 * @brief Module that controls a push button inside a STM32
 * 
 * The user may use this module to initialize a push button inside an STM32 and adds debounce logic to it
 * 
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 *
 */
#include "push_button.h"
#define DEBOUNCE_DELAY 20000

static void delay()
{
    for(volatile int i = 0; i < DEBOUNCE_DELAY; i++);
}


/**
 * @brief Initializes the user push button as an inpute inside a STM32 
 * 
 * This function initializes the push button GPIO port and pin as an input
 * 
 * @return No return values
 */

void button_init() 
{
    gpio_initPort(BUTTON_PORT);
    gpio_setPinMode(BUTTON_PORT, BUTTON_PIN, BUTTON_INPUT_MODE);
}

/**
 * @brief Gets the state of the push button  
 * 
 * This function returns HIGH or LOW depending if the button was pressed
 * 
 * @return Returns HIGH or LOW 
 */

uint8_t button_get_state() 
{
    // 1. Verifica si el botón está presionado (LOW = 0)
    if (gpio_readPin(BUTTON_PORT, BUTTON_PIN) == 0) 
    {
        // 2. Espera un tiempo para evitar rebotes
        delay();

        // 3. Si el boton se libero se regresa el 0 
        if (gpio_readPin(BUTTON_PORT, BUTTON_PIN) == 1)
        {
            return 0;
        }
            

        // 4. Mientras el boton se mantenga presionado 
        while (gpio_readPin(BUTTON_PORT, BUTTON_PIN) == 0)
        {
            delay(); // Esperar hasta que se suelte el boton
            
        }   
        
        // 5. Botón fue presionado y liberado
        return 1;
    }

    return 0;
}