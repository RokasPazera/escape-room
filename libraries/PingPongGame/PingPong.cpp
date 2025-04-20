#include "PingPongGame.h"
#include <Arduino.h>

byte number2[] = {
  B00111100, B01000010, B00000010, B00000100,
  B00011000, B00100000, B01000000, B01111110
};

byte sad[] = {
  B00000000, B01000100, B00010000, B00010000,
  B00000000, B00111000, B01000100, B00000000
};

byte smile[] = {
  B00000000, B01000100, B00010000, B00010000,
  B00010000, B01000100, B00111000, B00000000
};

PingPongGame* PingPongGame::instance = nullptr;

PingPongGame::PingPongGame(LedControl &ledMatrix, SimpleTimer &t) : lc(ledMatrix), timer(t) {
  instance = this;
}

void PingPongGame::begin() {
  newGame();
  ball_timer = timer.setInterval(BALL_DELAY, moveBallCallback);
}

void PingPongGame::update() {
  if (!gameStopped) {
    timer.run();
    setPad();
    drawGame();
    if (checkLose()) {
      gameOver();
      newGame();
    }
  }
}

bool PingPongGame::isGameOver() {
  return gameStopped;
}

bool PingPongGame::hasWon() {
  return bounceCount >= 8;
}

void PingPongGame::restart() {
  bounceCount = 0;
  gameStopped = false;
  newGame();
}

void PingPongGame::setSprite(byte *sprite) {
  for (int r = 0; r < 8; r++) {
    lc.setRow(0, r, sprite[r]);
  }
}

void PingPongGame::newGame() {
  lc.clearDisplay(0);
  xball = random(1, 7);
  yball = 1;
  direction = random(3, 6);
  setSprite(smile);
  delay(1000);
  lc.clearDisplay(0);
}

void PingPongGame::setPad() {
  xpad = map(analogRead(POTPIN), 0, 1020, 0, 8 - PADSIZE);
}

int PingPongGame::checkBounce() {
  if (!xball || !yball || xball == 7 || yball == 6) {
    int bounce = (yball == 0 || yball == 6) ? BOUNCE_HORIZONTAL : BOUNCE_VERTICAL;
    bounceCount++;
    if (bounceCount >= 8) {
      lc.clearDisplay(0);
      setSprite(number2);
      gameStopped = true;
    }
    return bounce;
  }
  return 0;
}

int PingPongGame::getHit() {
  if (yball != 6 || xball < xpad || xball > xpad + PADSIZE) return HIT_NONE;
  if (xball == xpad + PADSIZE / 2) return HIT_CENTER;
  return xball < xpad + PADSIZE / 2 ? HIT_LEFT : HIT_RIGHT;
}

bool PingPongGame::checkLose() {
  return yball == 6 && getHit() == HIT_NONE;
}

void PingPongGame::moveBallCallback() {
  if (instance) {
    instance->moveBall();
  }
}

void PingPongGame::moveBall() {
  int bounce = checkBounce();
  if (bounce) {
    switch (direction) {
      case 0: direction = 4; break;
      case 1: direction = (bounce == BOUNCE_VERTICAL) ? 7 : 3; break;
      case 2: direction = 6; break;
      case 6: direction = 2; break;
      case 7: direction = (bounce == BOUNCE_VERTICAL) ? 1 : 5; break;
      case 5: direction = (bounce == BOUNCE_VERTICAL) ? 3 : 7; break;
      case 3: direction = (bounce == BOUNCE_VERTICAL) ? 5 : 1; break;
      case 4: direction = 0; break;
    }
  }

  switch(getHit()) {
    case HIT_LEFT:
      if (direction == 0) direction = 7;
      else if (direction == 1) direction = 0;
      break;
    case HIT_RIGHT:
      if (direction == 0) direction = 1;
      else if (direction == 7) direction = 0;
      break;
  }

  if ((direction == 0 && xball == 0) || (direction == 4 && xball == 7)) direction++;
  if (direction == 0 && xball == 7) direction = 7;
  if (direction == 4 && xball == 0) direction = 3;
  if (direction == 2 && yball == 0) direction = 3;
  if (direction == 2 && yball == 6) direction = 1;
  if (direction == 6 && yball == 0) direction = 5;
  if (direction == 6 && yball == 6) direction = 7;

  if (xball == 0 && yball == 0) direction = 3;
  if (xball == 0 && yball == 6) direction = 1;
  if (xball == 7 && yball == 6) direction = 7;
  if (xball == 7 && yball == 0) direction = 5;

  yball_prev = yball;
  if (2 < direction && direction < 6) yball++;
  else if (direction != 6 && direction != 2) yball--;

  if (0 < direction && direction < 4) xball++;
  else if (direction != 0 && direction != 4) xball--;

  xball = max(0, min(7, xball));
  yball = max(0, min(6, yball));
}

void PingPongGame::drawGame() {
  if (yball_prev != yball && yball_prev != 7) lc.setRow(0, yball_prev, 0);
  
  byte padmap = byte(0xFF >> (8 - PADSIZE) << xpad);
  
  if (yball != 7) lc.setRow(0, yball, byte(1 << xball));
  lc.setRow(0, 7, padmap);
}


void PingPongGame::gameOver() {
  setSprite(sad);
  delay(1500);
  lc.clearDisplay(0);
}
