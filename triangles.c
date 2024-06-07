#include "point.c"
int comparePoints(const void *a, const void *b) {
  struct Triangle *t1 = (struct Triangle *)a;
  struct Triangle *t2 = (struct Triangle *)b;
  int mid1 = ((t1->p[0].z) + (t1->p[1].z) + (t1->p[2].z)) / 3;
  int mid2 = ((t2->p[0].z) + (t2->p[1].z) + (t2->p[2].z)) / 3;
  return mid1 - mid2;
}


void fillBottomFlatTriangle(struct Point v1, struct Point v2, struct Point v3) {
  float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
  float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

  float curx1 = v1.x;
  float curx2 = v1.x;

  for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
    drawLine((struct Point){(int)curx1, scanlineY},
             (struct Point){(int)curx2, scanlineY});
    curx1 += invslope1;
    curx2 += invslope2;
  }
}

void fillTopFlatTriangle(struct Point v1, struct Point v2, struct Point v3) {
  float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
  float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

  float curx1 = v3.x;
  float curx2 = v3.x;

  for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
    drawLine((struct Point){(int)curx1, scanlineY},
             (struct Point){(int)curx2, scanlineY});
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

void FillTriangle(struct Point p1, struct Point p2, struct Point p3) {
  struct Point v1 = {(int)p1.x, (int)p1.y};
  struct Point v2 = {(int)p2.x, (int)p2.y};
  struct Point v3 = {(int)p3.x, (int)p3.y};

  if (v2.y == v3.y) {
    fillBottomFlatTriangle(v1, v2, v3);
  } else if (v1.y == v2.y) {
    fillTopFlatTriangle(v1, v2, v3);
  } else {
    struct Point v4 = {
        (int)(v1.x +
              ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)),
        v2.y};
    fillBottomFlatTriangle(v1, v2, v4);
    fillTopFlatTriangle(v2, v4, v3);
  }
}
void sortFill(struct Triangle *triangle) {

  if (triangle->p[0].y <= triangle->p[1].y &&
      triangle->p[0].y <= triangle->p[2].y) {
    if (triangle->p[1].y < triangle->p[2].y)
      FillTriangle(triangle->p[0], triangle->p[1], triangle->p[2]);
    else
      FillTriangle(triangle->p[0], triangle->p[2], triangle->p[1]);
  }

  else if (triangle->p[1].y <= triangle->p[0].y &&
           triangle->p[1].y <= triangle->p[2].y) {
    if (triangle->p[2].y < triangle->p[0].y)
      FillTriangle(triangle->p[1], triangle->p[2], triangle->p[0]);
    else
      FillTriangle(triangle->p[1], triangle->p[0], triangle->p[2]);
  }

  else {
    if (triangle->p[0].y < triangle->p[1].y)
      FillTriangle(triangle->p[2], triangle->p[0], triangle->p[1]);
    else
      FillTriangle(triangle->p[2], triangle->p[1], triangle->p[0]);
  }
}

void drawTriangle(struct Triangle *triangle, int fill) {
  struct Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX,
      triRotatedZXY;
  MultiplyMatrixVector(&triangle->p[0], &triRotatedZ.p[0], matRotZ);
  MultiplyMatrixVector(&triangle->p[1], &triRotatedZ.p[1], matRotZ);
  MultiplyMatrixVector(&triangle->p[2], &triRotatedZ.p[2], matRotZ);

  MultiplyMatrixVector(&triRotatedZ.p[0], &triRotatedZX.p[0], matRotX);
  MultiplyMatrixVector(&triRotatedZ.p[1], &triRotatedZX.p[1], matRotX);
  MultiplyMatrixVector(&triRotatedZ.p[2], &triRotatedZX.p[2], matRotX);

  MultiplyMatrixVector(&triRotatedZX.p[0], &triRotatedZXY.p[0], matRotY);
  MultiplyMatrixVector(&triRotatedZX.p[1], &triRotatedZXY.p[1], matRotY);
  MultiplyMatrixVector(&triRotatedZX.p[2], &triRotatedZXY.p[2], matRotY);

  triTranslated = triRotatedZXY;
  triTranslated.p[0].z = triRotatedZXY.p[0].z + 3.0f;
  triTranslated.p[1].z = triRotatedZXY.p[1].z + 3.0f;
  triTranslated.p[2].z = triRotatedZXY.p[2].z + 3.0f;

  struct Point normal, line1, line2;
  line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
  line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
  line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

  line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
  line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
  line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

  normal.x = line1.y * line2.z - line1.z * line2.y;
  normal.y = line1.z * line2.x - line1.x * line2.z;
  normal.z = line1.x * line2.y - line1.y * line2.x;

  float l =
      sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
  normal.x /= l;
  normal.y /= l;
  normal.z /= l;

  if (normal.x * (triTranslated.p[0].x - vCamera.x) +
          normal.y * (triTranslated.p[0].y - vCamera.y) +
          normal.z * (triTranslated.p[0].z - vCamera.z) <
      0.0f) {
    struct Point light_direction = {0.0f, 0.0f, -1.0f};
    float l = sqrtf(light_direction.x * light_direction.x +
                    light_direction.y * light_direction.y +
                    light_direction.z * light_direction.z);
    light_direction.x /= l;
    light_direction.y /= l;
    light_direction.z /= l;

    float dp = normal.x * light_direction.x + normal.y * light_direction.y +
               normal.z * light_direction.z;

    intesity = (int)(dp * brightnessRange);

    MultiplyMatrixVector(&triTranslated.p[0], &triProjected.p[0], matProj);
    MultiplyMatrixVector(&triTranslated.p[1], &triProjected.p[1], matProj);
    MultiplyMatrixVector(&triTranslated.p[2], &triProjected.p[2], matProj);
    triProjected.p[0].x += 1.0f;
    triProjected.p[0].y += 1.0f;
    triProjected.p[1].x += 1.0f;
    triProjected.p[1].y += 1.0f;
    triProjected.p[2].x += 1.0f;
    triProjected.p[2].y += 1.0f;
    triProjected.p[0].x *= 0.5f * (float)MAX_WIDTH;
    triProjected.p[0].y *= 0.5f * (float)MAX_HIEGHT;
    triProjected.p[1].x *= 0.5f * (float)MAX_WIDTH;
    triProjected.p[1].y *= 0.5f * (float)MAX_HIEGHT;
    triProjected.p[2].x *= 0.5f * (float)MAX_WIDTH;
    triProjected.p[2].y *= 0.5f * (float)MAX_HIEGHT;
    if (!fill) {
      drawLine(triProjected.p[0], triProjected.p[1]);
      drawLine(triProjected.p[1], triProjected.p[2]);
      drawLine(triProjected.p[2], triProjected.p[0]);
    } else {
      sortFill(&triProjected);
    }
  }
}
