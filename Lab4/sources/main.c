/**
 * @file main.c
 * @brief Real-time potentiometer voltage readings by UART communication protocol
 * 
 * This program reads a potentiometer connected to ADC channel 0 and
 * sends the voltage readings to the computer through a UART communication protocol
 * to visualize the output in the serial monitor
 *
 * @authors David Mijares
 */

#include <stdint.h>
#include "Sensor.h"
#include "serial.h"
#include "Timer.h"

// Serial Global Constants
#define SERIAL_GPIOx A                                      // GPIO port fot USART2
#define PIN_TX 2                                            // Tx pin number
#define PIN_RX 3                                            // Rx pin number
#define ALTERNATE_FUNCTION_MODE 2                           // ALternate function mode value

// ADC Global Constants
#define ADC_CHANNEL 0                                       // ADC channel for potentiometer (PA0)
#define ADC_GPIOx A                                         // PWM's GPIO that will be implemented in the main
#define ADC_PIN     0
#define ANALOG_MODE 3
#define ADC_MAX_VALUE 4095                                  // Maximum ADC value for 12-bit resolution
#define ADC_TO_PERCENT(adc) ((adc * 100) / ADC_MAX_VALUE)   // Macro for conversion

// Conversion Mode Selection
#define CONTINUOUS_CONVERSION_MODE 1                        // Use continuous mode for real-time control

// Timer Global Constants
#define TIMx TIM_2                                          // Timer used for delay
#define DELAY_500_MS 500                                    // Delay 500ms


/**
 * @brief Main function - entry point of the program
 * 
 * This function initializes the ADC sensor, timer, and serial interface.
 * It then continuously reads the potentiometer value and prints it
 * over UART with a 500ms delay.
 * 
 * @return int Always returns 0 (the infinite loop is never exited)
 */
int main(void)
{ 
    uint32_t adc_value = 0; // Raw ADC reading (0-4095)
    uint32_t voltage = 0; 

    // Serial 
    serial_init(SERIAL_GPIOx, PIN_TX, PIN_RX, ALTERNATE_FUNCTION_MODE); 
  
    
    
    // ADC sensor
    sensor_init(ADC_CHANNEL, ADC_GPIOx, ADC_PIN, ANALOG_MODE); // Initialize sensor (ADC) on the specified channel
    sensor_startConversion(ADC_CHANNEL, CONTINUOUS_CONVERSION_MODE); // Start continuous conversions for real-time control

    // Timer
    timer_init(TIMx); // Initialize Timer

    // Infinite loop - real-time control
    while (1)
    {

        // Read the current potentiometer value (0-4095)
        adc_value = sensor_readValue();
        timer_delay_ms(TIMx, DELAY_500_MS); // Delay for 500ms
        voltage = (adc_value * 3300) / ADC_MAX_VALUE; 
        serial_printf("ADC Value: %d | Voltage: %d mV\n", adc_value, voltage); 

    }
    
    return 0;  // Never reached
}

