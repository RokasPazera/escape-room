#ifndef PINGPONGGAME_H
#define PINGPONGGAME_H

#include <LedControl.h>
#include <SimpleTimer.h>

#define POTPIN A5
#define PADSIZE 3
#define BALL_DELAY 200
#define BOUNCE_VERTICAL 1
#define BOUNCE_HORIZONTAL -1
#define HIT_NONE 0
#define HIT_CENTER 1
#define HIT_LEFT 2
#define HIT_RIGHT 3

class PingPongGame {
  public:
    PingPongGame(LedControl &ledMatrix, SimpleTimer &timer);
    void begin();
    void update();
    bool isGameOver();
    void restart();
    bool hasWon();

  private:
    LedControl &lc;
    SimpleTimer &timer;

    static PingPongGame* instance;   // static pointer to the active instance

    int bounceCount = 0;
    bool gameStopped = false;
    int ball_timer;
    byte direction;
    int xball, yball, yball_prev;
    byte xpad;

    void moveBall();                // the real logic function
    static void moveBallCallback(); // static wrapper for SimpleTimer

    int checkBounce();
    int getHit();
    bool checkLose();
    void drawGame();
    void gameOver();
    void setPad();
    void newGame();
    void setSprite(byte *sprite);
};

#endif
