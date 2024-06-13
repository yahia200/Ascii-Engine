#include "point.c"
#include <ncurses.h>
#include <stdlib.h>
int place = 0;
struct Mesh mesh;
struct mat matW;
struct Triangle triToDraw[MAX_MESH_SIZE];
void copyTriangle(struct Triangle *a, struct Triangle *b) {
  b->p[0].x = a->p[0].x;
  b->p[0].y = a->p[0].y;
  b->p[0].z = a->p[0].z;
  b->p[0].w = a->p[0].w;

  b->p[1].x = a->p[1].x;
  b->p[1].y = a->p[1].y;
  b->p[1].z = a->p[1].z;
  b->p[1].w = a->p[1].w;

  b->p[2].x = a->p[2].x;
  b->p[2].y = a->p[2].y;
  b->p[2].z = a->p[2].z;
  b->p[2].w = a->p[2].w;

  b->intensity = a->intensity;
}
int comparePoints(const void *a, const void *b) {
  struct Triangle t1;
  copyTriangle((struct Triangle *)a, &t1);
  Matrix_MultiplyPoint(matW, t1.p[0], &t1.p[0]);
  Matrix_MultiplyPoint(matW, t1.p[1], &t1.p[1]);
  Matrix_MultiplyPoint(matW, t1.p[2], &t1.p[2]);

  struct Triangle t2;
  copyTriangle((struct Triangle *)b, &t2);
  Matrix_MultiplyPoint(matW, t2.p[0], &t2.p[0]);
  Matrix_MultiplyPoint(matW, t2.p[1], &t2.p[1]);
  Matrix_MultiplyPoint(matW, t2.p[2], &t2.p[2]);
  float mid1 = ((t1.p[0].z) + (t1.p[1].z) + (t1.p[2].z)) / 0.003;
  float mid2 = ((t2.p[0].z) + (t2.p[1].z) + (t2.p[2].z)) / 0.003;
  return mid2 - mid1;
}

void sortTri(struct Triangle t[MAX_MESH_SIZE], int s) {

  qsort(t, s, sizeof(struct Triangle), comparePoints);
}
int comparePointsy(const void *a, const void *b) {
  struct Triangle t1;
  copyTriangle((struct Triangle *)a, &t1);
  Matrix_MultiplyPoint(matW, t1.p[0], &t1.p[0]);
  Matrix_MultiplyPoint(matW, t1.p[1], &t1.p[1]);
  Matrix_MultiplyPoint(matW, t1.p[2], &t1.p[2]);

  struct Triangle t2;
  copyTriangle((struct Triangle *)b, &t2);
  Matrix_MultiplyPoint(matW, t2.p[0], &t2.p[0]);
  Matrix_MultiplyPoint(matW, t2.p[1], &t2.p[1]);
  Matrix_MultiplyPoint(matW, t2.p[2], &t2.p[2]);
  float mid1 = ((t1.p[0].y) + (t1.p[1].y) + (t1.p[2].y)) / 0.003;
  float mid2 = ((t2.p[0].y) + (t2.p[1].y) + (t2.p[2].y)) / 0.003;
  return mid2 - mid1;
}

void sortTriy(struct Triangle t[MAX_MESH_SIZE], int s) {

  qsort(t, s, sizeof(struct Triangle), comparePointsy);
}
void fillBottomFlatTriangle(struct Point v1, struct Point v2, struct Point v3,
                            int intensity) {
  float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
  float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

  float curx1 = v1.x;
  float curx2 = v1.x;

  for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
    drawLine((struct Point){(int)curx1, scanlineY},
             (struct Point){(int)curx2, scanlineY}, intensity);
    curx1 += invslope1;
    curx2 += invslope2;
  }
}

void fillTopFlatTriangle(struct Point v1, struct Point v2, struct Point v3,
                         int intensity) {
  float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
  float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

  float curx1 = v3.x;
  float curx2 = v3.x;

  for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
    drawLine((struct Point){(int)curx1, scanlineY},
             (struct Point){(int)curx2, scanlineY}, intensity);
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

void FillTriangle(struct Point p1, struct Point p2, struct Point p3,
                  int intesity) {
  struct Point v1 = {(int)p1.x, (int)p1.y};
  struct Point v2 = {(int)p2.x, (int)p2.y};
  struct Point v3 = {(int)p3.x, (int)p3.y};

  if (v2.y == v3.y) {
    fillBottomFlatTriangle(v1, v2, v3, intesity);
  } else if (v1.y == v2.y) {
    fillTopFlatTriangle(v1, v2, v3, intesity);
  } else {
    struct Point v4 = {
        (int)(v1.x +
              ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)),
        v2.y};
    fillBottomFlatTriangle(v1, v2, v4, intesity);
    fillTopFlatTriangle(v2, v4, v3, intesity);
  }
}
void sortFill(struct Triangle *triangle) {

  if (triangle->p[0].y <= triangle->p[1].y &&
      triangle->p[0].y <= triangle->p[2].y) {
    if (triangle->p[1].y < triangle->p[2].y)
      FillTriangle(triangle->p[0], triangle->p[1], triangle->p[2],
                   triangle->intensity);
    else
      FillTriangle(triangle->p[0], triangle->p[2], triangle->p[1],
                   triangle->intensity);
  }

  else if (triangle->p[1].y <= triangle->p[0].y &&
           triangle->p[1].y <= triangle->p[2].y) {
    if (triangle->p[2].y < triangle->p[0].y)
      FillTriangle(triangle->p[1], triangle->p[2], triangle->p[0],
                   triangle->intensity);
    else
      FillTriangle(triangle->p[1], triangle->p[0], triangle->p[2],
                   triangle->intensity);
  }

  else {
    if (triangle->p[0].y < triangle->p[1].y)
      FillTriangle(triangle->p[2], triangle->p[0], triangle->p[1],
                   triangle->intensity);
    else
      FillTriangle(triangle->p[2], triangle->p[1], triangle->p[0],
                   triangle->intensity);
  }
}
void printMatrix(struct mat m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printw("%f\t", m.m[i][j]);
    }
    printw("\n");
  }
  printw("\n");
}

void rasterTriangle(struct Triangle t, int fill) {

  if (!fill) {
    drawLine(t.p[0], t.p[1], t.intensity);
    drawLine(t.p[1], t.p[2], t.intensity);
    drawLine(t.p[2], t.p[0], t.intensity);
  } else {
    sortFill(&t);
  }
}

void clipp(struct Triangle t, int fill) {
  struct Triangle clipped[2];
  struct Triangle listTriangles[MAX_MESH_SIZE];
  int ind = 0;
  // Add initial triangle
  copyTriangle(&t, &listTriangles[ind++]);
  int nNewTriangles = 1;

  for (int p = 0; p < 4; p++) {
    int nTrisToAdd = 0;
    while (nNewTriangles > 0) {
      // Take triangle from front of queue
      struct Triangle test;
      copyTriangle(&listTriangles[ind--], &test);
      nNewTriangles--;

      // Clip it against a plane. We only need to test each
      // subsequent plane, against subsequent new triangles
      // as all triangles after a plane clip are guaranteed
      // to lie on the inside of the plane. I like how this
      // comment is almost completely and utterly justified
      switch (p) {
      case 0:
        nTrisToAdd = Triangle_ClipAgainstPlane((struct Point){0.0f, 0.0f, 0.0f},
                                               (struct Point){0.0f, 1.0f, 0.0f},
                                               &test, &clipped[0], &clipped[1]);
        break;
      case 1:
        nTrisToAdd = Triangle_ClipAgainstPlane(
            (struct Point){0.0f, (float)MAX_HIEGHT - 1, 0.0f},
            (struct Point){0.0f, -1.0f, 0.0f}, &test, &clipped[0], &clipped[1]);
        break;
      case 2:
        nTrisToAdd = Triangle_ClipAgainstPlane((struct Point){0.0f, 0.0f, 0.0f},
                                               (struct Point){1.0f, 0.0f, 0.0f},
                                               &test, &clipped[0], &clipped[1]);
        break;
      case 3:
        nTrisToAdd = Triangle_ClipAgainstPlane(
            (struct Point){(float)MAX_WIDTH - 1, 0.0f, 0.0f},
            (struct Point){-1.0f, 0.0f, 0.0f}, &test, &clipped[0], &clipped[1]);
        break;
      }

      // Clipping may yield a variable number of triangles, so
      // add these new ones to the back of the queue for subsequent
      // clipping against next planes
      for (int w = 0; w < nTrisToAdd; w++)
        copyTriangle(&clipped[w], &listTriangles[++ind]);
    }
    nNewTriangles = ind + 1;
  }
  for (int i = 0; i <= ind; i++) {
    rasterTriangle(listTriangles[i], fill);
  }
}

void drawTriangle(struct Mesh *m, int fill) {
  int size = 0;
  struct Triangle triProjected, triTransformed, triViewed;
  struct mat matProj, matRotZ, matRotX, matRotY, matWorld;
  Matrix_MakeRotationZ(fThetaz, &matRotZ);
  Matrix_MakeRotationX(fThetax, &matRotX);
  Matrix_MakeRotationY(fThetay, &matRotY);
  Matrix_MakeProjection(90.0f, (float)MAX_HIEGHT / (float)MAX_WIDTH, 0.1f,
                        1000.0f, &matProj);
  struct mat matTrans;
  Matrix_MakeTranslation(0, 0, 3, &matTrans);

  Matrix_MultiplyMatrix(matRotZ, matRotX, &matWorld);  // Transform by rotation
  Matrix_MultiplyMatrix(matWorld, matRotY, &matWorld); // Transform by rotation
  Matrix_MultiplyMatrix(matWorld, matTrans,
                        &matWorld); // Transform by translation

  matW = matWorld;

  struct Point pUp = {0, 1, 0, 1};
  lookDir.x = 0;
  lookDir.y = 0;
  lookDir.z = 1;
  lookDir.w = 1;
  struct Point target;
  Point_Add(camera, lookDir, &target);

  struct mat matCamera;
  Matrix_PointAt(camera, target, pUp, &matCamera);

  struct mat matView;
  Matrix_QuickInverse(matCamera, &matView);
  for (int i = 0; i < m->size; i++) {
    Matrix_MultiplyPoint(matWorld, m->Triangles[i].p[0], &triTransformed.p[0]);
    Matrix_MultiplyPoint(matWorld, m->Triangles[i].p[1], &triTransformed.p[1]);
    Matrix_MultiplyPoint(matWorld, m->Triangles[i].p[2], &triTransformed.p[2]);

    struct Point normal, line1, line2;
    Point_Sub(triTransformed.p[1], triTransformed.p[0], &line1);
    Point_Sub(triTransformed.p[2], triTransformed.p[0], &line2);

    // Take cross product of lines to get normal to triangle surface
    Point_CrossProduct(line1, line2, &normal);

    // You normally need to normalise a normal!
    normal = Point_Normalise(normal);
    struct Point vCameraRay;
    Point_Sub(triTransformed.p[0], camera, &vCameraRay);
    // printw("%f  %f  %f  %f\n",normal.x, normal.y,normal.z, normal.w);

    // printw("  %f\n", Point_DotProduct(normal, vCamera));
    // printw("\n%f  %f  %f  %f", triTransformed.p[0].x, triTransformed.p[0].y,
    // triTransformed.p[0].z, triTransformed.p[0].w);
    if (Point_DotProduct(normal, vCameraRay) < 0.0f) {
      struct Point light_direction = {0.0f, 0.0f, -1.0f, 1};
      light_direction = Point_Normalise(light_direction);

      // How "aligned" are light direction and triangle surface normal?
      float dp = fmax(0.1f, Point_DotProduct(light_direction, normal));

      int intensity = (int)(dp * brightnessRange);
      // view
      Matrix_MultiplyPoint(matView, triTransformed.p[0], &triViewed.p[0]);
      Matrix_MultiplyPoint(matView, triTransformed.p[2], &triViewed.p[1]);
      Matrix_MultiplyPoint(matView, triTransformed.p[1], &triViewed.p[2]);
      int nClipped = 0;
      struct Triangle clipped[2];
      nClipped = Triangle_ClipAgainstPlane(
          (struct Point){0, 0, 0.1, 1}, (struct Point){0, 0, 1, 1}, &triViewed,
          &clipped[0], &clipped[1]);

      // printw("%d", nClipped);
      // printw("\n%f  %f %f  %f", clipped[0].p[0].x, clipped[0].p[0].y,
      // clipped[0].p[0].z, clipped[0].p[0].w);
      for (int n = 0; n < nClipped; n++) {
        // Project
        Matrix_MultiplyPoint(matProj, clipped[n].p[0], &triProjected.p[0]);
        Matrix_MultiplyPoint(matProj, clipped[n].p[1], &triProjected.p[1]);
        Matrix_MultiplyPoint(matProj, clipped[n].p[2], &triProjected.p[2]);

        // normalize
        Point_Div(triProjected.p[0], triProjected.p[0].w, &triProjected.p[0]);
        Point_Div(triProjected.p[1], triProjected.p[1].w, &triProjected.p[1]);
        Point_Div(triProjected.p[2], triProjected.p[2].w, &triProjected.p[2]);
        // printw("%f  %f  %f  %f\n", triProjected.p[0].x, triProjected.p[0].y,
        // triProjected.p[0].z, triProjected.p[0].w);
        // offset
        struct Point vOffsetView = {1, 1, 0, 1};
        Point_Add(triProjected.p[0], vOffsetView, &triProjected.p[0]);
        Point_Add(triProjected.p[1], vOffsetView, &triProjected.p[1]);
        Point_Add(triProjected.p[2], vOffsetView, &triProjected.p[2]);
        triProjected.p[0].x *= 0.5f * (float)MAX_WIDTH;
        triProjected.p[0].y *= 0.5f * (float)MAX_HIEGHT;
        triProjected.p[1].x *= 0.5f * (float)MAX_WIDTH;
        triProjected.p[1].y *= 0.5f * (float)MAX_HIEGHT;
        triProjected.p[2].x *= 0.5f * (float)MAX_WIDTH;
        triProjected.p[2].y *= 0.5f * (float)MAX_HIEGHT;
        triProjected.intensity = intensity;
        // rasterTriangle(triProjected, fill);
        copyTriangle(&triProjected, &triToDraw[size++]);
        // clipp(triProjected, fill);
      }
    }
    
  }
for (int i = 0; i < size; i++) {
      rasterTriangle(triToDraw[i], fill);
    }
}
