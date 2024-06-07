#include "structs.c"
#include <math.h>
float fThetax, fThetaz, fThetay;
float matProj[4][4], matRotZ[4][4], matRotX[4][4], matRotY[4][4];
void initMat() {
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)MAX_HIEGHT / (float)MAX_WIDTH;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
  matProj[0][0] = fAspectRatio * fFovRad;
  matProj[1][1] = fFovRad;
  matProj[2][2] = fFar / (fFar - fNear);
  matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj[2][3] = 1.0f;
  matProj[3][3] = 0.0f;
}

void rotate() {
  matRotZ[0][0] = cosf(fThetaz);
  matRotZ[0][1] = sinf(fThetaz);
  matRotZ[1][0] = -sinf(fThetaz);
  matRotZ[1][1] = cosf(fThetaz);
  matRotZ[2][2] = 1;
  matRotZ[3][3] = 1;

  matRotX[0][0] = 1;
  matRotX[1][1] = cosf(fThetax * 0.5f);
  matRotX[1][2] = sinf(fThetax * 0.5f);
  matRotX[2][1] = -sinf(fThetax * 0.5f);
  matRotX[2][2] = cosf(fThetax * 0.5f);
  matRotX[3][3] = 1;

  matRotY[0][0] = cosf(fThetay);
  matRotY[0][2] = -sinf(fThetay);
  matRotY[2][0] = sinf(fThetay);
  matRotY[1][1] = 1.0f;
  matRotY[2][2] = cosf(fThetay);
  matRotY[3][3] = 1.0f;

  fThetax += 0.0;
  fThetaz += 0.0;
  fThetay += 0.01;
}

