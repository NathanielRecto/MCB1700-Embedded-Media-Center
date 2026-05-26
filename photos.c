#include <LPC17xx.H> /* LPC17xx definitions */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

 int selector();
 int selected();


extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned int counter;

extern unsigned char FORTNITE_pixel_data[];
extern unsigned char DICK_pixel_data[];
extern unsigned char SONAR_pixel_data[];


int images(int x)		//function for displaying image
{
  	/* Main Program                       */
	 //selector to see which program is user the choosing
	
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	KBD_Init();
  LED_Init ();

  GLCD_Clear(White);
	
  SysTick_Config(SystemCoreClock/100); 

  while(1)		//loop forever
	{
			joystick_val = get_button();
		
				if(x == 1){
					GLCD_SetBackColor(Blue);
					GLCD_SetTextColor(Yellow);
					GLCD_DisplayString(0,0,1, "    Fortnite dude    ");
					GLCD_Bitmap(80, 40, 160, 160, FORTNITE_pixel_data);
					LED_On(x-1);

				}
				else if(x == 2){
					GLCD_SetBackColor(Blue);
					GLCD_SetTextColor(Yellow);
					GLCD_DisplayString(0,0,1, "NBA GOAT Gradey Dick!");
					GLCD_Bitmap(40, 40, 240, 160, DICK_pixel_data);

					LED_On(x-1);
				}
				else if(x == 3){
					GLCD_SetBackColor(Blue);
					GLCD_SetTextColor(Yellow);
					GLCD_DisplayString(0,0,1, "    SONAR BAT GUY    ");
				  GLCD_Bitmap(86, 40, 148, 160, SONAR_pixel_data);
					LED_On(x-1);
				}
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Blue);
				GLCD_DisplayString(9,0,1, "Push Select to exit");

			
			if (joystick_val != joystick_prev)
			{
					if (joystick_val == KBD_SELECT)
					{
							LED_Off(x-1);
							GLCD_Clear(White);
							return(0);
					}	
			}
	}
}






