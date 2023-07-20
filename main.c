#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/EmojiPlayer.h"
#include "images/StartPage.h"
#include "images/WinPage.h"
#include "images/LosePage.h"
#include "images/TREE.h"
#include "images/Door.h"
#include "images/Aligator.h"

/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;


  struct object Player;
  struct object Obstacle1;
  struct object Obstacle2;
  struct object Goal;


  int start = 1;  //int to initialize start
  int beginGame = 0;
  int endGame = 0;
  Player.col = 0;
  Player.row = 0;
  //int crash = 0;
  //int count = 0;
  int timer = 0;
  char timerBlock[45];

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();
    switch (state) {
      case START:
        if (start) {
          start = 0;
          drawFullScreenImageDMA(ForestRunPage);
          drawString(50, 60, "Press Enter to Begin Game", WHITE);
        }

        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          beginGame = 1;
          //crash = 0;
          //count = 0;
          state = PLAY;
        }
        
        // state = ?
        break;
      case PLAY:
        
        if (beginGame) {
          //set initial states for objects and background
          fillScreenDMA(BLACK);
          beginGame = 0;
          

          Player.row = 10;
          Player.col = 10;

          Obstacle1.row = 0;
          Obstacle1.col = 30;
          Obstacle2.row = 100;
          Obstacle2.col = 140;

          Goal.row = 120;
          Goal.col = 200;
        
          drawImageDMA(Obstacle1.row, Obstacle1.col, ALIGATOR_WIDTH, ALIGATOR_HEIGHT, Aligator);
          drawImageDMA(Obstacle2.row, Obstacle2.col, TREE_WIDTH, TREE_HEIGHT, TREE);
          drawImageDMA(Goal.row, Goal.col, DOOR_WIDTH, DOOR_HEIGHT, Door);
          drawImageDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, EmojiPlayer);
          

        }

        timer = vBlankCounter / 60;
        snprintf(timerBlock, 20, "TIMER: %d", timer);
        drawRectDMA(150, 80, 60, 10, WHITE);
        drawString(150, 80, timerBlock, BLACK);

        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          drawRectDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, BLACK);

          if (Player.row > 10) {
            Player.row -= 1;
          }

          drawImageDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, EmojiPlayer);
        }

        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          drawRectDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, BLACK);

          if (Player.row < 140) {
            Player.row += 1;
          }
          drawImageDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, EmojiPlayer);
        }

        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          drawRectDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, BLACK);

          if (Player.col < 220) {
            Player.col += 1;
          }
          drawImageDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, EmojiPlayer);
        }
      
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          drawRectDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, BLACK);

          if (Player.col > 10) {
            Player.col -= 1;
          }
          drawImageDMA(Player.row, Player.col, EMOJIPLAYER_WIDTH, EMOJIPLAYER_HEIGHT, EmojiPlayer);
        }

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          start = 1;
          state = PLAY;
          beginGame = 1;
          vBlankCounter = 0;
        }

        if (timer > 45) {
          endGame = 1;
          state = LOSE;
        }

        if ((Player.row < Obstacle1.row + 70) && (Player.row + 15 > Obstacle1.row) && (Player.col + 15 > Obstacle1.col) && (Player.col < Obstacle1.col + 30)) {
          endGame = 1;
          state = LOSE;
        }
        if ((Player.row < Obstacle2.row + 50) && (Player.row + 15 > Obstacle2.row) && (Player.col + 15 > Obstacle2.col) && (Player.col < Obstacle2.col + 30)) {
          endGame = 1;
          state = LOSE;
        }
        if ((Player.row < Goal.row + 40) && (Player.row + 15 > Goal.row) && (Player.col + 15 > Goal.col) && (Player.col < Goal.col + 40)) {
          endGame = 1;
          state = WIN;
        }

      


        

        // state = ?
        break;
      case WIN:

        if (endGame) {
          drawFullScreenImageDMA(Winpage);
          drawString(60, 60, "YOU WIN!!", WHITE);
          endGame = 0;
        }

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          start = 1;
          state = PLAY;
          beginGame = 1;
          vBlankCounter = 0;
        }

        // state = ?
        break;
      case LOSE:
        if (endGame) {
          drawFullScreenImageDMA(LosePage);
          drawString(60, 60, "YOU LOSE!!", WHITE);
          endGame = 0;
        }

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          start = 1;
          state = PLAY;
          beginGame = 1;
          vBlankCounter = 0;
        }
        // state = ?
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}