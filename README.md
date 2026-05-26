# MCB1700-Embedded-Media-Center

An embedded Media Center built on the MCB1700 development board using the LPC1768 microcontroller and Keil µVision. The project combines a joystick-controlled menu, photo gallery, USB audio player, and two simple games into one integrated system.

## Overview

This project was developed for COE718: Embedded Systems Design. The goal was to build a complete embedded application that uses several board peripherals through one menu-driven interface.

The Media Center includes:

- Photo Gallery using GLCD bitmap rendering
- USB Audio Player using the LPC1768 USB Audio Demo
- Snake game with border modes, scoring, collision detection, and LED feedback
- Blackjack game with hit/stand controls, Ace handling, dealer logic, and LED feedback
- Joystick-based navigation across all menus

## Hardware Used

- Keil MCB1700 development board
- LPC1768 ARM Cortex-M3 microcontroller
- GLCD display
- Joystick and SELECT button
- Potentiometer
- Speaker / DAC output
- Onboard LEDs
- USB connection to host PC

## Software and Tools

- Keil µVision IDE
- C programming language
- LPC17xx CMSIS and peripheral support files
- MCB1700 GLCD, joystick, and LED driver modules
- LPC1768 USB Audio Class Demo
- GIMP / bitmap conversion tools for creating GLCD image arrays

## System Features

### Main Menu

The main menu provides access to the three major parts of the Media Center: Photo Gallery, Audio Player, and Game Section. The joystick moves the highlighted option up and down, and the SELECT button opens the chosen module.

![Main Menu](assets/main-menu.jpg)

### Photo Gallery

The photo gallery displays three images stored as C byte arrays. Each image is drawn to the GLCD using `GLCD_Bitmap()`. The image position values were chosen based on each bitmap’s width and height so the pictures appear centred on the screen.

The gallery also turns on a matching LED while an image is displayed.

![Photo Gallery](assets/photo-gallery.jpg)

### Audio Player

The audio player is based on the LPC1768 USB Audio Demo. When selected, the board connects to the PC as a USB audio speaker. Audio samples are streamed from the host PC through USB, processed through Timer0, and sent to the DAC output for speaker playback.

The potentiometer controls volume, and the LEDs act as a simple volume-level display.

![Audio Player](assets/audio-player.jpg)

### Game Section

The game menu provides access to Snake and Blackjack. Each game runs as its own module and returns to the Game Menu when the player exits.

![Game Menu](assets/game-menu.jpg)

## Snake Game

Snake is implemented on a 10 by 20 GLCD text grid. The snake body is stored in a two-dimensional array, where index 0 represents the head and the remaining entries represent body segments.

Main features:

- Joystick-controlled movement
- Border ON and Border OFF modes
- Food spawning at random free positions
- Score increase when food is eaten
- Snake growth after collecting food
- Self-collision detection
- Wall collision in border mode
- LED blink effect using bit-band aliasing

![Snake Gameplay](assets/snake-gameplay.jpg)

![Snake Game Over](assets/snake-gameover.jpg)

## Blackjack Game

Blackjack is a simple text-based card game. The player and dealer each receive cards stored as integer values from 1 to 13. The game converts these values into card labels such as A, 2–10, J, Q, and K.

Main features:

- UP joystick input to hit
- DOWN joystick input to stand
- Dealer draws until reaching at least 17
- Ace can count as 11 or 1
- Player win, dealer win, bust, and tie conditions
- Result shown on the GLCD
- LED feedback using bit-band aliasing

![Blackjack Outcome](assets/blackjack-outcome.jpg)

## System Diagrams

### Top-Level Block Diagram

![Block Diagram](diagrams/block-diagram.png)

### System Flowchart

![System Flowchart](diagrams/system-flowchart.png)

## Project Structure

```text
Project/
├── Blinky.c                 # Main Media Center menu
├── photo_gallery.c          # Photo gallery menu
├── photos.c                 # Image display logic
├── game.c                   # Game selection menu
├── snake.c                  # Snake game
├── blackjack.c              # Blackjack game
├── usbdmain.c               # USB audio player integration
├── GLCD_SPI_LPC1700.c       # GLCD driver
├── KBD.c                    # Joystick input driver
├── LED.c                    # LED driver
├── images/                  # Converted bitmap C files
└── audio/                   # USB audio demo support files
```
## How to Run

1. Open the project in **Keil µVision**.
2. Select the **LPC1768 Flash** target.
3. Build the project.
4. Connect the **MCB1700** board to the PC.
5. Flash the program to the board.
6. Use the onboard joystick to navigate the Media Center.

## Controls

| Input | Action |
|---|---|
| Joystick Up | Move menu selection up / Hit in Blackjack |
| Joystick Down | Move menu selection down / Stand in Blackjack |
| Joystick Left / Right | Choose YES or NO on the Blackjack result screen |
| SELECT | Confirm a selection or exit selected screens |

## Module-Specific Controls

| Module | Controls |
|---|---|
| Main Menu | Use Up/Down to choose Photo Gallery, Audio Player, or Game. Press SELECT to enter. |
| Photo Gallery | Use Up/Down to choose an image. Press SELECT to view or exit an image. |
| Audio Player | Use the potentiometer to adjust volume. Press SELECT to exit audio mode. |
| Snake | Use the joystick directions to move the snake. In border-off mode, press SELECT to end the round. |
| Blackjack | Press Up to Hit, Down to Stand, Left/Right to choose YES or NO, and SELECT to confirm. |
