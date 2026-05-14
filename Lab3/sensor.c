#include "sensor.h"

int8_t sensor_init(Sensor_t *sensor, adc_instance_t inst, uint8_t channel)
{
    if (sensor == 0) return ADC_ERR_INVALID;

    sensor->instance = inst;
    sensor->channel  = channel;

    adc_init();                                         /* FR-1 */
    adc_setChannel(inst, channel, ADC_SAMPLETIME_84);  /* FR-3 */
    adc_enableAdc(inst);                               /* FR-2 */

    return ADC_OK;
}

int8_t sensor_startConversion(Sensor_t *sensor)
{
    if (sensor == 0) return ADC_ERR_INVALID;
    return adc_startSingleConversion(sensor->instance); /* FR-5 */
}

uint16_t sensor_readValue(Sensor_t *sensor)
{
    if (sensor == 0) return 0U;
    return adc_readData(sensor->instance);              /* FR-8 */
}