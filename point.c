#include "matrix.c"
struct Point vCamera = {0, 0, 0};
void drawPoint(struct Point point) {
  if (point.x >= 0 && point.x < MAX_WIDTH && point.y >= 0 &&
      point.y < MAX_HIEGHT) {
    mvwaddch(stdscr, (int)(point.y / 2), (int)(point.x), brightness[intesity]);
  }
}
void drawLine(struct Point po1, struct Point po2) {
  struct Point p1 = {(int)po1.x, (int)po1.y, (int)po1.z};
  struct Point p2 = {(int)po2.x, (int)po2.y, (int)po2.z};
  int dx = (int)(fabsf(p2.x - p1.x)), sx = p1.x < p2.x ? 1 : -1;
  int dy = (int)(-fabsf(p2.y - p1.y)), sy = p1.y < p2.y ? 1 : -1;
  int err = dx + dy, e; /* error value e_xy */

  for (;;) {
    drawPoint(p1);
    if (p1.x == p2.x && p1.y == p2.y)
      break;
    e = 2 * err;
    if (e >= dy) {
      err += dy;
      p1.x += sx;
    }
    if (e <= dx) {
      err += dx;
      p1.y += sy;
    }
  }
}
void MultiplyMatrixVector(struct Point *i, struct Point *o, float m[4][4]) {
  o->x = i->x * m[0][0] + i->y * m[1][0] + i->z * m[2][0] + m[3][0];
  o->y = i->x * m[0][1] + i->y * m[1][1] + i->z * m[2][1] + m[3][1];
  o->z = i->x * m[0][2] + i->y * m[1][2] + i->z * m[2][2] + m[3][2];
  float w = i->x * m[0][3] + i->y * m[1][3] + i->z * m[2][3] + m[3][3];

  if (w != 0.0f) {
    o->x /= w;
    o->y /= w;
    o->z /= w;
  }
}
