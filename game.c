#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 extern int snake (void);
 extern int blackjack (void);
 
 void game_menu(){ 
	GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
	GLCD_DisplayString (0, 0, 1, "     Game Menu       ");
	GLCD_DisplayString (1, 0, 1, "-------------------------");
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
 }
 
int start_game (void) 
{  	/* Main Program                       */
	 //selector to see which program is user the choosing
	
	int selector = 1;
	 					
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	KBD_Init();
  LED_Init ();
  GLCD_Clear(White);
	game_menu();

  SysTick_Config(SystemCoreClock/100); 


  while(1)		//loop forever
	{
			joystick_val = get_button();	
			
			if (joystick_val != joystick_prev)
			{
					if (joystick_val == KBD_DOWN)
					{
						if (selector >= 3){
								selector = 3;
						}
						else{
								selector +=1;
						}
					}
					else if (joystick_val == KBD_UP)
					{
						if (selector <= 1){
								selector = 1;
						}
						else{
								selector -=1;
						}
					}
				else if(joystick_val == KBD_SELECT)
				{
						if (selector == 1)
						{
							snake();
							game_menu();

						}
						else if (selector == 2)
						{
							blackjack();
							game_menu();

						}
						else if (selector == 3)
						{
							GLCD_Clear(White);
							return(0);
						}
				}
				joystick_prev = joystick_val;	
			}
			
				if (selector == 1)
				{
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);	
						GLCD_DisplayString(4,0,1, "  > Snake");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "    BlackJack");
						GLCD_DisplayString(6,0,1, "    Return to Home");
						
				}
				else if(selector == 2)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "    Snake");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "  > BlackJack");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "    Return to Home");

				}
				else if (selector == 3)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "    Snake");
						GLCD_DisplayString(5,0,1, "    BlackJack");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "  > Return to Home");
				}
			}
	}


