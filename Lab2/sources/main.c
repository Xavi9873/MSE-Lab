/**
 * @file main.c
 * @brief 
 *
 * @authors David Mijares
 *
 */
#include <stdint.h>

#include "Timer.h"

#include "PWM.h"


// LED global constants
#define LED_PIN 5           // User LED pin
#define LED_GPIOx A         // User LED GPIO
#define LED_OUTPUT_MODE 1 
#define LED_DELAY_MS 500        // LED Delay 500ms
#define LED_TIMx TIM_3

#define PWM_PIN 0           // Pin implemented in the main 
#define PWM_GPIOx A         // PWM's GPIO that will be implemented in the main
#define TIMx TIM_2          // TIM that will be implemented in the main
#define CHANNELx channel_1  // Channel that will be implemented in the main for the TIMx
#define FRECUENCY 1000      // Frecuency = 1kHz

void LED_init(); 
void blinking_LED(); 
void PWM_init(); 

int main()
{ 
    
    LED_init(); 

    PWM_init(); 
    pwm_start(TIMx, CHANNELx); 

    while (1)
    {
        blinking_LED(); 
    }
    return 0; 
}

void LED_init()
{
    gpio_init(); 
    gpio_initPort(LED_GPIOx);                               // Turn on LED GPIOx's RCC
    gpio_setPinMode(LED_GPIOx, LED_PIN, LED_OUTPUT_MODE);   // Set LED as output
    gpio_setPin(LED_GPIOx, LED_PIN);                        // Turn on LED

    timer_init(LED_TIMx); 
}

void blinking_LED()
{
        gpio_clearPin(LED_GPIOx, LED_PIN);  // Turn off LED
        timer_delay_ms(LED_TIMx, LED_DELAY_MS);       // Timer delay 500ms
        gpio_setPin(LED_GPIOx, LED_PIN);    // Turn on LED
        timer_delay_ms(LED_TIMx, LED_DELAY_MS);       // Timer delay 500ms    
}

void PWM_init()
{
    pwm_init(PWM_GPIOx, TIMx, PWM_PIN); 

    pwm_setSignal(TIMx, CHANNELx, FRECUENCY); 
}
