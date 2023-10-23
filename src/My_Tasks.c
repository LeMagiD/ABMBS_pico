#include "My_Tasks.h"
#include "McuRTOS.h"
#include "hardware/gpio.h"
#include "stdio.h"  // for printf()
#include "McuWait.h"

static void MainTask(void *pv){ // from lab, not relevant for quiz

    for(;;){
        vTaskDelay(pdMS_TO_TICKS(500));
    }
} 

void LedTaskQuizSW5(void *pv){
unsigned int LED_PIN = (int)pv; // Red=20, Green=19, Blue=18
gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);

for(;;){
if(gpio_get(LED_PIN)) gpio_put(LED_PIN,0);
else gpio_put(LED_PIN,1);
vTaskDelay(pdMS_TO_TICKS(500));
McuWait_Waitms(100);

}
}

void MakeLedTask(void){
if(xTaskCreate(LedTaskQuizSW5,
"Led for Quiz",
600/sizeof(StackType_t),
(void*)NULL,
tskIDLE_PRIORITY+2,
(TaskHandle_t*)NULL)!=pdPASS){

printf("pdPASS == NULL, Task couldn't be created");
}
}
