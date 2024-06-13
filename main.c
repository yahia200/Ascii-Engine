#include "engine.c"
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
clock_t timer;
double frameTime;
int sleepDuration = 10 * 1000;
int main() {
  initScreen();
  LoadFromObjectFile("duck.obj");
  camera.y -= 2;
  while (1) {
    timer = clock();
    clearScreen();
   drawMesh(1);
    fThetay -= 0.01;
    usleep(sleepDuration);
    timer = clock() - timer;
    frameTime =
        ((double)timer) / CLOCKS_PER_SEC + (float)sleepDuration / 1000000;
    move(0, 0);
    printw("FPS: %f", 1 / frameTime);
  }

  endwin();

  return 0;
}
