#ifndef _ADC_CONF_H_
#define _ADC_CONF_H_

#include <stdint.h>
#include <math.h>

#include "clock.h"


// ADC Prescale calculation (2^n | uint n < 8 )
#define ADC_MAX_CLK KHz(200L)

#define ADC_PRESCALE_BITS ( (uint8_t) ceil( log((float)F_CPU/ADC_MAX_CLK)/log(2) ) )
/*
#if (F_CPU==MHz(8))
#define ADC_PRESCALE_BITS 6 // for 8MHz
#else
#error "ADC Presacle bits undetermined"
#endif
*/

#define ADC_PRESCALE  ( (uint8_t) pow(2,ADC_PRESCALE_BITS) )
#define ADC_F (F_CPU/ADC_PRESCALE)
#define ADC_CYCLE ( F_CPU / ADC_F ) // == ADC_PRESCALE

const static uint8_t adc_channels [] = { 0, 4, 5, 6 };
#define ADC_CHANNEL_CT sizeof(adc_channels)

#endif /*_ADC_CONF_H_*/