/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game.h"
#include "McuRTOS.h"
#include "leds.h"

// Done as requested in Lab script sw10
#include "boulder_images.h"
#include "sprite.h"
#include "gameModel.h"
#include "gameView.h"
#include "boulder.h"

#include "i2cbus.h"

#if PL_CONFIG_USE_OLED_LCD
  #include "i2cbus.h"
  #include "McuSSD1306.h"
  #include "McuGDisplaySSD1306.h"
  #include "gameView.h"
  #include "McuFontDisplay.h"
  #include "McuGFont.h"
  #include "McuUtility.h"
  #include "McuFontCour08Normal.h"
  #include "McuFontHelv18Bold.h"
  #include "McuFontHelv12Bold.h"
  #include "McuFontHelv08Normal.h"
  #include "McuFontHelv12Normal.h"
#endif

#define CONFIG_USE_QUEUE  (1)
/* First idea: implementing very simple and crude synchronization between the game task
 *       and the debouncing engine. It works with a global variable, and the access is implemented in a reentrant way.
 * Second and better approach: using a FreeRTOS queue to properly synchronize between debouncing events and game task.
 */

typedef struct {
#if !CONFIG_USE_QUEUE
  bool validEvent; /* used to mark if event is valid. Avoids double-reading */
#endif
  BTN_Buttons_e button; /* button of the event */
  McuDbnc_EventKinds kind; /* kind of button event */
} Game_event_t;

#if CONFIG_USE_QUEUE
  static QueueHandle_t gameEventQueue; /* queue handle */
  #define GAME_EVENT_QUEUE_LENGTH    (32) /* number of elements in queue */
  #define GAME_EVENT_QUEUE_ELEM_SIZE (sizeof(Game_event_t)) /* size of a queue item */
#else
  static /*volatile*/  Game_event_t GameEvent;
#endif

#if CONFIG_USE_QUEUE
static void Game_SendEvent(Game_event_t *event) {
  if (xQueueSendToBack(gameEventQueue, event, pdMS_TO_TICKS(50))!=pdPASS) {
    /* failed to send event after timeout */
    for(;;) {}
  }
}
#endif

void Game_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind) {
#if CONFIG_USE_QUEUE
  Game_event_t event;

  event.button = button;
  event.kind = kind;
  Game_SendEvent(&event);
#else
  taskENTER_CRITICAL(); /* task critical section: events are created by timer task callback */
  GameEvent.validEvent = true;
  GameEvent.button = button;
  GameEvent.kind = kind;
  taskEXIT_CRITICAL();
#endif
}

#if 0 && PL_CONFIG_USE_OLED_LCD
static void DrawBox(void) {
  /* how to write to the display: */
  McuGDisplaySSD1306_Clear();
  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_DrawCircle(20 /* x */, 30 /* y */, 5 /* radius */, McuGDisplaySSD1306_COLOR_WHITE);
  McuGDisplaySSD1306_UpdateFull();
}

static void DrawText(void) {
  /* how to use fonts: */
  uint8_t buf[24] = "hello";
  McuFontDisplay_PixelDim x = 0, y = 0;

  McuFontDisplay_WriteString(buf, McuGDisplaySSD1306_COLOR_WHITE, &x, &y, McuFontHelv08Normal_GetFont());
  McuGDisplaySSD1306_UpdateFull();
}

/* snippet to show temperature on display: */
void ShowTemperature(float value) {
	McuFontDisplay_PixelDim x, y, w, h;
	uint8_t buf[24];
	
	McuGDisplaySSD1306_Clear();
	McuUtility_NumFloatToStr(buf,  sizeof(buf), value, 2);
	McuUtility_chcat(buf, sizeof(buf), 'C');
	w = McuFontDisplay_GetStringWidth(buf, McuFontHelv18Bold_GetFont(), NULL);
	h = McuFontDisplay_GetStringHeight(buf, McuFontHelv18Bold_GetFont(), NULL);
	x = (McuGDisplaySSD1306_GetWidth()-w)/2;
	y = (McuGDisplaySSD1306_GetHeight()-h)/2;
	McuFontDisplay_WriteString(buf, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, McuFontHelv18Bold_GetFont());
	
	McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_WHITE);
	McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-5, McuGDisplaySSD1306_GetHeight()-5, 1, McuGDisplaySSD1306_COLOR_WHITE);
	McuGDisplaySSD1306_UpdateFull();
}
#endif

#if CONFIG_USE_QUEUE
static void gameTask(void *pv) {
  bool centerButtonPressed;
  bool centerButtonHold;
  Game_event_t event;
  BaseType_t res;

#if PL_CONFIG_USE_OLED_LCD
  i2cLock();
  McuSSD1306_Init();
  McuGDisplaySSD1306_Init();
  i2cUnlock();

#if 0
  DrawBox();
  DrawText();
  ShowTemperature(3.5f);
#endif
  // Game_ShowScreen(GAME_SCREEN_AEMBS);
  Game_ShowScreen(GAME_SCREEN_DENIS);
  vTaskDelay(pdMS_TO_TICKS(2000));
  // Game_ShowScreen(GAME_SCREEN_GRAPHICS);
  // vTaskDelay(pdMS_TO_TICKS(2000));
  for(;;){
  Game_ShowScreen(GAME_SCREEN_TEMPERATURE); /*! \todo show temperature every 1 seconds*/
  vTaskDelay(pdMS_TO_TICKS(2000));
}
  
#endif

  for(;;) {
    res = xQueueReceive(gameEventQueue, &event, portMAX_DELAY);
    centerButtonPressed = false; /* reset */
    if (res==pdTRUE) { /* message received */
      centerButtonPressed = event.button==BTN_NAV_CENTER && event.kind==MCUDBNC_EVENT_PRESSED;
    }
    if (centerButtonPressed) {
#if LEDS_CONFIG_HAS_BLUE_LED
      Leds_On(LEDS_BLUE);
#elif LEDS_CONFIG_HAS_RED_LED
      Leds_On(LEDS_RED);
#endif
      vTaskDelay(pdMS_TO_TICKS(50));
#if LEDS_CONFIG_HAS_BLUE_LED
      Leds_Off(LEDS_BLUE);
#elif LEDS_CONFIG_HAS_RED_LED
      Leds_Off(LEDS_RED);
#endif
    }
    /* Erklärung, warum dies anfängt zu blinken:
    Sobald man drückt, gilt es als pressed, also leuchtet die bleue Led (gemäss code oben) kurz auf
    bleibt man nun auf dem knopf so zählt es nicht mehr als pressed sondern hold oder long_pressed-> deswegen blinkt nur noch die grüne led und nie mehr die 
    solang gedrückt bleibt*/
    Leds_Neg(LEDS_GREEN);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
#else
static void gameTask(void *pv) {
  bool centerButtonPressed;

  for(;;) {
    taskENTER_CRITICAL();
    centerButtonPressed = GameEvent.validEvent && GameEvent.button==BTN_NAV_CENTER && GameEvent.kind==MCUDBNC_EVENT_PRESSED;
    if (GameEvent.validEvent) {
      GameEvent.validEvent = false; /* reset */
    }
    taskEXIT_CRITICAL();
    if (centerButtonPressed) {
#if LEDS_CONFIG_HAS_BLUE_LED
      Leds_On(LEDS_BLUE);
#elif LEDS_CONFIG_HAS_RED_LED
      Leds_On(LEDS_RED);
#endif
      vTaskDelay(pdMS_TO_TICKS(50));
#if LEDS_CONFIG_HAS_BLUE_LED
      Leds_Off(LEDS_BLUE);
#elif LEDS_CONFIG_HAS_RED_LED
      Leds_Off(LEDS_RED);
#endif
    }
    Leds_Neg(LEDS_GREEN);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
#endif

void Game_Deinit(void) {
#if CONFIG_USE_QUEUE
  vQueueDelete(gameEventQueue);
  gameEventQueue = NULL;
#endif
}

void Game_Init(void) {
  BaseType_t res;

  res = xTaskCreate(gameTask, "gameTask", 600/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+2, NULL);
  if (res!=pdPASS) {
    /* error! */
    for(;;) {}
  }
#if CONFIG_USE_QUEUE
  gameEventQueue = xQueueCreate(GAME_EVENT_QUEUE_LENGTH, GAME_EVENT_QUEUE_ELEM_SIZE);
  if (gameEventQueue==NULL) {
    for(;;){} /* out of memory? */
  }
  vQueueAddToRegistry(gameEventQueue, "GameEventQueue");
#endif
}
