#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "platform.h"
#include "McuGPIO.h"
#include "stdio.h" // for printf()
// #include "Blinker.h"  // first tests with board led
#include "McuRTOS.h"
#include "My_Tasks.h"
#include "leds.h"
#include "game.h"
#include "McuWait.h"

#include "buttons.h"
#include "debounce.h"
#include "McuButton.h"
#include "McuDebounce.h"
#include "sensor.h"
#include "sensor2.h"

static McuGPIO_Handle_t myPin; // Device Handle Variable
void IndicateStart(void)
{
    Leds_Neg(LEDS_BLUE);
    McuWait_Waitms(50);
    Leds_Neg(LEDS_BLUE);
    Leds_Neg(LEDS_RED);
    McuWait_Waitms(50);
    Leds_Neg(LEDS_RED);
    Leds_Neg(LEDS_GREEN);
    McuWait_Waitms(50);
    Leds_Neg(LEDS_GREEN); 
}

void main(void)
{
    PL_Init();
    IndicateStart();

    Sensor2_Init();



    vTaskStartScheduler();
    for (;;)
    {
        // never leave main
    }

    return;
}