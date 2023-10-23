/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gpio.h"
#include "McuGPIO.h"
#include "leds.h"

static McuGPIO_Handle_t myGpioPin; /* pin handle */

void Gpio_Test(void) {
  static int lowCounter = 0;

  if (McuGPIO_IsLow(myGpioPin)) { /* check if pin is low */
    lowCounter++;
    Leds_Neg(LEDS_RED);
    Leds_Neg(LEDS_GREEN);
    Leds_Neg(LEDS_BLUE);
  }
}

void Gpio_Deinit(void) {
  myGpioPin = McuGPIO_DeinitGPIO(myGpioPin);
}

void Gpio_Init(void) {
  McuGPIO_Config_t config;

  McuGPIO_GetDefaultConfig(&config);
  config.isInput = true;
  config.hw.pin = 11;
  config.hw.pull = McuGPIO_PULL_UP;
 
  myGpioPin = McuGPIO_InitGPIO(&config);
  if (myGpioPin==NULL) {
    for(;;) { /* error */ }
  }
}



