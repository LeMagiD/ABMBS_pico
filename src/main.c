#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "platform.h"
#include "McuGPIO.h"
//#include "stdio.h"  // for printf()
//#include "Blinker.h"  // first tests with board led
#include "McuRTOS.h"
#include "My_Tasks.h"
#include "leds.h"

#include "buttons.h"
#include "debounce.h"
#include "McuButton.h"
#include "McuDebounce.h"

static McuGPIO_Handle_t myPin;  //Device Handle Variable



void main(void) {
PL_Init();
// gpio_set_irq_enabled_with_callback(BUTTONS_PINS_NAVCENTER_PIN,GPIO_IRQ_EDGE_FALL,true, &gpio_IsrCallback);

// MakeButtonsTask();
//MakeLedTask();
//LedTaskQuizSW5(19); // Pin 18, 19 or 20 for differt color leds
/* Lab SW4

McuGPIO_Config_t config;    //Konfigurationsvariable erstellen
McuGPIO_GetDefaultConfig(&config);  // Standard Konfigurationen für Konfigurationsvariable holen (überschreibbar)
config.isInput = true;
config.hw.pin = 11;         //Joystick mitte
config.hw.pull = McuGPIO_PULL_UP;

myPin = McuGPIO_InitGPIO(&config);  //Device Handle erstellen

if (myPin == NULL){
    printf("myPin is NULL");        //Vorsichtig mit printf umgehen, ist sehr grosse Funktionen
    return;
    }
if (McuGPIO_IsLow(myPin)){
    //do Something 
}
*/

for(;;) {


}



return;
} 