#include "McuRTOS.h"
#include "denis_game.h"
#include "leds.h"


#define EVENT_QUEUE_LENGTH (32)
#define EVENT_QUEUE_ELEM_SIZE (sizeof(Game_event_t))
static QueueHandle_t eventQueue;

typedef struct Game_event
{
    BTN_Buttons_e button;
    McuDbnc_EventKinds eventKind;
} Game_event_t; // typedef erspart einem den Gebrauch von "struct" beim erstellen von instanzen

void DeleteGameTask(void)
{
    // empty on purpose 
}

void MakeGameTask(void)
{
    BaseType_t res;

    res = xTaskCreate(D_GameEventTask,
                      "Game Event",
                      500 / sizeof(StackType_t),
                      (void *)NULL,
                      tskIDLE_PRIORITY + 0,
                      (TaskHandle_t *)NULL);
    if (res != pdPASS)
    {
        /*error handling here*/
    }
    eventQueue = xQueueCreate(EVENT_QUEUE_LENGTH, EVENT_QUEUE_ELEM_SIZE);
    if (eventQueue == NULL)
    {
        for (;;)
        {
        } /*out of memory?*/
    }
    vQueueAddToRegistry(eventQueue, "eventQueue");
    vTaskStartScheduler();
}
void D_GameEventTask(void *pv)
{
    (void)pv;

    Game_event_t event;
    bool pressed, rButPressed, lButPressed, uButPressed, dButPressed, cButPressed;
    for (;;)
    {
        if (xQueueReceive(eventQueue, &event, portMAX_DELAY) != pdPASS)
        {
            // Queue is empty
        }
        else
        {
            rButPressed = event.button == BTN_NAV_RIGHT && event.eventKind == MCUDBNC_EVENT_PRESSED;
            lButPressed = event.button == BTN_NAV_LEFT && event.eventKind == MCUDBNC_EVENT_PRESSED;
            uButPressed = event.button == BTN_NAV_UP && event.eventKind == MCUDBNC_EVENT_PRESSED;
            dButPressed = event.button == BTN_NAV_DOWN && event.eventKind == MCUDBNC_EVENT_PRESSED;
            cButPressed = event.button == BTN_NAV_CENTER && event.eventKind == MCUDBNC_EVENT_PRESSED;
        }
        if (cButPressed)
        {
            Leds_Neg(LEDS_GREEN);
            Leds_Neg(LEDS_BLUE);
            Leds_Neg(LEDS_RED);
        }
        else if (lButPressed || rButPressed)
        {
            Leds_Neg(LEDS_GREEN);
        }
        else if (uButPressed)
        {
            Leds_Neg(LEDS_BLUE);
        }
        else if (dButPressed)
        {
            Leds_Neg(LEDS_RED);
        }
    }
}



void D_Game_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds eventkind)
{
    Game_event_t event;
    event.button = button;
    event.eventKind = eventkind;
    if (xQueueSendToBack(eventQueue, &event, pdMS_TO_TICKS(100)) != pdPASS)
    {
        for (;;)
        { /*send to back failed*/
        }
    }
}
