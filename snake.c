#include <LPC17xx.H>
#include <stdio.h>
#include <stdlib.h>
#include "KBD.h"
#include "GLCD.h"
#include "LED.h"

#define DELAY1_2N 20
#define ROW_MAX 9
#define COL_MAX 19
#define ADDRESS(x) (*((volatile unsigned long *)(x)))
#define BitBand(x, y) ADDRESS (((unsigned long)(x) & 0xF0000000) | 0x02000000 \
															|(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
#define P1_28 (*((volatile unsigned long *)0x233806F0))				

/* Global game state */
int xpos;              // "row" index (0..9)
int ypos;              // "column" index (0..19)
int size;              // snake length
int direct = 0;        // current direction
int prev_direct = 0;   // previous direction
int joystick_val = 0; 
int joystick_prev_val = KBD_RIGHT;

char str[20];
int snake_body[100][2];     // snake body coordinates [row][col]
int speed;             // movement speed (smaller = faster)
int xfood, yfood;      // food position
int collision = 0;
int border = 0;
int score = 0;
int last_dir = KBD_RIGHT;

void food(void) {
    int i;

        // BORDER OFF ? food can spawn anywhere
        xfood = rand() % (ROW_MAX + 1);   // 0..9
        yfood = rand() % (COL_MAX + 1);   // 0..19

    // Make sure food does not appear inside the snake body
    for (i = 0; i < size; i++) {
        if (xfood == snake_body[i][0] && yfood == snake_body[i][1]) {
            food();   // re-roll entirely
            return;
        }
    }

    // Draw food
    GLCD_DisplayChar(xfood, yfood, 1, 0x81);
}

void delay (int count) {
  count <<= DELAY1_2N;
  while (count--) ;
}

void setbody(void) {
  int i;
  for (i = 0; i < size; i++) {
    switch (direct) {
      case 0: // right
        snake_body[i][0] = xpos;
        snake_body[i][1] = ypos - i;
        break;
      case 1: // left
        snake_body[i][0] = xpos;
        snake_body[i][1] = ypos + i;
        break;
      case 2: // down
        snake_body[i][0] = xpos + i;
        snake_body[i][1] = ypos;
        break;
      case 3: // up
        snake_body[i][0] = xpos - i;
        snake_body[i][1] = ypos;
        break;
    }
  }
}

void addbody(void) {
  size++;
  score++;
  if (speed != 0) {
    speed--;
  }
}

void check(void) {
  int i;

  // food collision
  if (xfood == snake_body[0][0] && yfood == snake_body[0][1]) {
    addbody();
    food();
  }

  // self collision
  for (i = 1; i < size; i++) {
    if (snake_body[0][0] == snake_body[i][0] && snake_body[0][1] == snake_body[i][1]) {
      collision = 1;
    }
  }
}

void updatebody(void) {
  int i;

  // shift body
  for (i = size; i > 0; i--) {
    if (i - 1 == 0) {
      snake_body[0][0] = xpos;
      snake_body[0][1] = ypos;
    } else {
      GLCD_DisplayChar(snake_body[i-1][0], snake_body[i-1][1], 1, ' ');
      snake_body[i-1][0] = snake_body[i-2][0];
      snake_body[i-1][1] = snake_body[i-2][1];
    }
  }

  // draw head and body with different chars depending on direction
  for (i = 1; i < size; i++) {
    GLCD_DisplayChar(snake_body[i][0], snake_body[i][1], 1, 0x82);  // body
  }

  switch (direct) {
    case 0: GLCD_DisplayChar(snake_body[0][0], snake_body[0][1], 1, 0x8B); break; // right head
    case 1: GLCD_DisplayChar(snake_body[0][0], snake_body[0][1], 1, 0x89); break; // left head
    case 2: GLCD_DisplayChar(snake_body[0][0], snake_body[0][1], 1, 0x87); break; // down head
    case 3: GLCD_DisplayChar(snake_body[0][0], snake_body[0][1], 1, 0x85); break; // up head
  }

  delay(speed);
  check();
}

void direction(int joyval) {
  int newX = xpos;
  int newY = ypos;

  if (joyval == 0 || collision) {
    return;
  }

  switch (joyval) {
    case KBD_UP:
      if (last_dir == KBD_DOWN) break;   // block reverse
      newX = xpos - 1;

      if (border) {
        if (newX < 0) {                  // would go past top
          collision = 1;
          return;
        }
      } else {
        if (newX < 0) newX = ROW_MAX;    // wrap around
      }

      xpos = newX;
      direct = 3;
      last_dir = KBD_UP;
      updatebody();                      // this will call check()
      break;

    case KBD_DOWN:
      if (last_dir == KBD_UP) break;
      newX = xpos + 1;

      if (border) {
        if (newX > ROW_MAX) {           // past bottom
          collision = 1;
          return;
        }
      } else {
        if (newX > ROW_MAX) newX = 0;
      }

      xpos = newX;
      direct = 2;
      last_dir = KBD_DOWN;
      updatebody();
      break;

    case KBD_LEFT:
      if (last_dir == KBD_RIGHT) break;
      newY = ypos - 1;

      if (border) {
        if (newY < 0) {                 // past left wall
          collision = 1;
          return;
        }
      } else {
        if (newY < 0) newY = COL_MAX;
      }

      ypos = newY;
      direct = 1;
      last_dir = KBD_LEFT;
      updatebody();
      break;

    case KBD_RIGHT:
      if (last_dir == KBD_LEFT) break;
      newY = ypos + 1;

      if (border) {
        if (newY > COL_MAX) {           // past right wall
          collision = 1;
          return;
        }
      } else {
        if (newY > COL_MAX) newY = 0;
      }

      ypos = newY;
      direct = 0;
      last_dir = KBD_RIGHT;
      updatebody();
      break;

    default:
      // ignore SELECT etc.
      break;
  }
}

void clearsnake(void) {
  int i;
  for (i = 0; i < size; i++) {
    snake_body[i][0] = 1;
    snake_body[i][1] = 1;
  }
}

// mode: 0 = border off game over, 1 = border on game over
void snake_led_effect(int mode) {
    int k;
		int i;
		int j;

    if (mode == 1) {
        // Border ON: slow strong blink a few times
        for (k = 0; k < 3; k++) {
            P1_28 = 1;
            for (i = 0; i < 3000000; i++);
            P1_28 = 0;
            for (j = 0; j < 3000000; j++);
        }
    } else {
        // Border OFF: quicker double blink
        for (k = 0; k < 2; k++) {
            P1_28 = 1;
            for (i = 0; i < 1500000; i++);
            P1_28 = 0;
            for (j = 0; j < 1500000; j++);
        }
    }
}

int snake(void) {
  int joy_difficulty, joy_try;
  int mode, modesel;
  int highscore = 0;
  int gameover, tryagain;
  int done = 0;
  char scores[54];

  // KBD_Init and LED_Init already done in start_game(),
  // but calling them again is harmless. GLCD is already initialised in main.
  // If you want, you can leave these out.
  // KBD_Init();
  // LED_Init();

  while (!done) {
    modesel = 1;
    mode = 1;
    tryagain = 1;
    direct = 0;
    prev_direct = 0;
    joystick_val = 0;
    joystick_prev_val = KBD_RIGHT;
    speed = 15;
    size = 2;
    xpos = 5;
    ypos = 10;
    score = 0;
    collision = 0;
    border = 0;

    GLCD_Clear(White);
    GLCD_SetBackColor(Blue);
    GLCD_SetTextColor(Yellow);
    GLCD_DisplayString(0, 0, 1, "Select Border mode: ");
		GLCD_DisplayString(1, 0, 1, "                    ");
		GLCD_SetBackColor(Yellow);
		GLCD_SetTextColor(Blue);
    GLCD_DisplayString(4, 0, 1, "  >  ON");
		GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);
    GLCD_DisplayString(5, 0, 1, "     OFF");
		GLCD_DisplayString(8, 0, 1, "Border OFF -> press");
    GLCD_DisplayString(9, 0, 1, "Select to exit game ");

    // choose border on/off
    while (modesel == 1) {
      joy_difficulty = get_button();
      switch (joy_difficulty) {
        case KBD_DOWN:
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Blue);
          GLCD_DisplayString(4, 0, 1, "     ON");
					GLCD_SetBackColor(Yellow);
					GLCD_SetTextColor(Blue);
          GLCD_DisplayString(5, 0, 1, "  >  OFF");
          mode = 2;
          break;
        case KBD_UP:
					GLCD_SetBackColor(Yellow);
					GLCD_SetTextColor(Blue);
          GLCD_DisplayString(4, 0, 1, "  >  ON");
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Blue);	
          GLCD_DisplayString(5, 0, 1, "     OFF");
          mode = 1;
          break;
        case KBD_SELECT:
          if (mode == 1) border = 1;
          if (mode == 2) border = 0;
          modesel = 0;

          GLCD_Clear(White);
          GLCD_SetBackColor(White);
          GLCD_SetTextColor(Blue);
          break;
      }
    }

    setbody();
    food();

    while (collision == 0) {
      joystick_val = get_button();
			
			// for border off mode, let SELECT end the game
			if (border == 0 && joystick_val == KBD_SELECT) {
				collision = 1;
				continue;
			}
			
      direction(joystick_val);
      sprintf(str, "Score: %d", score);
      GLCD_DisplayString(0, 0, 0, (unsigned char *)str);
    }

    // game over screen
    GLCD_Clear(Red);
    GLCD_SetBackColor(Red);
    GLCD_SetTextColor(White);
    if (score >= highscore) highscore = score;
    sprintf(scores, "SCORE: %d  HIGH: %d", score, highscore);
    GLCD_DisplayString(0, 0, 0, (unsigned char *)scores);
    GLCD_DisplayString(2, 0, 1, "     GAME OVER");
    GLCD_DisplayString(7, 0, 1, "Play again?");
    GLCD_DisplayString(8, 0, 1, "   > YES");
    GLCD_DisplayString(9, 0, 1, "     NO");
		
		// run LED effect once, based on border mode
		snake_led_effect(border);

    gameover = 0;
    while (gameover == 0) {
      joy_try = get_button();
      switch (joy_try) {
        case KBD_DOWN:
          GLCD_DisplayString(8, 0, 1, "     YES");
          GLCD_DisplayString(9, 0, 1, "   > NO");
          tryagain = 0;
          break;
        case KBD_UP:
          GLCD_DisplayString(8, 0, 1, "   > YES");
          GLCD_DisplayString(9, 0, 1, "     NO");
          tryagain = 1;
          break;
        case KBD_SELECT:
          if (tryagain == 0) {
            // NO: exit game and go back to Game Menu
            done = 1;
            return 0;
          }
          if (tryagain == 1) {
            // YES: restart game
            gameover = 1;
            collision = 0;
            border = 0;
            clearsnake();
          }
          break;
      }
    }

    delay(5);
  }

  return 0;
}


