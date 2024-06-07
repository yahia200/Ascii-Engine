#include "mesh.c"
#include <string.h>

void clearScreen() {
  refresh();
  clear();
}
void initScreen() {
  initscr();
  noecho();
  curs_set(0);
  brightnessRange = strlen(brightness);
  initMat();
  rotate();
}
