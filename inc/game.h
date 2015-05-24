#ifndef GAME_H
#define GAME_H
#include "stm32f4xx_ltdc.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "stm32f429i_discovery_ioe.h"

void
GAME_EventHandler1();

void
GAME_EventHandler2();

void
GAME_EventHandler3();

void
GAME_Update();

void
GAME_Render();




#endif
