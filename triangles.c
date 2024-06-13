#include "point.c"
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
struct Mesh mesh;
struct mat matW;
struct mat matProj, matRotZ, matRotX, matRotY, matWorld;
struct mat matTrans;
struct mat matView;
struct args as[THREADS];
struct Triangle transTris[MAX_MESH_SIZE];
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
   Matrix_MultiplyPointZ(matWorld, t1.p[0], &t1.p[0]);
   Matrix_MultiplyPointZ(matWorld, t1.p[1], &t1.p[1]);
   Matrix_MultiplyPointZ(matWorld, t1.p[2], &t1.p[2]);

  struct Triangle t2;
   copyTriangle((struct Triangle *)b, &t2);
   Matrix_MultiplyPointZ(matWorld, t2.p[0], &t2.p[0]);
   Matrix_MultiplyPointZ(matWorld, t2.p[1], &t2.p[1]);
   Matrix_MultiplyPointZ(matWorld, t2.p[2], &t2.p[2]);
  float mid1 = ((t1.p[0].z) + (t1.p[1].z) + (t1.p[2].z)) / 0.003;
  float mid2 = ((t2.p[0].z) + (t2.p[1].z) + (t2.p[2].z)) / 0.003;
  return mid2 - mid1;
}

void sortTri(struct Triangle t[MAX_MESH_SIZE], int s) {

  qsort(t, s, sizeof(struct Triangle), comparePoints);
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

void *drawDiv(void *in) {
  struct Triangle triProjected, triViewed, triTransformed;
  int start = ((struct args *)in)->start;
  int end = ((struct args *)in)->end;
  int size = 0;
  for (int i = start; i < end; i++) {
    Matrix_MultiplyPoint(matWorld, mesh.Triangles[i].p[0],
                         &triTransformed.p[0]);
    Matrix_MultiplyPoint(matWorld, mesh.Triangles[i].p[1],
                         &triTransformed.p[1]);
    Matrix_MultiplyPoint(matWorld, mesh.Triangles[i].p[2],
                         &triTransformed.p[2]);

    struct Point normal, line1, line2;
    Point_Sub(triTransformed.p[1], triTransformed.p[0], &line1);
    Point_Sub(triTransformed.p[2], triTransformed.p[0], &line2);

    
    Point_CrossProduct(line1, line2, &normal);

    
    normal = Point_Normalise(normal);
    struct Point vCameraRay;
    Point_Sub(triTransformed.p[0], camera, &vCameraRay);
    

    
    if (Point_DotProduct(normal, vCameraRay) < 0.0f) {
      struct Point light_direction = {0.0f, 0.0f, -1.0f, 1};
      light_direction = Point_Normalise(light_direction);

      
      float dp = fmax(0.1f, Point_DotProduct(light_direction, normal));

      int intensity = (int)(dp * brightnessRange);

      // view
      Matrix_MultiplyPoint(matView, triTransformed.p[0], &triViewed.p[0]);
      Matrix_MultiplyPoint(matView, triTransformed.p[2], &triViewed.p[1]);
      Matrix_MultiplyPoint(matView, triTransformed.p[1], &triViewed.p[2]);
      int nClipped = 0;
      struct Triangle clipped[2];
      nClipped = Triangle_ClipAgainstPlane(
          (struct Point){0, 0, 1, 1}, (struct Point){0, 0, 1, 1}, &triViewed,
          &clipped[0], &clipped[1]);

      
      for (int n = 0; n < nClipped; n++) {
        // Project
        Matrix_MultiplyPoint(matProj, clipped[n].p[0], &triProjected.p[0]);
        Matrix_MultiplyPoint(matProj, clipped[n].p[1], &triProjected.p[1]);
        Matrix_MultiplyPoint(matProj, clipped[n].p[2], &triProjected.p[2]);

        // normalize
        Point_Div(triProjected.p[0], triProjected.p[0].w, &triProjected.p[0]);
        Point_Div(triProjected.p[1], triProjected.p[1].w, &triProjected.p[1]);
        Point_Div(triProjected.p[2], triProjected.p[2].w, &triProjected.p[2]);
        
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
        copyTriangle(&triProjected, &((struct args *)in)->t[size++]);
      }
    }
  }
  ((struct args *)in)->s = size;
  return 0;
}

void drawTriangle(int fill) {
  pthread_t threads[THREADS];
  Matrix_MakeRotationZ(fThetaz, &matRotZ);
  Matrix_MakeRotationX(fThetax, &matRotX);
  Matrix_MakeRotationY(fThetay, &matRotY);
  Matrix_MakeProjection(90.0f, (float)MAX_HIEGHT / (float)MAX_WIDTH, 0.1f,
                        1000.0f, &matProj);

  Matrix_MakeTranslation(0, 0, 3, &matTrans);

  Matrix_MultiplyMatrix(matRotZ, matRotX, &matWorld);  // Transform by rotation
  Matrix_MultiplyMatrix(matWorld, matRotY, &matWorld); // Transform by rotation
  Matrix_MultiplyMatrix(matWorld, matTrans,
                        &matWorld); // Transform by translation

 

  struct Point pUp = {0, 1, 0, 1};
  lookDir.x = 0;
  lookDir.y = 0;
  lookDir.z = 1;
  lookDir.w = 1;
  struct Point target;
  Point_Add(camera, lookDir, &target);

  struct mat matCamera;
  Matrix_PointAt(camera, target, pUp, &matCamera);
  sortTri(mesh.Triangles, mesh.size);
  Matrix_QuickInverse(matCamera, &matView);

  for (int i = 0; i < THREADS; i++) {
    as[i].start = mesh.size * (i / THREADS);
    as[i].end = mesh.size * ((i + 1) / THREADS);
    pthread_create(&threads[i], NULL, drawDiv, (void *)&as[i]);
  }

  for (int j = 0; j < THREADS; j++) {
    thrd_join(threads[j], NULL);
    for (int i = 0; i < as[j].s; i++) {
      rasterTriangle(as[j].t[i], fill);
    }
  }
}
