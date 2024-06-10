#include "matrix.c"
#include <ncurses.h>
struct Point camera = {0, 0, 0, 1};
struct Point lookDir;
void drawPoint(struct Point point, int intensity) {
  //  printw("\n%f   %f   %f\n", point.x, point.y, point.z);
  if (point.x >= 0 && point.x < MAX_WIDTH && point.y >= 0 &&
      point.y < MAX_HIEGHT) {
     mvwaddch(stdscr, (int)(point.y / 2), (int)(point.x),
     brightness[intensity]);
  }
}
void drawLine(struct Point po1, struct Point po2, int intensity) {
  struct Point p1 = {(int)po1.x, (int)po1.y, (int)po1.z};
  struct Point p2 = {(int)po2.x, (int)po2.y, (int)po2.z};
  int dx = (int)(fabsf(p2.x - p1.x)), sx = p1.x < p2.x ? 1 : -1;
  int dy = (int)(-fabsf(p2.y - p1.y)), sy = p1.y < p2.y ? 1 : -1;
  int err = dx + dy, e; /* error value e_xy */

  for (;;) {
    drawPoint(p1, intensity);
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
void Matrix_MultiplyPoint(struct mat m, struct Point i, struct Point *p) {
  p->x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
  p->y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
  p->z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
  p->w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
}
void Point_Add(struct Point v1, struct Point v2, struct Point *p) {
  p->x = v1.x + v2.x;
  p->y = v1.y + v2.y;
  p->z = v1.z + v2.z;
  p->w = 1;
}

void Point_Sub(struct Point v1, struct Point v2, struct Point *p) {
  p->x = v1.x - v2.x;
  p->y = v1.y - v2.y;
  p->z = v1.z - v2.z;
  p->w = 1;
}

void Point_Mul(struct Point v1, float k, struct Point *p) {
  p->x = v1.x * k;
  p->y = v1.y * k;
  p->z = v1.z * k;
  p->w = 1;
}

void Point_Div(struct Point v1, float k, struct Point *p) {
  p->x = v1.x / k;
  p->y = v1.y / k;
  p->z = v1.z / k;
  p->w = 1;
}

float Point_DotProduct(struct Point v1, struct Point v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Point_Length(struct Point v) { return sqrtf(Point_DotProduct(v, v)); }

struct Point Point_Normalise(struct Point v) {
  float l = Point_Length(v);
  return (struct Point){v.x / l, v.y / l, v.z / l, 1};
}

void Point_CrossProduct(struct Point v1, struct Point v2, struct Point *p) {
  p->x = v1.y * v2.z - v1.z * v2.y;
  p->y = v1.z * v2.x - v1.x * v2.z;
  p->z = v1.x * v2.y - v1.y * v2.x;
}

void Matrix_PointAt(struct Point pos, struct Point target, struct Point up,
                    struct mat *m) {
  zeroMat(m);
  // Calculate new forward direction
  struct Point newForward;
  Point_Sub(target, pos, &newForward);
  newForward = Point_Normalise(newForward);

  // Calculate new Up direction
  struct Point a, dot;

  Point_Mul(newForward, Point_DotProduct(up, newForward), &a);
  struct Point newUp;
  Point_Sub(up, a, &newUp);
  newUp = Point_Normalise(newUp);

  // New Right direction is easy, its just cross product
  struct Point newRight;
  Point_CrossProduct(newUp, newForward, &newRight);

  // Construct Dimensioning and Translation Matrix
  m->m[0][0] = newRight.x;
  m->m[0][1] = newRight.y;
  m->m[0][2] = newRight.z;
  m->m[0][3] = 0.0f;
  m->m[1][0] = newUp.x;
  m->m[1][1] = newUp.y;
  m->m[1][2] = newUp.z;
  m->m[1][3] = 0.0f;
  m->m[2][0] = newForward.x;
  m->m[2][1] = newForward.y;
  m->m[2][2] = newForward.z;
  m->m[2][3] = 0.0f;
  m->m[3][0] = pos.x;
  m->m[3][1] = pos.y;
  m->m[3][2] = pos.z;
  m->m[3][3] = 1.0f;
}

void Point_IntersectPlane(struct Point plane_p, struct Point plane_n,
                          struct Point lineStart, struct Point lineEnd,
                          struct Point *p) {
  plane_n = Point_Normalise(plane_n);
  float plane_d = -Point_DotProduct(plane_n, plane_p);
  float ad = Point_DotProduct(lineStart, plane_n);
  float bd = Point_DotProduct(lineEnd, plane_n);
  float t = (-plane_d - ad) / (bd - ad);
  struct Point lineStartToEnd;
  Point_Sub(lineEnd, lineStart, &lineStartToEnd);
  struct Point lineToIntersect;
  Point_Mul(lineStartToEnd, t, &lineToIntersect);
  Point_Add(lineStart, lineToIntersect, p);
}

float dist(struct Point p, struct Point plane_n, struct Point plane_p) {
  return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z -
          Point_DotProduct(plane_n, plane_p));
}

int Triangle_ClipAgainstPlane(struct Point plane_p, struct Point plane_n,
                              struct Triangle *in_tri,
                              struct Triangle *out_tri1,
                              struct Triangle *out_tri2) {
  // Make sure plane normal is indeed normal
  plane_n = Point_Normalise(plane_n);

  // Return signed shortest distance from point to plane, plane normal must be
  // normalised

  // Create two temporary storage arrays to classify points either side of plane
  // If distance sign is positive, point lies on "inside" of plane
  struct Point *inside_points[3];
  int nInsidePointCount = 0;
  struct Point *outside_points[3];
  int nOutsidePointCount = 0;

  // Get signed distance of each point in struct Triangle to plane
  float d0 = dist(in_tri->p[0], plane_n, plane_p);
  float d1 = dist(in_tri->p[1], plane_n, plane_p);
  float d2 = dist(in_tri->p[2], plane_n, plane_p);

  if (d0 >= 0) {
    inside_points[nInsidePointCount++] = &in_tri->p[0];
  } else {
    outside_points[nOutsidePointCount++] = &in_tri->p[0];
  }
  if (d1 >= 0) {
    inside_points[nInsidePointCount++] = &in_tri->p[1];
  } else {
    outside_points[nOutsidePointCount++] = &in_tri->p[1];
  }
  if (d2 >= 0) {
    inside_points[nInsidePointCount++] = &in_tri->p[2];
  } else {
    outside_points[nOutsidePointCount++] = &in_tri->p[2];
  }

  // Now classify struct Triangle points, and break the input struct Triangle
  // into smaller output struct Triangles if required. There are four possible
  // outcomes...

  if (nInsidePointCount == 0) {
    // All points lie on the outside of plane, so clip whole struct Triangle
    // It ceases to exist

    return 0; // No returned struct Triangles are valid
  }

  if (nInsidePointCount == 3) {
    // All points lie on the inside of plane, so do nothing
    // and allow the struct Triangle to simply pass through
    out_tri1->p[0].x = in_tri->p[0].x;
    out_tri1->p[0].y = in_tri->p[0].y;
    out_tri1->p[0].z = in_tri->p[0].z;
    out_tri1->p[0].w = in_tri->p[0].w;

    out_tri1->p[1].x = in_tri->p[1].x;
    out_tri1->p[1].y = in_tri->p[1].y;
    out_tri1->p[1].z = in_tri->p[1].z;
    out_tri1->p[1].w = in_tri->p[1].w;

    out_tri1->p[2].x = in_tri->p[2].x;
    out_tri1->p[2].y = in_tri->p[2].y;
    out_tri1->p[2].z = in_tri->p[2].z;
    out_tri1->p[2].w = in_tri->p[2].w;

    return 1; // Just the one returned original struct Triangle is valid
  }

  if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
    // Triangle should be clipped. As two points lie outside
    // the plane, the struct Triangle simply becomes a smaller struct Triangle

    // Copy appearance info to new struct Triangle

    // The inside point is valid, so keep that...
    out_tri1->p[0].x = inside_points[0]->x;
    out_tri1->p[0].y = inside_points[0]->y;
    out_tri1->p[0].z = inside_points[0]->z;
    out_tri1->p[0].w = inside_points[0]->w;

    // but the two new points are at the locations where the
    // original sides of the struct Triangle (lines) intersect with the plane
    Point_IntersectPlane(plane_p, plane_n, *inside_points[0],
                         *outside_points[0], &out_tri1->p[1]);
    Point_IntersectPlane(plane_p, plane_n, *inside_points[0],
                         *outside_points[1], &out_tri1->p[2]);

    return 1; // Return the newly formed single struct Triangle
  }

  if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
    // Triangle should be clipped. As two points lie inside the plane,
    // the clipped struct Triangle becomes a "quad". Fortunately, we can
    // represent a quad with two new struct Triangles

    // The first struct Triangle consists of the two inside points and a new
    // point determined by the location where one side of the struct Triangle
    // intersects with the plane
    out_tri1->p[0].x = inside_points[0]->x;
    out_tri1->p[0].y = inside_points[0]->y;
    out_tri1->p[0].z = inside_points[0]->z;
    out_tri1->p[0].w = inside_points[0]->w;

    out_tri1->p[1].x = inside_points[1]->x;
    out_tri1->p[1].y = inside_points[1]->y;
    out_tri1->p[1].z = inside_points[1]->z;
    out_tri1->p[1].w = inside_points[1]->w;

    Point_IntersectPlane(plane_p, plane_n, *inside_points[0],
                         *outside_points[0], &out_tri1->p[2]);

    // The second struct Triangle is composed of one of he inside points, a
    // new point determined by the intersection of the other side of the
    // struct Triangle and the plane, and the newly created point above
    out_tri2->p[0].x = inside_points[1]->x;
    out_tri2->p[0].y = inside_points[1]->y;
    out_tri2->p[0].z = inside_points[1]->z;
    out_tri2->p[0].w = inside_points[1]->w;

    out_tri2->p[1].x = out_tri1->p[2].x;
    out_tri2->p[1].y = out_tri1->p[2].y;
    out_tri2->p[1].z = out_tri1->p[2].z;
    out_tri2->p[1].w = out_tri1->p[2].w;
    Point_IntersectPlane(plane_p, plane_n, *inside_points[1],
                         *outside_points[0], &out_tri2->p[2]);

    return 2; // Return two newly formed struct Triangles which form a quad
  }
}
