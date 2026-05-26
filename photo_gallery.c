#include <LPC17xx.H> /* LPC17xx definitions */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "photos.h"

int selector();
int selected();

extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned int counter;

extern int images (int x);

 void photo_menu(){ 
	GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
	GLCD_DisplayString (0, 0, 1, "    Photo Gallery    ");
	GLCD_DisplayString (1, 0, 1, "-------------------------");
	 GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
	GLCD_DisplayString (8, 0, 1, "Move Stick Up/Down"); 
	GLCD_DisplayString (9, 0, 1, "Press SELECT to view");
 }

int photo(void)		//function for displaying image
{
  	/* Main Program                       */
	 //selector to see which program is user the choosing
	
	int selector = 1;
	
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	KBD_Init();
  LED_Init ();
  GLCD_Clear(White);
	photo_menu();
	GLCD_SetBackColor(Yellow);
	GLCD_SetTextColor(Blue);	
	GLCD_DisplayString(3,0,1, "  > View image 1");
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Blue);
	GLCD_DisplayString(4,0,1, "    View image 2");
	GLCD_DisplayString(5,0,1, "    View image 3");
	GLCD_DisplayString(6,0,1, "    Return to Home");
  SysTick_Config(SystemCoreClock/100); 

  while(1)		//loop forever
	{
			joystick_val = get_button();	
			
			if (joystick_val != joystick_prev)
			{
					if (joystick_val == KBD_DOWN)
					{
						if (selector >= 4){
								selector = 4;
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
							GLCD_Clear(White);
							images(1);
							photo_menu();
	
						}
						else if (selector == 2)
						{
							GLCD_Clear(White);
							images(2);
							photo_menu();

						}
						else if (selector == 3)
						{
							GLCD_Clear(White);
							images(3);
							photo_menu();
						}
						else if(selector == 4){
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
						GLCD_DisplayString(3,0,1, "  > View image 1");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "    View image 2");
						GLCD_DisplayString(5,0,1, "    View image 3");
						GLCD_DisplayString(6,0,1, "    Return to Home");
				}
				else if(selector == 2)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(3,0,1, "    View image 1");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(4,0,1, "  > View image 2");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "    View image 3");
						GLCD_DisplayString(6,0,1, "    Return to Home");
				}
				else if(selector == 3)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(3,0,1, "    View image 1");
						GLCD_DisplayString(4,0,1, "    View image 2");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(5,0,1, "  > View image 3");
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "    Return to Home");
				}
				else if(selector == 4)
				{
						GLCD_SetBackColor(White);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(3,0,1, "    View image 1");
						GLCD_DisplayString(4,0,1, "    View image 2");
						GLCD_DisplayString(5,0,1, "    View image 3");
						GLCD_SetBackColor(Yellow);
						GLCD_SetTextColor(Blue);
						GLCD_DisplayString(6,0,1, "  > Return to Home");
				}
			}
	}






