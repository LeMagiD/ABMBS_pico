/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_config.h"
#include "game.h"
#include "gameView.h"
#include "gameModel.h"
#include "sprite.h"
#include "McuUtility.h"
#include "McuGDisplaySSD1306.h"
#include "McuFontDisplay.h"
#include "McuFontHelv10Normal.h"
#include "McuFontHelv12Normal.h"
#include "McuFontHelv18Bold.h"

#include "sensor.h"
#include "i2cbus.h"
#include <stdio.h>
#if configUSE_SEGGER_SYSTEM_VIEWER_HOOKS
  #include "McuSystemView.h"
#endif
#include "boulder.h"
#include "boulder_images.h"

/*!
 * \brief Shows one line of text centered on the display, with a border around it
 * \param text0 Line one of text
 */
static void Show1Liner(const unsigned char *text0) {
  /*! \todo Make sure things are reentrant! */
  PGFONT_Callbacks font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text0, font, NULL)/2;
  y = McuGDisplaySSD1306_GetHeight()/2 - charHeight/2; /* 1 line */
  McuFontDisplay_WriteString((unsigned char*)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-1-4, McuGDisplaySSD1306_GetHeight()-1-4, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_UpdateFull();
}

/*!
 * \brief Shows two lines of text centered on the display, with a border around it
 * \param text0 Line one of text
 * \param text1 Line two of text
 */
static void Show2Liner(const unsigned char *text0, const unsigned char *text1) {
  /*! \todo Make sure things are reentrant! */
  PGFONT_Callbacks font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text0, font, NULL)/2;
  y = McuGDisplaySSD1306_GetHeight()/2 - (2*charHeight)/2; /* 2 lines */
  McuFontDisplay_WriteString((unsigned char*)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  x = McuGDisplaySSD1306_GetWidth()/2 - McuFontDisplay_GetStringWidth((unsigned char*)text1, font, NULL)/2;
  y += McuGDisplaySSD1306_GetHeight()/2 - (2*charHeight)/2;
  McuFontDisplay_WriteString((unsigned char*)text1, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth()-1, McuGDisplaySSD1306_GetHeight()-1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth()-1-4, McuGDisplaySSD1306_GetHeight()-1-4, 1, McuGDisplaySSD1306_COLOR_BLUE);

  //hier i2c bus sperren (?) 
  McuGDisplaySSD1306_UpdateFull();
  //hier i2c bus entsperren (?) 
}

static void PaintGamePlayScreen(void) {
  /*! \todo Make sure things are reentrant! */
  i2cLock();
  McuGDisplaySSD1306_Clear();
  Boulder_PaintTiles();
  i2cUnlock();
  
  int cntr = 0;
  Sprite_t **sprites;

  do {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites!=NULL) {
      Sprite_PaintList(*sprites);
    }
  } while(cntr!=0);
  McuGDisplaySSD1306_UpdateFull();
}

static void ShowGraphics() {
/* Ziel:
- Zeichne zwei kreise, einer in der Mitte des oberen linken quadranten und einen im unteren rechten - CHECK
- Zeichne eine diagonale von unten links nach oben rechts - CHECK
- Zeichne über dem Kreis unten rechts ein quadrat - CHECK
- Zeichne über dem Kreis links oben ein dreieck - CHECK
Pseudo: - DONE
- Hole breite und höhe des Screen
- Setze die Formen an die richtigen stellen 
*/

McuGDisplaySSD1306_Clear();
uint8_t xCircleOL, yCircleOL, xCircleUR, yCircleUR;
uint8_t quarterWidth, quarterHeight,circleRadius;

// Get Screen Size
quarterWidth = McuGDisplaySSD1306_GetWidth()/4; // Coordinates start from the top left corner => x positive to the right, y positive down
quarterHeight = McuGDisplaySSD1306_GetHeight()/4;
// set midpoints and radius for circles 
xCircleOL = quarterWidth;
yCircleOL = quarterHeight;
xCircleUR = 3*quarterWidth;
yCircleUR = 3*quarterHeight;
circleRadius = quarterHeight; //since height is smaller than width
McuGDisplaySSD1306_DrawCircle(xCircleOL,yCircleOL,circleRadius,McuGDisplaySSD1306_COLOR_WHITE);
McuGDisplaySSD1306_DrawCircle(xCircleUR,yCircleUR-quarterHeight/16,circleRadius,McuGDisplaySSD1306_COLOR_WHITE);  // -quarterHeight/16 because without it, one pixel is not shown

uint8_t xStart=0,yStart=McuGDisplaySSD1306_GetHeight()-1;
uint8_t xEnd = McuGDisplaySSD1306_GetWidth()-1,yEnd = 0;
McuGDisplaySSD1306_DrawLine(xStart,yStart,xEnd,yEnd,McuGDisplaySSD1306_COLOR_WHITE);

uint8_t wSidePadding = 2, hSidePadding = wSidePadding/2, xBoxMidpoint = xCircleUR, yBoxMidpoint = yCircleUR;
McuGDisplaySSD1306_DrawBox(xBoxMidpoint-quarterWidth+wSidePadding,yBoxMidpoint-quarterHeight+hSidePadding,2*quarterWidth - 2*wSidePadding, 2*quarterHeight - 2*hSidePadding,1,McuGDisplaySSD1306_COLOR_WHITE);

uint8_t xLeftCorner = xCircleOL - quarterWidth + wSidePadding, yLeftCorner = yCircleOL + quarterHeight - hSidePadding;
uint8_t xRightCorner = xCircleOL + quarterWidth - wSidePadding, yRightCorner = yCircleOL + quarterHeight - hSidePadding;
uint8_t xTopCorner = xCircleOL, yTopCorner = yCircleOL - quarterHeight + hSidePadding;
McuGDisplaySSD1306_DrawLine(xLeftCorner,yLeftCorner,xTopCorner,yTopCorner,McuGDisplaySSD1306_COLOR_WHITE);
McuGDisplaySSD1306_DrawLine(xLeftCorner,yLeftCorner,xRightCorner,yRightCorner,McuGDisplaySSD1306_COLOR_WHITE);
McuGDisplaySSD1306_DrawLine(xTopCorner,yTopCorner,xRightCorner,yRightCorner, McuGDisplaySSD1306_COLOR_WHITE);

McuGDisplaySSD1306_UpdateFull();
}


void Game_ShowScreen(Game_Screen_e screen) {
  switch(screen) {
    /*! \todo extend as needed */
    case GAME_SCREEN_DENIS:
    Show1Liner((const unsigned char*)"OLED START"); 
    break;
    case GAME_SCREEN_AEMBS:
      Show1Liner((const unsigned char*)"AEMBS");
      break;
    case GAME_SCREEN_GRAPHICS: /*! \todo */
      // Show2Liner((const unsigned char*)"Some", (const unsigned char*)"Graphics"); /*! \todo test display with graphics */
    ShowGraphics();
      break;
    case GAME_SCREEN_TEMPERATURE: 
    /*! \todo print Temperetaure in different fonts
    Pseudo: 
    - Get temperature (make sure, sensor task is running) 
    - use one liner to print on Display*/
  char str1[6];
  char str2[15] = "TEMP ";
  float temp = GetSht31Temp();

  sprintf(str1,"%.2f",temp);
  
  strcat(str2,str1);
  strcat(str2,"C");

  i2cLock();
  Show1Liner(str2);
  i2cUnlock();
      break;
    case GAME_SCREEN_GAMEPLAY:
      PaintGamePlayScreen();
      break;
    default:
      break;
  }
}

void Game_ViewInit(void) {
  /* nothing needed */
}

void Game_ViewDeinit(void) {
  /* nothing needed */
}
