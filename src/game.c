#include "game.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Player1
int16_t player1X = 0;
int16_t player1Y = LCD_PIXEL_HEIGHT-10;
uint16_t player1W = 60;
uint16_t player1H = 10;
uint8_t player1IsReversed = 1;


//The coins 
uint16_t coinSize = 10;

typedef struct Coin {
	  int16_t coinX;
	  int16_t coinY;
	  int16_t coinVX;
	  int16_t coinVY;
	  uint8_t isRun;

} Coin;


//Mode
uint8_t demoMode = 0;


#define MAX_COIN_NUM 10
Coin Coins[MAX_COIN_NUM];
uint16_t hitTimes = 0;
uint16_t missTimes = 0;
uint16_t player1Color = LCD_COLOR_WHITE;
/**
 * 
 */

void
initCoins(uint8_t i)
{	
		Coins[i].coinX = rand() % (LCD_PIXEL_WIDTH - coinSize);
		Coins[i].coinY = coinSize ;
		Coins[i].coinVX = 0;
		Coins[i].coinVY = (rand()%5 +1);
		Coins[i].isRun = 1;
}

void updateCoin(uint8_t coinID){
		if (Coins[coinID].isRun)
		{
			LCD_SetTextColor( LCD_COLOR_BLACK );
			LCD_DrawFullRect(Coins[coinID].coinX, Coins[coinID].coinY, coinSize ,coinSize);
			Coins[coinID].coinY += Coins[coinID].coinVY; 

				if (Coins[coinID].coinX >= player1X && Coins[coinID].coinX <= player1X + player1W &&
					Coins[coinID].coinY >= player1Y && Coins[coinID].coinY <= player1Y + player1H)
				{	
					player1Color -= 0x007F;
					if(player1Color < 0) {
						player1Color = LCD_COLOR_WHITE;
					}
					hitTimes++;
					Coins[coinID].isRun = 0;
					initCoins(coinID);
				}
				if (Coins[coinID].coinY>LCD_PIXEL_HEIGHT-10)
				{	
					missTimes++;
					Coins[coinID].isRun = 0;
					initCoins(coinID);
				}
		} else { //isRun
			//missTimes++;
			initCoins(coinID);
		} 
}



void
GAME_EventHandler1()
{
	if( STM_EVAL_PBGetState( BUTTON_USER ) ){

		player1IsReversed = 0;

		while( STM_EVAL_PBGetState( BUTTON_USER ) );

		player1IsReversed = 1;
	}
}

void
GAME_EventHandler2()
{
}

void
GAME_EventHandler3()
{
}

void
GAME_Update()
{
	//Player1
	LCD_SetTextColor( LCD_COLOR_BLACK );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	if( player1IsReversed )
				player1X -= 5;
			else
				player1X += 5;

			if( player1X <= 0 )
				player1X = 0;
			else if( player1X + player1W >= LCD_PIXEL_WIDTH )
				player1X = LCD_PIXEL_WIDTH - player1W;
			
	//The coins behavior

		for(uint8_t i=0 ;i<MAX_COIN_NUM;i++){
			updateCoin(i);
			if (i == MAX_COIN_NUM)
			{
				i = 0;
			}
		}

}

void
GAME_Render()
{
	LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawCircle( LCD_PIXEL_WIDTH/ 2, LCD_PIXEL_HEIGHT  / 2 , 50);
	
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DrawLine(  0, LCD_PIXEL_HEIGHT / 2, LCD_PIXEL_WIDTH  , LCD_DIR_HORIZONTAL );
	//Xpos,Ypos,length, direction
	//Win or lose
	uint8_t ascii = 0x30;
		if(hitTimes > 10 || missTimes > 15){
			LCD_Clear(0x0000);
			if(missTimes < 15)
				LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "You Win");
			else
				LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "You Lose");
			missTimes = hitTimes = 0;
			vTaskDelay(700);
			LCD_Clear(0x0000);
			LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "Play Again");
			vTaskDelay(500);
			LCD_Clear(0x0000);
			LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2+20, "Ready Go");
			vTaskDelay(300);
			LCD_Clear(0x0000);
		}

	//hitTimes
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayChar(LCD_PIXEL_HEIGHT/2+5,10, hitTimes+48);
	//Display Ball
	LCD_SetTextColor( LCD_COLOR_BLUE );
	int i;
	for(i=0;i<MAX_COIN_NUM;i++){
		LCD_DrawFullRect( Coins[i].coinX,Coins[i].coinY , coinSize,coinSize );
		if (i == MAX_COIN_NUM)
			{
				i = 0;
			}
	}
}

