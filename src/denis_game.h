#ifndef DENIS_GAME_H__
#define DENIS_GAME_H__

#include "buttons.h"
#include "McuDebounce.h"

void Game_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds eventkind);
void MakeGameTask(void);      
void DeleteGameTask(void);      
void GameEventTask(void *pv);


#endif