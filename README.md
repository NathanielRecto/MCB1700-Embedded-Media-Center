# Media Center

## Project Title
Media Center

## Motivation
This project was created as part of the COE718: Embedded System Design course to demonstrate a real-time application using the MCB1700 board and uVision. The goal was to incorporate the concepts learned throughout the semester into a fully functional media center. The media center consists of three main components: a photo gallery, an audio player, and a media game. It serves as a practical application of embedded system design.

## Installation
To install and run the project, follow these steps:
1. Clone the project repository to your development environment.
2. Open the project in Keil uVision.
3. Build and compile the code.
4. Flash the compiled code to the MCB1700 board.
5. Power on the board.

## Usage
1. Main Menu: When the application is booted up, you will be presented with the main menu, which includes options for the Photo Gallery, Audio Player, and Game. Use the joystick to navigate through the menu, and press right to select an option.

   ![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/e3690e50-ad5f-4751-8dc7-ef4b0048a327)

2. Photo Gallery: In the Photo Gallery, you can view a list of images. Use the joystick to select an image and display it on the LCD. Press left to return to the gallery menu.

   ![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/ee1eca96-ad87-4292-a0c9-f876241fee21)

   ![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/c17e16b7-50f1-4e80-8ed8-cd0aec098b51)

3. Audio Player: The Audio Player allows you to stream audio from your PC to the MCB1700 board. Use the potentiometer to control the volume (counterclockwise to increase, clockwise to decrease). LEDs indicate the volume level. To return to the menu, move the joystick to the left.

   ![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/b3d12c5b-7d2e-4746-93fc-bb4f97cafc02)

4. Game (Tic-Tac-Toe): The game menu provides options to start a game or return to the main menu.

   ![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/16796305-ac3d-490c-a4e8-f3e835598c4f)

   - Game Rules: The game is played on a 3x3 grid. Two players take turns placing X and O pieces. The first player to get three pieces in a row wins. If the grid is full and no player wins, it's a draw.

   ![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/8012aa5b-7726-4a63-a5ea-0fad04887bcd)

   - Tracking Scores: Player scores are displayed on the top left and top right. The scores increase when a player wins.


## Functions
- **Photo Gallery**: The `photo_gallery()` function displays the gallery menu and handles the image display. It uses the `GLCD_Bitmap()` function to draw images on the LCD. The program calculates the offset to center the image on the screen.

- **Audio Player**: The `audio_player()` function streams audio from the PC to the board. The potentiometer adjusts the volume, and LEDs indicate the volume level. To stop audio playback, move the joystick to the left, triggering a soft reset using `NVIC_SystemReset()`.

- **Game (Tic-Tac-Toe)**: The game uses a two-dimensional array to track player moves on the 3x3 grid. It checks for win/lose/draw conditions based on the positions of X and O pieces. Player scores are tracked, and a hovering feature indicates where the player is about to place their game piece.

## Libraries
- Keil uVision (C programming language)
- GLCD_Bitmap() function for displaying images
- USB demo project for audio streaming
- LED.c for controlling LEDs
- GLCD_SPI_LPC1700.c for LCD display
- KBD.c for joystick input

## Hardware Components
- MCB1700 board
- Controllers
- Potentiometer
- KBD directional joystick
- KBD select button
- Reset button
- LCD Screen
- Speakers
- Port LEDs

![image](https://github.com/dosawaru/Multimedia-Center/assets/35234154/c08c0f0d-b0ba-4892-8c9a-137329dd64d3)

## Software Components
- Keil uVision
- C programming language

The project demonstrates the integration of hardware and software to create a functional media center. It leverages the knowledge gained throughout the course, making it a user-friendly, interactive, and fun project to explore.
