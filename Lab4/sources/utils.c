/******************************************************************************
 * Copyright (C) 2026 by Carlos Villarreal - CETYS Universidad
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Carlos Villarreal and CETYS Universidad are not liable for any
 * misuse of this material.
 *
 *****************************************************************************/
/**
 * @file utils.c
 * @brief Utility library with helper functions.
 *
 * Utils module has helper functions to treat strings, ASCII conversions, and
 * printing utilities.
 *
 * @author David Mijares
 * @date 04/30/2026
 *
 */

/*** Includes ***/
#include "utils.h"

/*** Preprocessor Definitions ***/

/*** Type Prototypes ***/

/*** Local Variables ***/

/*** External Variables ***/

/*** Function Prototypes ***/

static uint32_t utils_printString(char *dst, char *src);
static uint32_t utils_printInt(char *dst, int32_t num, uint8_t sign, uint32_t base);

/*** Function Definitions ***/


/**
 * @brief Format and store a string into a buffer
 *
 * This function builds a formatted string based on a format specifier string
 * and a variable list of arguments, similar to the standard snprintf.
 * The resulting string is written into the provided destination buffer.
 *
 * @param dst Pointer to the destination buffer where the formatted string will be stored.
 * @param format Format string containing text and conversion specifiers (e.g., %d, %s, %x).
 * @param ... Variable arguments corresponding to the format specifiers.
 *
 * @return None.
 * 
 */

void utils_snprintf(char *dst, const char *format, va_list args)
{

    // Comienza a leer lo que hay dentro de ""
    while (*format)
    {
        // Si encuentra un simbolo %
        if (*format == '%')
        {
            format++;                                                       // Aumenta el apuntador format
            uint32_t len = 0;                                               // Variable para sacar la longitud de cada argumento variable (args) 
            switch (*format)
            {
                /* Hint: on the data type cases use va_arg(args, data_type) */
                case 's':
                    // Caso de strings

                    len = utils_printString(dst, va_arg(args, char*));         // Copia el string que hay en args y se manda a dst

                    dst += len;                                                     // avanzar el puntero los espacios que ya se llenaron en dst con el string
                    break;
                case 'd':
                    // Caso de int                    
                    len = utils_printInt(dst, va_arg(args, int), 1, 10);       // Convertir el numero int en va_arg a un string y meterlo a dst

                    dst += len;                                                      // avanzar el puntero los espacios que ya se llenaron en dst con el int
                    break;
                case 'u':

                    // Caso unsinged int
                    len = utils_printInt(dst, va_arg(args, unsigned int), 0, 10);       // Convertir el numero unsigned int en va_arg a un string y meterlo a dst

                    dst += len;                                                      // avanzar el puntero los espacios que ya se llenaron en dst con el unsigned int
                    break;
                case 'x':

                    //  Caso Hexadecimal
                    len = utils_printInt(dst, va_arg(args, unsigned int), 0, 16);        // Convertir el numero int en va_arg a un string y meterlo a dst

                    dst += len;                                                      // avanzar el puntero los espacios que ya se llenaron en dst con el int
                    break;
                case 'c':
                    // Caso char
                    *dst++ = (char)va_arg(args, int);                               // Se le mete el valor en int que hay en args y se transforma a char
                    break;
                case '%':
                    *dst++ = '%';
                    break;
                default:
                    *dst++ = '%';                                                   // Mete a dst un %
                    *dst++ = *format;                                               // Mete a dst el caracter incorrecto que se metio
                    break;
            }
        }
        else
        {
            *dst++ = *format;
        }

        format++;
    }
    *dst = '\0';   //Agregar un final del string poniendo un NULL
}


/**
 * @brief Copy a string into a destination buffer.
 *
 * This function calculates the length of the source string and copies
 * its contents into the destination buffer.
 *
 * @param dst Pointer to the destination buffer where the string will be copied.
 * @param src Pointer to the source string to copy.
 *
 * @return Length of the string copied (number of characters).
 */
static uint32_t utils_printString(char *dst, char *src)
{
    uint32_t i = 0;

    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }

    return i;
}

/**
 * @brief Convert an integer to ASCII and copy it into a destination buffer.
 *
 * This function converts an integer into its ASCII representation
 * based on the specified base (2-16) and sign option.
 * The resulting string is copied into the destination buffer.
 *
 * @param dst Pointer to the destination buffer where the ASCII string will be stored.
 * @param num Integer number to convert.
 * @param sign Interger value that indicates if data is signed or unsigned.
 * @param base Numerical base for conversion.
 *
 * @return Length of the ASCII string copied into the destination buffer.
 */
static uint32_t utils_printInt(char *dst, int32_t num, uint8_t sign, uint32_t base)
{
    uint8_t buffer[33];                                         // Buffer to hold the converted number (32 digits + null terminator)
    uint32_t length = utils_itoa(num, buffer, sign, base);      // Length of the converted string
    
    utils_memCpy(dst, buffer, length);                          // Copy the converted string to the destination buffer
    
    return length;                                              // Return the length of the converted string
}



/**
 * @brief Copy a block of memory from source to destination.
 *
 * This function copies a specified number of bytes from the source buffer
 * to the destination buffer. It assumes that the source and destination
 * buffers do not overlap.
 *
 * @param dst Pointer to the destination buffer where data will be copied.
 * @param src Pointer to the source buffer containing the data to copy.
 * @param length Number of bytes to copy from source to destination.
 *
 * @return Pointer to the destination buffer (dst).
 */
void * utils_memCpy(void *dst, void *src, size_t length)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;

    while (length--)
    {
        *d++ = *s++;
    }

    return dst;
}

/**
 * @brief Reverse the order of elements of a data set
 *
 * This function takes a pointer to a data set and its length, and reverses
 * the order of the elements in place.
 *
 * @param data Pointer to the data set to reverse.
 * @param length Number of bytes in the data set.
 *
 * @return Pointer to the reversed data set (data).
 */
void * utils_memReverse(void *data, size_t length)
{
    if (length == 0)
    {
       return data; 
    } 
    size_t end = length - 1;                                // Index of the last element
    for(size_t i = 0; i < end; i++, end--)                 // Loop until the middle of the data set
    {
        uint8_t temp = *((uint8_t *)data + i);              // Store the current element in a temporary variable
        *((uint8_t *)data + i) = *((uint8_t *)data + end);  // Move the element from the end to the current position
        *((uint8_t *)data + end) = temp;                    // Move the temp/original variable to the end position   
    }
   return data;                                             // Return the pointer to the reversed data set
}


uint32_t utils_itoa(int32_t data, uint8_t *ptr, uint8_t sign, uint8_t base) 
{
    uint32_t i = 0;                                         // Indice para recorrer el buffer donde vas escribiendo caracteres.
    uint32_t num = 0;                                       // Variable auxiliar para trabajar el número como positivo.
    uint8_t negative = 0;                                   // Flag para saber si el número es negativo.


    // Validar base
    if (base < 2 || base > 16)
    {
        return 0;
    }
    
    // Si el numero es negativo
    if (sign == 1 && (data < 0)) 
    {
        negative = 1;                                       // Prender flag de numero negativo
        num = (uint32_t)(-data);                            // Convertir a positivo el numero para poder dividir y guardarlo en num
    } 
    else 
    {
        num = (uint32_t)data;                               // En caso contrario el numero simplemente se hace unsinged int y se guarda en num
    }

    // Caso particular que num sea 0 desde el inicio
    if (num == 0)
    {
        ptr[i++] = '0';
    }

    // Mientras num sea mayor a 0 
    while (num > 0)
    {
        uint32_t residuo = num % base;                      // Obtener el residuo de num y la base del numero al que se quiere convertir

        // Si es digito decimal 
        if (residuo < 10) 
        {
            ptr[i++] = (uint8_t)(residuo + '0');            // Lo conviertes a ASCII (0–9)
        } 
        // Si es un numero Hexadecimal mayor a 10 
        else 
        {
            ptr[i++] = (uint8_t)((residuo - 10) + 'A');     // Se convierte a Hexadecimal
        }

        num /= base;                                        // Divide el numero dependiendo la base 
    }

    if (negative) 
    {
        ptr[i++] = '-';                                     // Agrega el signo negativo 
    }

    utils_memReverse(ptr, i);                               // Se pone al reves el orden del string que se hizo
    
    ptr[i] = '\0';                                          // Termina el string

    return i;                                               // Se regresa el tamano del string
}




int32_t utils_atoi(uint8_t *ptr, uint32_t digits, uint8_t sign, uint8_t base)
{
    int32_t result = 0;     // Variable con resultado final
    uint32_t i = 0;         // Index
    uint8_t negative = 0;   // Flag para saber si es negativo 

    // Revisar si la base entra en el rango
    if (base < 2 || base > 16)
    {
        return 0;
    }

    // Verificar signo
    if (sign == 1 && ptr[0] == '-')
    {
        negative = 1;
        i = 1;              // empezar después del signo
    }

    // Recorrer los dígitos
    for (; i < digits; i++)
    {
        uint8_t c = ptr[i];
        uint32_t value = 0;

        // Revisar que el caracter actual este en el rango de 0 a 9
        if (c >= '0' && c <= '9')
        {
            value = c - '0';
        }
        // Si el rango va de la A a la F
        else if (c >= 'A' && c <= 'F')
        {
            value = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f')
        {
            value = c - 'a' + 10;
        }
        // Si la letra se sale del rango se rompe el ciclo
        else
        {
            break; 
        }

        // Validar que el dígito sea válido para la base
        if (value >= base)
        {
            break;
        }

        // Construir el número
        result = result * base + value;
    }

    // Aplicar signo si es necesario
    if (negative)
    {
        result = -result;
    }

    return result;
}