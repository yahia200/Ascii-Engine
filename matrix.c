#include "structs.c"
#include <math.h>
float fThetax, fThetaz, fThetay;


void zeroMat(struct mat *m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m->m[i][j] = 0;
    }
  }
}

struct mat Matrix_MakeIdentity() {
  struct mat matrix;
  matrix.m[0][0] = 1.0f;
  matrix.m[1][1] = 1.0f;
  matrix.m[2][2] = 1.0f;
  matrix.m[3][3] = 1.0f;
  return matrix;
}

void Matrix_MakeRotationX(float fAngleRad, struct mat *m) {
  zeroMat(m);
  m->m[0][0] = 1.0f;
  m->m[1][1] = cosf(fAngleRad);
  m->m[1][2] = sinf(fAngleRad);
  m->m[2][1] = -sinf(fAngleRad);
  m->m[2][2] = cosf(fAngleRad);
  m->m[3][3] = 1.0f;
}

void Matrix_MakeRotationY(float fAngleRad, struct mat *m) {
  zeroMat(m);
  m->m[0][0] = cosf(fAngleRad);
  m->m[0][2] = sinf(fAngleRad);
  m->m[2][0] = -sinf(fAngleRad);
  m->m[1][1] = 1.0f;
  m->m[2][2] = cosf(fAngleRad);
  m->m[3][3] = 1.0f;
}

void Matrix_MakeRotationZ(float fAngleRad, struct mat *m) {
  zeroMat(m);
  m->m[0][0] = cosf(fAngleRad);
  m->m[0][1] = sinf(fAngleRad);
  m->m[1][0] = -sinf(fAngleRad);
  m->m[1][1] = cosf(fAngleRad);
  m->m[2][2] = 1.0f;
  m->m[3][3] = 1.0f;
}

void Matrix_MakeTranslation(float x, float y, float z, struct mat *m) {
  zeroMat(m);
  m->m[0][0] = 1.0f;
  m->m[1][1] = 1.0f;
  m->m[2][2] = 1.0f;
  m->m[3][3] = 1.0f;
  m->m[3][0] = x;
  m->m[3][1] = y;
  m->m[3][2] = z;
}

void Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear,
                           float fFar, struct mat *m) {
  zeroMat(m);
  float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
  m->m[0][0] = fAspectRatio * fFovRad;
  m->m[1][1] = fFovRad;
  m->m[2][2] = fFar / (fFar - fNear);
  m->m[3][2] = (-fFar * fNear) / (fFar - fNear);
  m->m[2][3] = 1.0f;
  m->m[3][3] = 0.0f;
}

void Matrix_MultiplyMatrix(struct mat m1, struct mat m2, struct mat *m) {
  zeroMat(m);
  for (int c = 0; c < 4; c++)
    for (int r = 0; r < 4; r++)
      m->m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] +

                   m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
}

void Matrix_QuickInverse(struct mat m, struct mat * matrix) // Only for Rotation/Translation Matrices
	{
		matrix->m[0][0] = m.m[0][0]; matrix->m[0][1] = m.m[1][0]; matrix->m[0][2] = m.m[2][0]; matrix->m[0][3] = 0.0f;
		matrix->m[1][0] = m.m[0][1]; matrix->m[1][1] = m.m[1][1]; matrix->m[1][2] = m.m[2][1]; matrix->m[1][3] = 0.0f;
		matrix->m[2][0] = m.m[0][2]; matrix->m[2][1] = m.m[1][2]; matrix->m[2][2] = m.m[2][2]; matrix->m[2][3] = 0.0f;
		matrix->m[3][0] = -(m.m[3][0] * matrix->m[0][0] + m.m[3][1] * matrix->m[1][0] + m.m[3][2] * matrix->m[2][0]);
		matrix->m[3][1] = -(m.m[3][0] * matrix->m[0][1] + m.m[3][1] * matrix->m[1][1] + m.m[3][2] * matrix->m[2][1]);
		matrix->m[3][2] = -(m.m[3][0] * matrix->m[0][2] + m.m[3][1] * matrix->m[1][2] + m.m[3][2] * matrix->m[2][2]);
		matrix->m[3][3] = 1.0f;
	}
