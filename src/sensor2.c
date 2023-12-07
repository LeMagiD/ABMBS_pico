#include "platform.h"
#include "sensor.h"
#include "McuRTOS.h"
#include "McuUtility.h"
#include "McuSHT31.h"
#include "stdio.h"
#include "stdint.h"
#include "i2cbus.h"

// Shortened Version of sensor.c, made by myself (Denis) 

typedef struct sht31_t
{
  float humidity;
  float temperature;
} SHT31_t;
SHT31_t sensor;

static void sensor2Task(void *pv)
{
  uint8_t res;
  uint8_t buf[32];

  for (;;)
  {
    i2cLock();
    res = McuSHT31_ReadTempHum(&sensor.temperature, &sensor.humidity);
    i2cUnlock();

    if(res!=ERR_OK){
      for(;;){
        // sensor data wrong?
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}



float GetSht31Temp(void){
return sensor.temperature;
}
float GetSht31Hum(void){
return sensor.humidity;
}

void Sensor2_Init(void)
{
  BaseType_t res;

  res = xTaskCreate(sensor2Task, "sensor2", 800 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 1, NULL);
  if (res != pdPASS)
  {
    /* error! */
    for (;;)
    {
    }
  }

}

void Sensor2_Deinit(void)
{
  // empty
}