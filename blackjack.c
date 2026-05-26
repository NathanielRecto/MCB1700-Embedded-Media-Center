#include <LPC17xx.H>
#include <stdio.h>
#include <stdlib.h>
#include "KBD.h"
#include "GLCD.h"
#include "LED.h"

#define MAX_CARDS  5   // max cards per hand
#define ADDRESS(x) (*((volatile unsigned long *)(x)))
#define BitBand(x, y) ADDRESS (((unsigned long)(x) & 0xF0000000) | 0x02000000 \
															|(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
																
#define P1_28 (*((volatile unsigned long *)0x233806F0))															

/* Helper to draw one card value (1..13) as text */
void card_name(int value, char *buf) {
  switch (value) {
    case 1:  sprintf(buf, "A");  break;
    case 11: sprintf(buf, "J");  break;
    case 12: sprintf(buf, "Q");  break;
    case 13: sprintf(buf, "K");  break;
    default: sprintf(buf, "%d", value); break; // 2..10
  }
}

/* Card value for scoring (A = 11, J Q K = 10) */
int card_value(int value) {
  if (value == 1)  return 11;     // Ace as 11 first
  if (value >= 11) return 10;     // J Q K
  return value;                   // 2..10
}

/* Compute hand total with Ace adjustment */
int hand_total(int *cards, int count) {
  int i;
  int total = 0;
  int aces = 0;

  for (i = 0; i < count; i++) {
    int v = cards[i];
    if (v == 1) aces++;
    total += card_value(v);
  }

  while (total > 21 && aces > 0) {
    total -= 10;   // count one Ace as 1 instead of 11
    aces--;
  }

  return total;
}

/* Draw current state: player cards, dealer cards, totals */
void blackjack_draw_state(int *p_cards, int p_count,
                          int *d_cards, int d_count,
                          int dealer_hidden, int round_over) {
  int i;
  char buf[32];
  char cardbuf[4];
  int player_total = hand_total(p_cards, p_count);
  int dealer_total = hand_total(d_cards, d_count);

  GLCD_Clear(White);
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);

  // Title
  GLCD_DisplayString(0, 0, 1, "     Blackjack      ");
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  // Player line: "Player: 18"
  sprintf(buf, "Player: %d", player_total);
  GLCD_DisplayString(2, 0, 1, (unsigned char *)buf);

  // Player cards line: "[A] [7]"
  buf[0] = 0;
  for (i = 0; i < p_count; i++) {
    card_name(p_cards[i], cardbuf);   // "A", "10", "K" etc

    if (i == 0)
      sprintf(buf, "[%s]", cardbuf);
    else
      sprintf(buf, "%s [%s]", buf, cardbuf);
  }
  GLCD_DisplayString(3, 0, 1, (unsigned char *)buf);

  // Dealer line
  if (dealer_hidden && !round_over) {
    // dealer hidden
    GLCD_DisplayString(5, 0, 1, "Dealer: ?");
  } else {
    sprintf(buf, "Dealer: %d", dealer_total);
    GLCD_DisplayString(5, 0, 1, (unsigned char *)buf);
  }

  // Dealer cards line
  buf[0] = 0;
  if (dealer_hidden && !round_over) {
    if (d_count >= 1) {
      card_name(d_cards[0], cardbuf);
      sprintf(buf, "[%s] [?]", cardbuf);
    } else {
      sprintf(buf, "[?]");
    }
  } else {
    for (i = 0; i < d_count; i++) {
      card_name(d_cards[i], cardbuf);
      if (i == 0)
        sprintf(buf, "[%s]", cardbuf);
      else
        sprintf(buf, "%s [%s]", buf, cardbuf);
    }
  }
  GLCD_DisplayString(6, 0, 1, (unsigned char *)buf);

  // Controls hint at bottom
  GLCD_DisplayString(9, 0, 1, "UP=Hit  DOWN=Stand");
}

void blackjack_show_outcome_banner(int player_total, int dealer_total,
                                   int bust_player, int bust_dealer) {
  char buf[32];
	//GLCD_Clear(White);
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);

  if (bust_player) {
    sprintf(buf, "    Player busts    ");
  } else if (bust_dealer) {
    sprintf(buf, "    Dealer busts    ");
  } else if (player_total > dealer_total) {
    sprintf(buf, "    Player wins     ");
  } else if (dealer_total > player_total) {
    sprintf(buf, "    Dealer wins     ");
  } else {
    sprintf(buf, "    Push (tie)      ");
  }

  // Row 0 shows the outcome now
  GLCD_DisplayString(0, 0, 1, (unsigned char *)buf);

  // Restore normal colours for rest of screen
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
}

// outcome: 0 = dealer wins, 1 = player wins, 2 = push
void blackjack_led_effect(int outcome) {
		int i;
		int k;
    // simple pattern reused from conditional / barrel-shift style loops
    // but now we drive an LED via bit-band alias
    switch (outcome) {
    case 1: // Player wins ? blink a few times
        for (k = 0; k < 3; k++) {
            P1_28 = 1;
            for (i = 0; i < 2000000; i++);
            P1_28 = 0;
            for (i = 0; i < 2000000; i++);
        }
        break;

    case 0: // Dealer wins ? slow single blink
        P1_28 = 1;
        for (i = 0; i < 4000000; i++);
        P1_28 = 0;
        break;

    case 2: // Push ? quick double blink
        for (k = 0; k < 2; k++) {
            P1_28 = 1;
            for (i = 0; i < 1000000; i++);
            P1_28 = 0;
            for (i = 0; i < 1000000; i++);
        }
        break;
    }
}

/* One Blackjack session. Returns 0 to go back to Game Menu. */
int blackjack(void) {
  int done = 0;
  int p_cards[MAX_CARDS];
  int d_cards[MAX_CARDS];
  int p_count, d_count;
  int player_turn;
  int player_bust, dealer_bust;
  int joy;
  int player_total, dealer_total;
  int gameover;
  int tryagain;   // 1 = yes, 0 = no
	int outcome;

  // simple seed if needed
  // srand(SysTick->VAL);

  while (!done) {

    // initial deal
    p_count = 0;
    d_count = 0;
    player_turn = 1;
    player_bust = 0;
    dealer_bust = 0;

    p_cards[p_count++] = (rand() % 13) + 1;
    p_cards[p_count++] = (rand() % 13) + 1;
    d_cards[d_count++] = (rand() % 13) + 1;
    d_cards[d_count++] = (rand() % 13) + 1;

    // player phase
    // player phase
		while (player_turn && !player_bust) {
			// draw once for current state
			blackjack_draw_state(p_cards, p_count, d_cards, d_count, 1, 0);

			// wait for a valid button (UP or DOWN)
			do {
				joy = get_button();
			} while (joy != KBD_UP && joy != KBD_DOWN);

			if (joy == KBD_UP) {
				// Hit
				if (p_count < MAX_CARDS) {
					p_cards[p_count++] = (rand() % 13) + 1;
				}
				player_total = hand_total(p_cards, p_count);
				if (player_total > 21) {
					player_bust = 1;
					player_turn = 0;
				}
			}
			else if (joy == KBD_DOWN) {
				// Stand
				player_turn = 0;
			}
		}

    // dealer phase
    if (!player_bust) {
      int dealer_done = 0;
      while (!dealer_done) {
        dealer_total = hand_total(d_cards, d_count);

        if (dealer_total < 17 && d_count < MAX_CARDS) {
          d_cards[d_count++] = (rand() % 13) + 1;
        } else {
          dealer_done = 1;
        }

        if (dealer_total > 21) {
          dealer_bust = 1;
          dealer_done = 1;
        }
      }
    }

    player_total = hand_total(p_cards, p_count);
		dealer_total = hand_total(d_cards, d_count);
		
		// Decide outcome code: 0 = dealer wins, 1 = player wins, 2 = push

		if (player_bust) {
				outcome = 0;
		} else if (dealer_bust) {
				outcome = 1;
		} else if (player_total > dealer_total) {
				outcome = 1;
		} else if (dealer_total > player_total) {
				outcome = 0;
		} else {
				outcome = 2;
		}

		// show final hands on same screen
		blackjack_draw_state(p_cards, p_count, d_cards, d_count, 0, 1);

		// overlay who wins on top row
		blackjack_show_outcome_banner(player_total, dealer_total, player_bust, dealer_bust);
		// play again prompt at bottom
		GLCD_DisplayString(8, 0, 1, "Play again?");
		GLCD_DisplayString(9, 0, 1, " > YES   NO         ");
		
		// trigger LED effect using Lab 2 style bit-band method
		blackjack_led_effect(outcome);
		
		gameover = 0;
		tryagain = 1;    // default YES

		while (!gameover) {
			joy = get_button();

			if (joy == KBD_LEFT) {
				// highlight YES
				GLCD_DisplayString(9, 0, 1, " > YES   NO       ");
				tryagain = 1;
			}
			else if (joy == KBD_RIGHT) {
				// highlight NO
				GLCD_DisplayString(9, 0, 1, "   YES > NO       ");
				tryagain = 0;
			}
			else if (joy == KBD_SELECT) {
				if (tryagain == 0) {
					// NO -> exit Blackjack back to Game Menu
					GLCD_Clear(White);
					done = 1;
					return 0;
				} else {
					// YES -> start a new round
					gameover = 1;
				}
			}
		}
  }

  return 0;
}

