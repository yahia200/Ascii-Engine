#include "mesh.c"
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

pthread_t handler;
char input;
struct Point pointList[100000];

void processInput() {
  switch (input) {
  case 'w':
    camera.y -= 0.5;
    break;
  case 's':
    camera.y += 0.5;
    break;
  case 'd':
    camera.x += 0.5;
    break;
  case 'a':
    camera.x -= 0.5;
    break;
  case 'm':
    camera.z -= 0.5;
    break;
  case 'k':
    camera.z += 0.5;
    break;
  }
}

void *handle() {
  while (1) {
    input = getch();
    processInput();
    input = ' ';
  }
}

void clearScreen() {
  refresh();
  clear();
}
void initScreen() {
  initscr();
  noecho();
  curs_set(0);
  brightnessRange = strlen(brightness);
  pthread_create(&handler, NULL, handle, NULL);
  getmaxyx(stdscr, MAX_HIEGHT, MAX_WIDTH);
  MAX_HIEGHT *=2;

}

void split(char str1[], char splitLine[4][20]) {
  int i, j, ctr;
  int f = 1; // true on first char
  j = 0;
  ctr = 0;

  for (i = 0; i <= (strlen(str1)); i++) {
    if (str1[i] == ' ' || str1[i] == '\0') {
      f = 1;
      splitLine[ctr][j] = '\0';
      ctr++;
      j = 0;
    } else {
      if (ctr > 0 && j == 0 && str1[i] == 'R' && f) {
        f = 0;
        continue;
      }
      splitLine[ctr][j] = str1[i];
      j++;
    }
  }
}

bool LoadFromObjectFile(char *sFilename) {
  char splitLine[4][20];
  char const *const fileName = sFilename; /* should check that argc > 1 */
  FILE *file = fopen(fileName, "r");      /* should check the result */
  char line[256];
  int c = 0;
  int t = 0;

  while (fgets(line, sizeof(line), file)) {
    if (line[0] == 'v') {
      split(line, splitLine);
      pointList[c].x = -atof(splitLine[1]);
      pointList[c].y = -atof(splitLine[2]);
      pointList[c].z = atof(splitLine[3]);
      pointList[c].w = 1;
      c++;
    }

    else if (line[0] == 'f') {
      split(line, splitLine);
      int p1 = atoi(splitLine[1]);
      int p2 = atoi(splitLine[2]);
      int p3 = atoi(splitLine[3]);
      struct Triangle tri = {pointList[p1 - 1], pointList[p2 - 1],
                             pointList[p3 - 1]};
      copyTriangle(&tri, &mesh.Triangles[t]);
      t++;
    }
  }
  mesh.size = t;

  // for(int i=0;i<c;i++){
  //   printf("\n%f   %f   %f   %f",pointList[i].x, pointList[i].y,
  //   pointList[i].z, pointList[i].w);
  // }

  fclose(file);
  return true;
}
