#ifndef GPIO_H  //If GPIO_H is not defined
#define GPIO_H  // define

/**
 * @file GPIO.h
 * @brief Module that manipulates a GPIO for a STM23
 * 
 * The user may use this module with multiple functions to:
 * Initialize a STM32 GPIO port and manipulate a specific pin 
 * 
 * @author David Mijares
 */

#include <stdint.h>
#include "stm32f4xx.h" 

typedef enum port
 {
    A, 
    B, 
    C, 
    D, 
    E, 
    H, 
    size
 }port_t; 

extern GPIO_TypeDef* gpio[size];
/*
 
    Description: initializes de GPIO subsytem
    Functional Requirements: 
        FR-1: The system shall configure all GPIO ports to a default state. 
 
    Inicializar una variable para trabajar todos los GPIOs
 */

void gpio_init(); 



/*
 
    Description: Initializes a specifit GPIO port
    Functional Requirements: 
        FR-2: The system shall enable clockinf for the specified port 
 
        Inicializar un puerto (GPIO) en especifico, iniciar su reloj 

    La funcion debera recibir un numero de entrada que sera interpretado por el ENUM
    Este le indicara al array gpio que GPIO del STM32 se quiere trabajar
 */

void gpio_initPort(port_t p); 


 
/*
    Description: Configures the mode of a specific pin.
    Functional Requirements: 
        FR-3: The system shall allow configuration of pin direction and mode. 

    La funcion recibe los siguientes parametros: 

    1. Un numero que sera interpretado por el ENUM como el GPIOx que se quiere trabajar
    2. Un unsigned int que indicara que pin dentro del GPIO se quiere modificar
    3. Un unsinged int que indicara que modo se le asignara al pin del GPIOx 
 */

void gpio_setPinMode(port_t p, uint8_t pin, uint8_t mode);



/*
    Description: Sets a pin to logic high.
    Functional Requirements: 
        FR-4: The system shall drive the specified pin to a high state.

    La funcion recibe los siguientes parametros> 
    1. Un numero que sera interpretado por el ENUM como el GPIOx que se quiere trabajar
    2. Un unsigned int que indicara que pin dentro del GPIO se quiere modificar
 */

void gpio_setPin(port_t p, uint8_t pin);



/*
 
    Description: Clears a pin to logic low.
    Functional Requirements: 
        FR-5: The system shall drive the specified pin to a low state.
    
    La funcion recibe los siguientes parametros> 
    1. Un numero que sera interpretado por el ENUM como el GPIOx que se quiere trabajar
    2. Un unsigned int que indicara que pin dentro del GPIO se quiere modificar
 */

void gpio_clearPin(port_t p, uint8_t pin);



/*
 
    Description: Toggles the state of a pin.
    Functional Requirements: 
        FR-6: The system shall invert the current state of the specified pin.
    La funcion recibe los siguientes parametros> 
        1. Un numero que sera interpretado por el ENUM como el GPIOx que se quiere trabajar
        2. Un unsigned int que indicara que pin dentro del GPIO se quiere modificar
 */

void gpio_togglePin(port_t p, uint8_t pin);



/*
 
    Description: Reads the current state of a pin.
    Functional Requirements: 
        FR-7: The system shall return the digital state of the specified pin.
    La funcion recibe los siguientes parametros> 
        1. Un numero que sera interpretado por el ENUM como el GPIOx que se quiere trabajar
        2. Un unsigned int que indicara que pin dentro del GPIO se quiere modificar
 */

uint8_t gpio_readPin(port_t p, uint8_t pin);

#endif