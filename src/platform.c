#include "platform.h"
#include "McuLib.h"
#include "McuWait.h"
#include "McuUtility.h"
#include "McuGPIO.h"
#include "McuLED.h"

#include "gpio.h"
#include "leds.h"

uint32_t SystemCoreClock = 120000000;

void PL_Init(void) {
    McuLib_Init();
    McuWait_Init();
    McuGPIO_Init();
    McuLED_Init();
    Leds_Init();
    Gpio_Init();
    McuRTOS_Init();
   /* for(;;) { 
      Gpio_Test();
    }*/
}

void PL_Deinit(void) {
    McuRTOS_Deinit();
    Gpio_Deinit();
    Leds_Deinit();
    McuLED_Deinit();
    McuGPIO_Deinit();
    McuWait_Deinit();
    McuLib_Deinit();
}