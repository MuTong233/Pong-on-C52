/********

                Ping-Pong Game for 8051 w/ PlatformIO.

                Copyright Team MyGensou 2018-2025

                The code is licensed under the GPLv3 Open Source License

********/

#include <extra_function.h>

// Data type define
typedef unsigned int u16;
typedef unsigned char u8;

// Data Buffer
int GameDiff, GameInProgress, KeyOp, Score1P, Score2P, GameWinner, GameStarter;
int DelayTime = 150;

// Initialize Game Data
void GameDataInitialize() {
  GameDiff = 0;
  GameInProgress = 0;
  KeyOp = 0;
  Score1P = 0;
  Score2P = 0;
  GameWinner = 0;
  DelayTime = 150;
  GameStarter = 1;
}

// Game Difficult Selector
void GameDiffSelect() {
  while (1) {
    doShowNixie(1, 5);
    doShowNixie(1, 21);
    doShowNixie(2, 14);
    doShowNixie(2, 21);
    doShowNixie(3, 18);
    doShowNixie(3, 21);
    doShowNixie(4, 20);
    doShowNixie(4, 21);
    doShowNixie(5, 13);
    doShowNixie(5, 21);
    doShowNixie(6, 1);
    doShowNixie(6, 21);
    doShowNixie(7, 15);
    doShowNixie(7, 21);
    doShowNixie(8, 15);
    doShowNixie(8, 21);
    GameDiff = doKeyScan(0);
    if (GameDiff != 0)
      break;
  }
  for (DelayTime = 150; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 13);
    doShowNixie(1, 21);
    doShowNixie(2, 1);
    doShowNixie(2, 21);
    doShowNixie(3, 15);
    doShowNixie(3, 21);
    doShowNixie(4, 15);
    doShowNixie(4, 21);
    doShowNixie(7, GameDiff);
    doShowNixie(7, 21);
  }
}

// Game Starter Selector
void GameInitSelect() {
  GameStarter = 0;
  while (1) {
    doShowNixie(1, 5);
    doShowNixie(1, 21);
    doShowNixie(2, 14);
    doShowNixie(2, 21);
    doShowNixie(3, 18);
    doShowNixie(3, 21);
    doShowNixie(4, 20);
    doShowNixie(4, 21);
    doShowNixie(5, 15);
    doShowNixie(5, 21);
    doShowNixie(6, 26);
    doShowNixie(6, 21);
    doShowNixie(7, 5);
    doShowNixie(7, 21);
    doShowNixie(8, 19);
    doShowNixie(8, 21);
    GameStarter = doKeyScan(0);
    if (GameStarter == 1 || GameStarter == 2)
      break;
  }
  for (DelayTime = 150; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 15);
    doShowNixie(1, 21);
    doShowNixie(2, 26);
    doShowNixie(2, 21);
    doShowNixie(3, 5);
    doShowNixie(3, 21);
    doShowNixie(4, 19);
    doShowNixie(4, 21);
    doShowNixie(7, GameStarter);
    doShowNixie(7, 21);
  }
}

// Result Check Tools
void ScoreCheck() {
  if (Score1P == 10)
    GameWinner = 1;
  if (Score2P == 10)
    GameWinner = 2;
  if (Score1P == 10 && Score2P == 10)
    GameWinner = 3;
}

// Show Game Result
void GameResultShow(u16 i) {
  for (DelayTime = 300; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 20);
    doShowNixie(1, 21);
    doShowNixie(2, i);
    doShowNixie(2, 21);
    doShowNixie(3, 22);
    doShowNixie(3, 21);
    doShowNixie(4, 20);
    doShowNixie(4, 21);
    doBeep(1);
  }
}

// Check Game Result
void GameResultCheck() {
  switch (GameWinner) {
  case 0:
    break;
  case 1:
    GameResultShow(1);
    break;
  case 2:
    GameResultShow(2);
    break;
  case 3:
    doErrorReport(8, 7);
    break;
  default:
    doErrorReport(6, 3);
    break;
  }
}

// Show the Score
void ShowScore() {
  for (DelayTime = 450; DelayTime > 0; DelayTime--) {
    doShowNixie(1, 1);
    doShowNixie(1, 21);
    doShowNixie(2, 20);
    doShowNixie(2, 21);
    doShowNixie(3, Score1P / 10);
    doShowNixie(3, 21);
    doShowNixie(4, Score1P % 10);
    doShowNixie(4, 21);
    doShowNixie(5, 2);
    doShowNixie(5, 21);
    doShowNixie(6, 20);
    doShowNixie(6, 21);
    doShowNixie(7, Score2P / 10);
    doShowNixie(7, 21);
    doShowNixie(8, Score2P % 10);
    doShowNixie(8, 21);
  }
}

// Main Game Function
void GameMain() {
  // TODO: Rework this dumb things with IRQ based stuff.

  int i, away = 0;
  GameInProgress = 1; // Let Game Kick the bass!

  // Game main logic
  while (away != 1) {
    KeyOp = 0;
    away = 0;

    // start from 1p
    for (i = 0; i < 7; i++) {
      if (GameStarter != 1) {
        break;
      }
      doShowLED(i, 100);
      switch (GameDiff) {
      case 0:
        doErrorReport(3, 6);
        break;
      case 1:
        Delay60ms();
        KeyOp = doKeyScan(0);
        if (KeyOp != 0) {
          away = 1;
          break;
        }
        Delay40ms();
        break;
      case 2:
        Delay60ms();
        break;
      case 3:
        Delay40ms();
        break;
      case 4:
        Delay20ms();
        break;
      default:
        doErrorReport(3, 7);
        break;
      }
      KeyOp = doKeyScan(0);
      if (KeyOp != 0) {
        away = 1;
        break;
      }
    }

    // Check if pre-pressed btn
    if (away == 1) {
      Score1P = Score1P + 1;
      GameStarter = 1;
      break;
    } else {
      away = 0;
    }

    // Now it's the fate to check the key!
    switch (GameDiff) {
    case 0:
      doErrorReport(4, 1);
      break;
    case 1:
      DelayTime = 1000;
      break;
    case 2:
      DelayTime = 650;
      break;
    case 3:
      DelayTime = 425;
      break;
    case 4:
      DelayTime = 200;
      break;
    default:
      doErrorReport(4, 2);
      break;
    }

    // Wait for input
    for (; DelayTime > 0; DelayTime--) {
      if (GameStarter != 1) {
        break;
      }
      doShowLED(7, 100);
      KeyOp = doKeyScan(0);
      if (KeyOp == 0) {
        away = 1;
      } else {
        away = 0;
        break;
      }
    }

    // If time's up, terminate the game
    if (DelayTime == 0 || away == 1) {
      Score1P = Score1P + 1;
      GameStarter = 1;
      break;
    }

    // Otherwise reset key counter
    KeyOp = 0;
    away = 0;
    GameStarter = 2;

    // now it's 2p's turn
    for (i = 0; i < 7; i++) {
      if (GameStarter != 2) {
        break;
      }
      doShowLED(7 - i, 100);
      switch (GameDiff) {
      case 0:
        doErrorReport(3, 6);
        break;
      case 1:
        Delay60ms();
        KeyOp = doKeyScan(0);
        if (KeyOp != 0) {
          away = 1;
          break;
        }
        Delay40ms();
        break;
      case 2:
        Delay60ms();
        break;
      case 3:
        Delay40ms();
        break;
      case 4:
        Delay20ms();
        break;
      default:
        doErrorReport(3, 7);
        break;
      }
      KeyOp = doKeyScan(0);
      if (KeyOp != 0) {
        away = 1;
        break;
      }
    }

    if (away == 1) {
      Score2P = Score2P + 1;
      GameStarter = 2;
      break;
    } else {
      away = 0;
    }

    // Now it's the fate to check the key!
    switch (GameDiff) {
    case 0:
      doErrorReport(4, 1);
      break;
    case 1:
      DelayTime = 1000;
      break;
    case 2:
      DelayTime = 650;
      break;
    case 3:
      DelayTime = 425;
      break;
    case 4:
      DelayTime = 200;
      break;
    default:
      doErrorReport(4, 2);
      break;
    }

    for (; DelayTime > 0; DelayTime--) {
      if (GameStarter != 2) {
        break;
      }
      doShowLED(0, 100);
      KeyOp = doKeyScan(0);
      if (KeyOp == 0) {
        away = 1;
      } else {
        away = 0;
        break;
      }
    }

    if (DelayTime == 0 || away == 1) {
      Score2P = Score2P + 1;
      GameStarter = 2;
      break;
    } else {
      away = 0;
    }
    GameStarter = 1;
  }
  GameInProgress = 0; // Stop the logic
}

// main
void main() {
  SystemPost(); // TODO: Rework this function to initialize UART and many other
                // things.
  GameDataInitialize();
  while (1) {
    // TODO: Full game logic rewrite.
    SystemAttract();
    doWaitLED(3);
    GameDiffSelect();
    doWaitLED(3);
    GameInitSelect();
    doWaitLED(3);
    CountDown(5);
    doClearDisp();
    while (1) {
      GameMain();
      ShowScore();
      ScoreCheck();
      CountDown(3);
      if (GameWinner != 0)
        break;
      if (GameInProgress == 1)
        doErrorReport(5, 1);
    }
    doWaitLED(3);
    GameResultCheck();
    doWaitLED(3);
    if (GameInProgress == 1)
      break;
    GameDataInitialize();
    CountDown(5);
    doClearDisp();
  }
  // TODO: Better error handler.
  doErrorReport(6, 8);
}