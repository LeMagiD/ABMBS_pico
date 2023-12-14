/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "sensor.h"
#include "McuRTOS.h"
#include "McuUtility.h"
#include "McuSHT31.h"
#include "stdio.h"
#include "i2cbus.h"

#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS
#include "McuSystemView.h"
#endif

typedef struct sht31_t
{
  float humidity;
  float temperature;
} SHT31_t;
  SHT31_t sensor;

static void sensorTask(void *pv)
{
  float t, h;
  uint8_t res;
  uint8_t buf[32];

  for (;;)
  {
    i2cLock();
    res = McuSHT31_ReadTempHum(&t, &h);
    i2cUnlock();
    sensor.temperature = t;
    sensor.humidity = h;

#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS /* example printing sensor values with SystemView */
    McuUtility_strcpy(buf, sizeof(buf), (unsigned char *)"res: ");
    McuUtility_strcatNum8u(buf, sizeof(buf), res);
    McuUtility_strcat(buf, sizeof(buf), (unsigned char *)", t: ");
    McuUtility_strcatNumFloat(buf, sizeof(buf), t, 1);
    McuUtility_strcat(buf, sizeof(buf), (unsigned char *)"C, h: ");
    McuUtility_strcatNumFloat(buf, sizeof(buf), h, 1);
    McuUtility_strcat(buf, sizeof(buf), (unsigned char *)"%");
    McuSystemView_Print((char *)buf);
#endif
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
//! \todo Test the 2 functions below with printf or show someway different 

float GetSht31Temp(void){
return sensor.temperature;
}
float GetSht31Hum(void){
return sensor.humidity;
}

void Sensor_Deinit(void)
{
  // empty
}

void Sensor_Init(void)
{
  BaseType_t res;

  res = xTaskCreate(sensorTask, "sensor", 800 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 1, NULL);
  if (res != pdPASS)
  {
    /* error! */
    for (;;)
    {
    }
  }
}
