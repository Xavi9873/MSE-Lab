#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stdint.h>
#include "stm32f4xx.h"

/* ── Error codes ─────────────────────────────────────────────────── */
#define ADC_OK            ( 0)
#define ADC_ERR_INVALID   (-1)

/* ── ADC instance enumeration ────────────────────────────────────── */
typedef enum {
    ADC_1 = 0,
    ADC_2 = 1,
    ADC_3 = 2,
    ADC_INSTANCES_SIZE = 3
} adc_instance_t;

/* ── Sample time options (STM32F4 encoding, CR register bits) ─────── */
typedef enum {
    ADC_SAMPLETIME_3   = 0,
    ADC_SAMPLETIME_15  = 1,
    ADC_SAMPLETIME_28  = 2,
    ADC_SAMPLETIME_56  = 3,
    ADC_SAMPLETIME_84  = 4,   /* Recommended for potentiometer */
    ADC_SAMPLETIME_112 = 5,
    ADC_SAMPLETIME_144 = 6,
    ADC_SAMPLETIME_480 = 7
} adc_sampletime_t;

/* ── Injected channel enumeration (1–4, matches JDRx registers) ──── */
typedef enum {
    ADC_INJECTED_CH1 = 1,
    ADC_INJECTED_CH2 = 2,
    ADC_INJECTED_CH3 = 3,
    ADC_INJECTED_CH4 = 4
} adc_injected_rank_t;

/* Pointer array — accessible by sensor module */
extern ADC_TypeDef* ADC_inst[ADC_INSTANCES_SIZE];

/* ══════════════════════════════════════════════════════════════════
 *  PUBLIC API  (9 functions matching the SRS)
 * ══════════════════════════════════════════════════════════════════ */

/**
 * @brief FR-1: Initialize the ADC subsystem.
 *
 * Enables clocking for all ADC instances and configures each one
 * to a known default state (12-bit resolution, right alignment,
 * single conversion, software trigger, ADC powered OFF).
 *
 * @return ADC_OK always (defensive: call before any other function)
 */
int8_t adc_init(void);

/**
 * @brief FR-2: Enable (power on) a specific ADC instance.
 *
 * Sets the ADON bit in CR2. The ADC must be enabled before starting
 * any conversion.
 *
 * @param inst  ADC instance to enable (ADC_1, ADC_2, or ADC_3)
 * @return ADC_OK on success, ADC_ERR_INVALID if inst is out of range
 */
int8_t adc_enableAdc(adc_instance_t inst);

/**
 * @brief FR-3: Configure a regular ADC channel.
 *
 * Selects the channel and its sample time for the first slot of the
 * regular conversion sequence (L = 1 conversion).
 *
 * @param inst        ADC instance
 * @param channel     Channel number (0–18)
 * @param sampleTime  Sample time from adc_sampletime_t
 * @return ADC_OK on success, ADC_ERR_INVALID on bad parameters
 */
int8_t adc_setChannel(adc_instance_t inst,
                      uint8_t channel,
                      adc_sampletime_t sampleTime);

/**
 * @brief FR-4: Configure an injected ADC channel.
 *
 * Assigns a channel to a specific rank in the injected sequence
 * and sets its sample time. Injected conversions have higher
 * priority than regular ones and can interrupt them.
 *
 * @param inst        ADC instance
 * @param channel     Channel number (0–18)
 * @param rank        Injected rank (ADC_INJECTED_CH1 … ADC_INJECTED_CH4)
 * @param sampleTime  Sample time from adc_sampletime_t
 * @return ADC_OK on success, ADC_ERR_INVALID on bad parameters
 */
int8_t adc_setInjectedChannel(adc_instance_t inst,
                               uint8_t channel,
                               adc_injected_rank_t rank,
                               adc_sampletime_t sampleTime);

/**
 * @brief FR-5: Start a single regular conversion (software trigger).
 *
 * Clears the EOC flag and sets SWSTART. The caller should poll
 * the EOC flag or call adc_readData() which polls internally.
 *
 * @param inst  ADC instance
 * @return ADC_OK on success, ADC_ERR_INVALID if inst is out of range
 */
int8_t adc_startSingleConversion(adc_instance_t inst);

/**
 * @brief FR-6: Start continuous regular conversions.
 *
 * Sets CONT bit and then SWSTART. The ADC will convert repeatedly
 * until adc_stopContinuousConversion() is called or ADC is disabled.
 *
 * @param inst  ADC instance
 * @return ADC_OK on success, ADC_ERR_INVALID if inst is out of range
 */
int8_t adc_startContinuousConversion(adc_instance_t inst);

/**
 * @brief FR-7: Start an injected channel conversion (software trigger).
 *
 * Sets the JSWSTART bit in CR2. Use after adc_setInjectedChannel().
 *
 * @param inst  ADC instance
 * @return ADC_OK on success, ADC_ERR_INVALID if inst is out of range
 */
int8_t adc_startInjectedConversion(adc_instance_t inst);

/**
 * @brief FR-8: Read result of the last regular conversion.
 *
 * Polls the EOC flag (blocking) and returns the 12-bit value from DR.
 * Reading DR automatically clears the EOC flag.
 *
 * @param inst  ADC instance
 * @return 12-bit conversion result (0–4095), or 0 on invalid inst
 */
uint16_t adc_readData(adc_instance_t inst);

/**
 * @brief FR-9: Read result of a specific injected channel.
 *
 * Polls the JEOC flag (blocking) and returns the 12-bit value from
 * the JDRx register that corresponds to the requested rank.
 *
 * @param inst  ADC instance
 * @param rank  Injected rank whose result is needed
 * @return 12-bit conversion result (0–4095), or 0 on invalid params
 */
uint16_t adc_readInjectedChannelData(adc_instance_t inst,
                                      adc_injected_rank_t rank);

#endif /* ADC_DRIVER_H */