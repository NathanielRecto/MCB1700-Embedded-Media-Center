#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

extern int audio_main (void);
extern int photo (void);
extern int start_game (void);


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
 void main_menu(){ 
	//GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
	GLCD_DisplayString (0, 0, 1, "COE718 Project Demo ");
	GLCD_DisplayString (1, 0, 1, "     Main Menu      ");
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Blue);
	GLCD_DisplayString (8, 0, 1, "Use Joystick Up/Down");
	GLCD_DisplayString (9, 0, 1, "  Select to Choose"); 
 }
 
int main (void) 
{  	/* Main Program                       */
	 //selector to see which program is user the choosing
										
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	int selector = 1;
	
	KBD_Init();
  LED_Init ();
  GLCD_Init();
  GLCD_Clear(White);
	main_menu();
	
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
							photo();	
						}
						else if (selector == 2)
						{
							audio_main();
						}
						else if (selector == 3)
						{
							start_game();	
						}
				}
				joystick_prev = joystick_val;	
			}
	
				if (selector == 1)
				{
						main_menu();
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);	
						GLCD_DisplayString(4,0,1, "  > Photo Gallery");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "    Audio Player");
						GLCD_DisplayString(6,0,1, "    Game");
				}
				else if(selector == 2)
				{
						main_menu();
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "    Photo Gallery");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "  > Audio Player");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "    Game");
				}
				else if (selector == 3)
				{
						main_menu();
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "    Photo Gallery");
						GLCD_DisplayString(5,0,1, "    Audio Player");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "  > Game");				
				}
			}
	}


