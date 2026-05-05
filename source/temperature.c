/*
 * Copyright 2016-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    temperature.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_lpadc.h"
#include "fsl_spc.h"
#include "temperature.h"

#define ADC_CHANNEL_NUMBER   0U
#define ADC_COMMAND_ID       1U
#define ADC_TRIGGER_ID       0U
#define ADC_VREF_MV          3300U
#define ADC_12BIT_MAX        4095U
#define ADC_READ_DELAY       1000000U

/* KY-028 NTC thermistor parameters */
#define NTC_NOMINAL_RESISTANCE  10000.0f  /* 10kΩ at 25°C */
#define NTC_NOMINAL_TEMP        25.0f     /* 25°C */
#define NTC_BETA                3950.0f   /* Beta coefficient */
#define NTC_SERIES_RESISTOR     10000.0f  /* 10kΩ fixed resistor on module */
#define KELVIN_OFFSET           273.15f

volatile uint32_t g_tempInt  = 0;
volatile uint32_t g_tempFrac = 0;

void temperature_init(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    BOARD_InitPins();
    ADC_Init();
    PRINTF("Temperature Sensor (KY-028) Reader\r\n");
}

float temperature_read(void)
{
    uint32_t rawValue = ADC_ReadRaw();
    uint32_t millivolts = (rawValue * ADC_VREF_MV) / ADC_12BIT_MAX;

    float tempC = 0.0f;
    if (rawValue > 0U && rawValue < ADC_12BIT_MAX)
    {
        float resistance = NTC_SERIES_RESISTOR * (float)rawValue
                         / (float)(ADC_12BIT_MAX - rawValue);
        float steinhart = logf(resistance / NTC_NOMINAL_RESISTANCE) / NTC_BETA;
        steinhart += 1.0f / (NTC_NOMINAL_TEMP + KELVIN_OFFSET);
        tempC = (1.0f / steinhart) - KELVIN_OFFSET;
    }

    /* Update globals so send_temperature() can still use them */
    uint32_t tempInt  = (uint32_t)(tempC < 0 ? -tempC : tempC);
    uint32_t tempFrac = (uint32_t)(((tempC < 0 ? -tempC : tempC) - tempInt) * 10.0f);
    g_tempInt  = tempInt;
    g_tempFrac = tempFrac;

    return tempC;
}

static void ADC_Init(void)
{
    lpadc_config_t adcConfig;
    lpadc_conv_command_config_t cmdConfig;
    lpadc_conv_trigger_config_t trigConfig;

    /* Enable VREF analog module via SPC — required for ADC voltage reference */
    SPC_EnableActiveModeAnalogModules(SPC0, kSPC_controlVref);

    /* Enable ADC0 clock */
    CLOCK_AttachClk(kFRO_HF_to_ADC0);
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1U);
    CLOCK_EnableClock(kCLOCK_Adc0);

    /* Initialize LPADC with default config */
    LPADC_GetDefaultConfig(&adcConfig);
    adcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2; /* VDDA */
    LPADC_Init(ADC0, &adcConfig);
    LPADC_DoAutoCalibration(ADC0);

    /* Configure command 1: channel 0 side A, 12-bit, single-ended */
    LPADC_GetDefaultConvCommandConfig(&cmdConfig);
    cmdConfig.channelNumber = ADC_CHANNEL_NUMBER;
    cmdConfig.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    cmdConfig.conversionResolutionMode = kLPADC_ConversionResolutionStandard;
    LPADC_SetConvCommandConfig(ADC0, ADC_COMMAND_ID, &cmdConfig);

    /* Configure trigger 0: targets command 1, software trigger */
    LPADC_GetDefaultConvTriggerConfig(&trigConfig);
    trigConfig.targetCommandId = ADC_COMMAND_ID;
    trigConfig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(ADC0, ADC_TRIGGER_ID, &trigConfig);
}

static uint32_t ADC_ReadRaw(void)
{
    lpadc_conv_result_t result;

    LPADC_DoSoftwareTrigger(ADC0, 1U);
    while (!LPADC_GetConvResult(ADC0, &result, 0U))
    {
    }
    return result.convValue >> 3U; /* 12-bit value in upper bits */
}
