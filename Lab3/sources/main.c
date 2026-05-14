/**
 * @file main.c
 * @brief Real-time LED brightness control using potentiometer and PWM
 * 
 * This program reads a potentiometer connected to ADC channel 0 and
 * controls LED brightness via PWM on TIM2 channel 1.
 *
 * @authors David Mijares
 */

#include <stdint.h>
#include "PWM.h"
#include "Sensor.h"

// PWM Global Constants
#define PWM_PIN 1           // Pin implemented in the main 
#define PWM_GPIOx A         // PWM's GPIO that will be implemented in the main
#define TIMx TIM_2          // TIM that will be implemented in the main
#define CHANNELx channel_2  // Channel that will be implemented in the main for the TIMx
#define FREQUENCY 1000      // Frequency = 1kHz

// ADC Global Constants
#define ADC_CHANNEL 0           // ADC channel for potentiometer (PA0)
#define ADC_GPIOx A         // PWM's GPIO that will be implemented in the main
#define ADC_PIN     0
#define ANALOG_MODE 3
#define ADC_MAX_VALUE 4095      // Maximum ADC value for 12-bit resolution
#define ADC_TO_PERCENT(adc) ((adc * 100) / ADC_MAX_VALUE)  // Macro for conversion

// Conversion Mode Selection
#define CONTINUOUS_CONVERSION_MODE 1  // Use continuous mode for real-time control


/**
 * @brief Main function - entry point
 * 
 * Continuously reads potentiometer value and updates LED brightness in real-time
 */
int main(void)
{ 
    uint32_t adc_value;
    uint8_t duty_cycle;
    
    pwm_init(PWM_GPIOx, TIMx, PWM_PIN);
    pwm_setSignal(TIMx, CHANNELx, FREQUENCY, 0);  // Start with LED off
    
    // Initialize sensor (ADC) on the specified channel
    sensor_init(ADC_CHANNEL, ADC_GPIOx, ADC_PIN, ANALOG_MODE);
    
    // Start continuous conversions for real-time control
    sensor_startConversion(ADC_CHANNEL, CONTINUOUS_CONVERSION_MODE);
    
    // Start PWM timer (starts with whatever duty cycle is configured)
    pwm_start(TIMx, CHANNELx);
    
    // Infinite loop - real-time control
    while (1)
    {
        // Read the current potentiometer value (0-4095)
        adc_value = sensor_readValue();
        
        // Convert ADC value to duty cycle percentage (0-100)
        duty_cycle = (adc_value * 100) / ADC_MAX_VALUE;
        
        // Update PWM signal with new duty cycle
        pwm_setSignal(TIMx, CHANNELx, FREQUENCY, duty_cycle);

        // Small delay to prevent overwhelming the system
        for(volatile int i = 0; i < 100; i++) { __NOP(); }
    }
    
    return 0;  // Never reached
}
