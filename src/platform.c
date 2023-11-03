#include "platform.h"
#include "McuLib.h"
#include "McuWait.h"
#include "McuUtility.h"
#include "McuGPIO.h"
#include "McuLED.h"

#include "gpio.h"
#include "leds.h"

#include "McuRTT.h"            // for SystemView
#include "McuSystemView.h"     // for SystemView

#include "buttons.h"
#include "debounce.h"
#include "McuButton.h"
#include "McuDebounce.h"


uint32_t SystemCoreClock = 120000000;

void PL_Init(void) {
    McuLib_Init();
    McuRTT_Init();
    McuSystemView_Init();
    McuWait_Init();
    McuGPIO_Init();
    McuLED_Init();
    Leds_Init();
    Gpio_Init();
    McuRTOS_Init();
    BTN_Init();
    Debounce_Init();
    McuBtn_Init();
    McuDbnc_Init();
}

void PL_Deinit(void) {
    McuDbnc_Deinit();
    McuBtn_Deinit();
    Debounce_Deinit();
    BTN_Deinit();
    McuRTOS_Deinit();
    Gpio_Deinit();
    Leds_Deinit();
    McuLED_Deinit();
    McuGPIO_Deinit();
    McuWait_Deinit(); 
    McuSystemView_Deinit();
    McuRTT_Deinit();
    McuLib_Deinit();
}