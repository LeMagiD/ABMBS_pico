#ifndef GAME_H__
#define GAME_H__

#include "buttons.h"
#include "McuDebounce.h"

void Game_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds eventkind);
/* void MakeGameTask(void);      used for previous, own game task
 void DeleteGameTask(void);      got replaced my stygers game tasks below*/ 
void GameEventTask(void *pv);
void Game_Init(void);
void Game_Deinit(void );

#endif