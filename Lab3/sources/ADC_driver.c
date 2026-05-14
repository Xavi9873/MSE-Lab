/**
 * @file ADC_driver.c
 * @brief ADC bare-metal driver for STM32F4 — Lab Assignment 03
 *
 * Implements all 9 functions defined in the SRS (FR-1 through FR-9).
 * No HAL dependency — register-level access only.
 *
 * @authors  (your names here)
 */

#include "ADC_driver.h"

/* ── Private: register bit definitions ──────────────────────────── */

/* CR2 */
#define ADC_CR2_ADON       (1U << 0)   /* ADC power on            */
#define ADC_CR2_CONT       (1U << 1)   /* Continuous mode         */
#define ADC_CR2_ALIGN      (1U << 11)  /* Data alignment (0=right)*/
#define ADC_CR2_JSWSTART   (1U << 22)  /* Injected SW start       */
#define ADC_CR2_SWSTART    (1U << 30)  /* Regular SW start        */

/* CR1 */
#define ADC_CR1_RES_12BIT  (0U << 24)  /* 12-bit resolution       */
#define ADC_CR1_RES_MASK   (3U << 24)

/* SR */
#define ADC_SR_EOC         (1U << 1)   /* End of regular conv.    */
#define ADC_SR_JEOC        (1U << 2)   /* End of injected conv.   */

/* RCC APB2 */
#define RCC_APB2ENR_ADC1EN (1U << 8)
#define RCC_APB2ENR_ADC2EN (1U << 9)
#define RCC_APB2ENR_ADC3EN (1U << 10)

/* ADC common prescaler (CCR) — ADCPRE = 00 → PCLK2/2 */
#define ADC_CCR_ADCPRE_DIV2  (0U << 16)

/* ── Instance pointer array (defined here, declared extern in .h) ── */
ADC_TypeDef* ADC_inst[ADC_INSTANCES_SIZE];

/* ══════════════════════════════════════════════════════════════════
 *  Private helpers
 * ══════════════════════════════════════════════════════════════════ */

static int8_t _isValidInst(adc_instance_t inst)
{
    return (inst < ADC_INSTANCES_SIZE) ? ADC_OK : ADC_ERR_INVALID;
}

static int8_t _isValidChannel(uint8_t ch)
{
    return (ch <= 18U) ? ADC_OK : ADC_ERR_INVALID;
}

/**
 * @brief Write sample time bits into SMPR1 or SMPR2
 *
 * Channels  0– 9 → SMPR2  (3 bits each, starting at bit 0)
 * Channels 10–18 → SMPR1  (3 bits each, starting at bit 0)
 */
static void _setSampleTime(adc_instance_t inst,
                           uint8_t ch,
                           adc_sampletime_t st)
{
    if (ch <= 9U)
    {
        ADC_inst[inst]->SMPR2 &= ~(7U  << (ch * 3U));
        ADC_inst[inst]->SMPR2 |=  ((uint32_t)st << (ch * 3U));
    }
    else
    {
        uint8_t shift = (ch - 10U) * 3U;
        ADC_inst[inst]->SMPR1 &= ~(7U  << shift);
        ADC_inst[inst]->SMPR1 |=  ((uint32_t)st << shift);
    }
}

/**
 * @brief Apply default configuration to one ADC instance (no ADON yet)
 */
static void _configureDefaults(adc_instance_t inst)
{
    /* 12-bit resolution */
    ADC_inst[inst]->CR1 &= ~ADC_CR1_RES_MASK;
    ADC_inst[inst]->CR1 |=  ADC_CR1_RES_12BIT;

    /* Single conversion mode, right alignment, software trigger */
    ADC_inst[inst]->CR2 &= ~(ADC_CR2_CONT | ADC_CR2_ALIGN);

    /* Regular sequence length = 1 conversion (SQR1 L bits = 0000) */
    ADC_inst[inst]->SQR1 &= ~(0xFU << 20U);

    /* Injected sequence length = 1 (JSQR JL bits = 00) */
    ADC_inst[inst]->JSQR &= ~(3U << 20U);
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-1  adc_init
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_init(void)
{
    /* Map pointers */
    ADC_inst[ADC_1] = ADC1;
    ADC_inst[ADC_2] = ADC2;
    ADC_inst[ADC_3] = ADC3;

    /* Enable clocks for all three ADC instances */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN
                  | RCC_APB2ENR_ADC2EN
                  | RCC_APB2ENR_ADC3EN;

    /* Set ADC common prescaler: PCLK2 / 2 */
    ADC123_COMMON->CCR &= ~(3U << 16U);
    ADC123_COMMON->CCR |=  ADC_CCR_ADCPRE_DIV2;

    /* Apply defaults to every instance (all powered off) */
    _configureDefaults(ADC_1);
    _configureDefaults(ADC_2);
    _configureDefaults(ADC_3);

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-2  adc_enableAdc
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_enableAdc(adc_instance_t inst)
{
    if (_isValidInst(inst) != ADC_OK) return ADC_ERR_INVALID;

    ADC_inst[inst]->CR2 |= ADC_CR2_ADON;

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-3  adc_setChannel
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_setChannel(adc_instance_t inst,
                      uint8_t channel,
                      adc_sampletime_t sampleTime)
{
    if (_isValidInst(inst)    != ADC_OK) return ADC_ERR_INVALID;
    if (_isValidChannel(channel) != ADC_OK) return ADC_ERR_INVALID;

    /* Place channel in SQ1 (bits [4:0] of SQR3) */
    ADC_inst[inst]->SQR3 &= ~(0x1FU);
    ADC_inst[inst]->SQR3 |=  (channel & 0x1FU);

    /* Configure sample time */
    _setSampleTime(inst, channel, sampleTime);

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-4  adc_setInjectedChannel
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_setInjectedChannel(adc_instance_t inst,
                               uint8_t channel,
                               adc_injected_rank_t rank,
                               adc_sampletime_t sampleTime)
{
    if (_isValidInst(inst)       != ADC_OK) return ADC_ERR_INVALID;
    if (_isValidChannel(channel) != ADC_OK) return ADC_ERR_INVALID;
    if (rank < ADC_INJECTED_CH1 || rank > ADC_INJECTED_CH4)
        return ADC_ERR_INVALID;

    /*
     * JSQR layout (JL = 00 → 1 injected conversion):
     *   bits [9:5]  = JSQ2,  bits [4:0]  = JSQ1
     *   bits [14:10]= JSQ3,  bits [19:15]= JSQ4
     *
     * For JL=0 (1 conversion), only JSQ4 is used by hardware.
     * We write the channel into the rank's slot so the driver
     * supports up to 4 injected channels if JL is later changed.
     */
    uint8_t shift = (uint8_t)((rank - 1U) * 5U);
    ADC_inst[inst]->JSQR &= ~(0x1FU << shift);
    ADC_inst[inst]->JSQR |=  ((uint32_t)(channel & 0x1FU) << shift);

    /* Update injected sequence length to include this rank */
    uint8_t current_jl = (uint8_t)((ADC_inst[inst]->JSQR >> 20U) & 0x3U);
    if ((rank - 1U) > current_jl)
    {
        ADC_inst[inst]->JSQR &= ~(3U << 20U);
        ADC_inst[inst]->JSQR |=  ((uint32_t)(rank - 1U) << 20U);
    }

    /* Configure sample time (shared SMPR registers) */
    _setSampleTime(inst, channel, sampleTime);

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-5  adc_startSingleConversion
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_startSingleConversion(adc_instance_t inst)
{
    if (_isValidInst(inst) != ADC_OK) return ADC_ERR_INVALID;

    /* Ensure continuous mode is OFF */
    ADC_inst[inst]->CR2 &= ~ADC_CR2_CONT;

    /* Clear EOC flag before starting */
    ADC_inst[inst]->SR  &= ~ADC_SR_EOC;

    /* Trigger conversion via software */
    ADC_inst[inst]->CR2 |=  ADC_CR2_SWSTART;

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-6  adc_startContinuousConversion
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_startContinuousConversion(adc_instance_t inst)
{
    if (_isValidInst(inst) != ADC_OK) return ADC_ERR_INVALID;

    /* Enable continuous mode */
    ADC_inst[inst]->CR2 |= ADC_CR2_CONT;

    /* Clear EOC, then start */
    ADC_inst[inst]->SR  &= ~ADC_SR_EOC;
    ADC_inst[inst]->CR2 |=  ADC_CR2_SWSTART;

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-7  adc_startInjectedConversion
 * ══════════════════════════════════════════════════════════════════ */
int8_t adc_startInjectedConversion(adc_instance_t inst)
{
    if (_isValidInst(inst) != ADC_OK) return ADC_ERR_INVALID;

    /* Clear JEOC flag before starting */
    ADC_inst[inst]->SR  &= ~ADC_SR_JEOC;

    /* Trigger injected conversion via software */
    ADC_inst[inst]->CR2 |=  ADC_CR2_JSWSTART;

    return ADC_OK;
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-8  adc_readData
 * ══════════════════════════════════════════════════════════════════ */
uint16_t adc_readData(adc_instance_t inst)
{
    if (_isValidInst(inst) != ADC_OK) return 0U;

    /* Blocking poll on EOC flag */
    while (!(ADC_inst[inst]->SR & ADC_SR_EOC))
    {
        __NOP();
    }

    /* Reading DR clears EOC automatically */
    return (uint16_t)(ADC_inst[inst]->DR & 0x0FFFU);
}

/* ══════════════════════════════════════════════════════════════════
 *  FR-9  adc_readInjectedChannelData
 * ══════════════════════════════════════════════════════════════════ */
uint16_t adc_readInjectedChannelData(adc_instance_t inst,
                                      adc_injected_rank_t rank)
{
    if (_isValidInst(inst) != ADC_OK) return 0U;
    if (rank < ADC_INJECTED_CH1 || rank > ADC_INJECTED_CH4) return 0U;

    /* Blocking poll on JEOC flag */
    while (!(ADC_inst[inst]->SR & ADC_SR_JEOC))
    {
        __NOP();
    }

    /* Clear JEOC */
    ADC_inst[inst]->SR &= ~ADC_SR_JEOC;

    /* Return result from the correct JDRx register */
    switch (rank)
    {
        case ADC_INJECTED_CH1: return (uint16_t)(ADC_inst[inst]->JDR1 & 0x0FFFU);
        case ADC_INJECTED_CH2: return (uint16_t)(ADC_inst[inst]->JDR2 & 0x0FFFU);
        case ADC_INJECTED_CH3: return (uint16_t)(ADC_inst[inst]->JDR3 & 0x0FFFU);
        case ADC_INJECTED_CH4: return (uint16_t)(ADC_inst[inst]->JDR4 & 0x0FFFU);
        default:               return 0U;
    }
}