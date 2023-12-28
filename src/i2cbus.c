#include "i2cbus.h"
#include "platform.h"
#include "sensor.h"
#include "McuRTOS.h"
#include "McuUtility.h"
#include "McuSystemView.h"

static SemaphoreHandle_t mutex = NULL;


void i2cLock(void){
mutex = xSemaphoreCreateRecursiveMutex();
vQueueAddToRegistry(mutex,"I2C Mutex von Denis");

if(xSemaphoreTakeRecursive(mutex,pdMS_TO_TICKS(20))!=pdTRUE){
    for(;;){
        // timeout ?
    }
}
}

void i2cUnlock(void){
    /*! \todo check if the right task wants to delete the mutex*/
vSemaphoreDelete(mutex);

}