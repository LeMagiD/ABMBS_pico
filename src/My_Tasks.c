#include "My_Tasks.h"
#include "McuRTOS.h"
#include "hardware/gpio.h"
#include "stdio.h"  // for printf()
#include "McuWait.h"
#include "McuSystemView.h"
#include "McuGPIO.h"
#include "leds.h"
#include "buttons.h"

typedef struct
{
  LEDS_Leds_e LED;
  BTN_Buttons_e BUTTON;
  // Weitere Parameter hier
} TaskParams;


static void MainTask(void *pv){ // from lab, not relevant for quiz

    for(;;){
        vTaskDelay(pdMS_TO_TICKS(500));
    }
} 
void InitButtons(McuGPIO_Handle_t *handle, int Pin)
{
  McuGPIO_Config_t config;
  McuGPIO_GetDefaultConfig(&config);
  config.isInput = true;
  config.hw.pin = Pin;
  config.hw.pull = McuGPIO_PULL_UP;
  *handle = McuGPIO_InitGPIO(&config);
}
static void mainTask(void *pv){ // copied from Video c, SW6. Probably wont work since not all code is shown in video and not all is implemented here
   LEDS_Leds_e led = *(uint32_t*)pv;                 

    printf("mainTask led param is %d /n",(int)led);  
    /* testing task priorities*/
    //vTaskPrioritySet(NULL,uxTaskPriorityGet(NULL+1)):
    //vTaskPrioritySet(NULL,uxTaskPriorityGet(NULL-1));
    for(;;){
        McuSystemView_Print((const char*)"Blink\r\n"); 
        vTaskDelay(pdMS_TO_TICKS(500));
        McuSystemView_OnUserStart(1); /* using ID 1 */
        Leds_Neg(led);                                 
        McuSystemView_OnUserStop(1); /* using ID 1 */
    }

}


void LedTaskQuizSW5(void *pv){
unsigned int LED_PIN = (int)pv; // Red=20, Green=19, Blue=18
gpio_init(LED_PIN);
gpio_set_dir(LED_PIN, GPIO_OUT);

for(;;){
if(gpio_get(LED_PIN)) gpio_put(LED_PIN,0);
else gpio_put(LED_PIN,1);
//TickType_t prevWakeTime = xTaskGetTickCount(); 
//xTaskDelayUntil(prevWakeTime,pdMS_TO_TICKS(500));
}
}

void ButtonTask(void *pv){
  static McuGPIO_Handle_t ButtonHold;
  int PinButtonHold = 11;

  static McuGPIO_Handle_t ButtonDown;
  int PinButtonDown = 14;
  static McuGPIO_Handle_t ButtonUp;
  int PinButtonUp = 15;

  InitButtons(&ButtonHold, PinButtonHold);
  InitButtons(&ButtonDown, PinButtonDown);
  InitButtons(&ButtonUp, PinButtonUp);

  if (ButtonHold == NULL && ButtonUp == NULL && ButtonDown)
  {
    Leds_On(LEDS_BLUE);
    McuSystemView_Print((const char*)"Blue LED on\r\n"); 
    vTaskDelay(pdMS_TO_TICKS(200));

    for (;;)    // todo: fabrice fragen was das macht
    {
      
    }
  }

  for (;;)
  {
    if (McuGPIO_IsLow(ButtonHold))
    {
      Leds_On(LEDS_RED);
      Leds_On(LEDS_BLUE);
      Leds_On(LEDS_GREEN);
        McuSystemView_Print((const char*)"All LEDS on\r\n"); 
    vTaskDelay(pdMS_TO_TICKS(200));


    }

    if (McuGPIO_IsLow(ButtonUp))
    {
      Leds_On(LEDS_RED);
        McuSystemView_Print((const char*)"Red LED on\r\n"); 
    vTaskDelay(pdMS_TO_TICKS(200));


    }

    if (McuGPIO_IsLow(ButtonDown))
    {
      Leds_On(LEDS_GREEN);
        McuSystemView_Print((const char*)"Green LED on\r\n"); 
    vTaskDelay(pdMS_TO_TICKS(200));


    }
  

    Leds_Off(LEDS_RED);
    Leds_Off(LEDS_BLUE);
    Leds_Off(LEDS_GREEN);
      //  McuSystemView_Print((const char*)"All LEDS off\r\n"); 
    vTaskDelay(pdMS_TO_TICKS(200));


  }
}

void DebounceButtTask(void *pv){
Debounce_Init();
TaskParams taskparams = *(TaskParams*) pv; 
LEDS_Leds_e led = taskparams.LED;
BTN_Buttons_e butt = taskparams.BUTTON;

for(;;){

if(BTN_IsPressed(butt)){

  Leds_Neg(led);
}
}
}


void MakeLedTask(void){
if(xTaskCreate(mainTask,
"Led for Quiz",
600/sizeof(StackType_t),
(void*)NULL,
tskIDLE_PRIORITY+2,
(TaskHandle_t*)NULL)!=pdPASS){

printf("pdPASS == NULL, Task couldn't be created");
}
vTaskStartScheduler();
}

void MakeButtonsTask(void){
if(xTaskCreate(ButtonTask,
"Button Task",
600/sizeof(StackType_t),
(void*)NULL,
tskIDLE_PRIORITY+2,
(TaskHandle_t*)NULL)!=pdPASS){

printf("pdPASS == NULL, Task couldn't be created");
}
vTaskStartScheduler();
}



void MakeDebounceButtTask(void){

   TaskParams params ={
    .LED = LEDS_BLUE,
    .BUTTON = BTN_NAV_CENTER,
  } ;

BaseType_t Task1 = xTaskCreate(DebounceButtTask,
                  "Debouncing Task",
                  600/sizeof(StackType_t),
                  &params, 
                  tskIDLE_PRIORITY+1,
                  (TaskHandle_t*)NULL);

if(Task1!=pdPASS){
printf("pdPASS == NULL, Task couldn't be created");
}

vTaskStartScheduler();


}