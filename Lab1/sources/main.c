/**
 * @file main.c
 * @brief An aplication code that uses a push button as an input and a LED as an output in a STM23 
 * Each time the push button is pressed the LED will toggle turning on and off
 *
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 *
 */
#include <stdint.h>
#include "push_button.h"
#include "LED.h"

int main()
{
    // 1. Initialize the button and the led
        button_init(); 
        led_init(); 
        led_off(); // When the led turns off the program is ready to execute
            
    while (1)
    {
        // 2. Check if the button was pressed
        if(button_get_state() == 1)
        {
            led_toggle(); // Change the state of the LED from HIGH to LOW and from LOW to HIGH
        }
    }
    return 0; 
}