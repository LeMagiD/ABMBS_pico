#include "Blinker.h"
#include "platform.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "McuWait.h"

const unsigned int LED_PIN = PICO_DEFAULT_LED_PIN; /* GPIO 25 */

void BoardLedInit(void){
gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);
}

void LetItBlink(int OnTimeInMs, int OffTimeInMs){
BoardLedInit();

gpio_put(LED_PIN, 1); 
McuWait_Waitms(OnTimeInMs);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(OffTimeInMs);
}

void SoSLed(void){

BoardLedInit();

for(;;){
//SOS
//S
gpio_put(LED_PIN, 1); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 1); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 1); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
//O
gpio_put(LED_PIN, 1); 
McuWait_Waitms(600);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 1); 
McuWait_Waitms(600);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 1); 
McuWait_Waitms(600);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
//S
gpio_put(LED_PIN, 1); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 1); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 1); 
McuWait_Waitms(200);
gpio_put(LED_PIN, 0); 
McuWait_Waitms(500);
}
}