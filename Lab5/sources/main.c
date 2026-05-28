/**
 * @file main.c
 * @brief Real-time potentiometer voltage readings by UART communication protocol
 * 
 * This program reads a potentiometer connected to ADC channel 0 and
 * sends the voltage readings to the computer through a UART communication protocol
 * to visualize the output in the serial monitor
 *
 * @authors David Mijares, Ximena Cedillo, Xavier Clemente
 */

#include <stdint.h>
#include "MPU6050.h"
#include "serial.h"
#include "Timer.h"

// Serial Global Constants
#define SERIAL_GPIOx A                                      // GPIO port fot USART2
#define PIN_TX 2                                            // Tx pin number
#define PIN_RX 3                                            // Rx pin number
#define ALTERNATE_FUNCTION_MODE 2                           // ALternate function mode value

// MPU6050 Global Constants
#define I2C_GPIOx B                                         // GPIOB for I2C1
#define PIN_SCL 8                                           // PB8 -> I2C1_SCL
#define PIN_SDA 9                                           // PB9 -> I2C1_SDA
#define I2C_ALTERNATE_FUNCTION_MODE 2                       // Alternate Function mode



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

    MPU6050_t sensor; 

    // Serial 
    serial_init(SERIAL_GPIOx, PIN_TX, PIN_RX, ALTERNATE_FUNCTION_MODE); 
  
    mpu6050_init(I2C_GPIOx, PIN_SCL, PIN_SDA, I2C_ALTERNATE_FUNCTION_MODE);

    timer_delay_ms(TIMx, 100);

    // Timer
    timer_init(TIMx); // Initialize Timer



    // Infinite loop - real-time control
    while (1)
    {
        serial_printf("Inicio\r\n");
        // Read MPU6050 data
        mpu6050_readData(&sensor);
        // Send data through UART
        serial_printf("AX:%d AY:%d AZ:%d | GX:%d GY:%d GZ:%d\r\n", sensor.ax, sensor.ay, sensor.az, sensor.gx, sensor.gy, sensor.gz);
        timer_delay_ms(TIMx, DELAY_500_MS); // Delay for 500ms
    }
    
    return 0;  // Never reached
}
