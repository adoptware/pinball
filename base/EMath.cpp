/***************************************************************************
                          EMath.cpp  -  description
                             -------------------
    begin                : Sat Jan 29 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include <math.h>
#include "EMath.h"

Matrix identityMatrix =
{
	{
  	/* 3x3 identity */
   	{ 1.0, 0.0, 0.0 },
   	{ 0.0, 1.0, 0.0 },
   	{ 0.0, 0.0, 1.0 },
  },
	/* zero translation */
	{ 0.0, 0.0, 0.0 }
};

EMath::EMath() {
}

EMath::~EMath() {
}

void EMath::applyMatrix(const Matrix & mtx, const Vertex3D & vtxIn, Vertex3D & vtxOut) {
	vtxOut.x = vtxIn.x * mtx.v[0][0] + vtxIn.y * mtx.v[0][1] + vtxIn.z * mtx.v[0][2] + mtx.t[0];
	vtxOut.y = vtxIn.x * mtx.v[1][0] + vtxIn.y * mtx.v[1][1] + vtxIn.z * mtx.v[1][2] + mtx.t[1];
	vtxOut.z = vtxIn.x * mtx.v[2][0] + vtxIn.y * mtx.v[2][1] + vtxIn.z * mtx.v[2][2] + mtx.t[2];
}

void EMath::applyMatrixRot(const Matrix & mtx, const Vertex3D & vtxIn, Vertex3D & vtxOut) {
	vtxOut.x = vtxIn.x * mtx.v[0][0] + vtxIn.y * mtx.v[0][1] + vtxIn.z * mtx.v[0][2];
	vtxOut.y = vtxIn.x * mtx.v[1][0] + vtxIn.y * mtx.v[1][1] + vtxIn.z * mtx.v[1][2];
	vtxOut.z = vtxIn.x * mtx.v[2][0] + vtxIn.y * mtx.v[2][1] + vtxIn.z * mtx.v[2][2];
}

void EMath::applyMatrixTrans(const Matrix & mtx, const Vertex3D & vtxIn, Vertex3D & vtxOut) {
	vtxOut.x = mtx.t[0];
	vtxOut.y = mtx.t[1];
	vtxOut.z = mtx.t[2];
}

void EMath::crossProduct(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut) {
	vtxOut.x = (vtxA.y * vtxB.z) - (vtxA.z * vtxB.y);
	vtxOut.y = (vtxA.z * vtxB.x) - (vtxA.x * vtxB.z);
	vtxOut.z = (vtxA.x * vtxB.y) - (vtxA.y * vtxB.x);
}

float EMath::dotProduct(const Vertex3D & vtxA, const Vertex3D & vtxB) {
	return vtxA.x * vtxB.x + vtxA.y * vtxB.y + vtxA.z * vtxB.z;
}

float EMath::emAcos(float f) {
	return (float) acos(EM_PI_2*f);
}

float EMath::emAtan(float f) {
	return (float)atan(EM_PI_2*f);
}

float EMath::emCos(float f) {
	return (float)cos(EM_PI_2*f);
}

float EMath::emRand() {
	return ((float)(rand()-(RAND_MAX/2))/(RAND_MAX/2));
}

float EMath::emSin(float f) {
	return (float)sin(EM_PI_2*f);
}

float EMath::emSqrt(float f) {
	return (float)sqrt(f);
}

float EMath::emTan(float f) {
	return (float)tan(EM_PI_2*f);
}

float EMath::emPow(float x, float y) {
	return (float)pow(x, y);
}

/*
 */
void EMath::getTransformationMatrix(Matrix & mtx, const Vertex3D & vtxT, const Vertex3D & vtxR) {
	float sin_x = EMath::emSin(vtxR.x);
  float cos_x = EMath::emCos(vtxR.x);

  float sin_y = EMath::emSin(vtxR.y);
  float cos_y = EMath::emCos(vtxR.y);

  float sin_z = EMath::emSin(vtxR.z);
  float cos_z = EMath::emCos(vtxR.z);

  float sinx_siny = sin_x * sin_y;
  float cosx_siny = cos_x * sin_y;

  mtx.v[0][0] = cos_y * cos_z;
  mtx.v[0][1] = cos_y * sin_z;
  mtx.v[0][2] = -sin_y;

  mtx.v[1][0] = (sinx_siny * cos_z) - (cos_x * sin_z);
  mtx.v[1][1] = (sinx_siny * sin_z) + (cos_x * cos_z);
  mtx.v[1][2] = sin_x * cos_y;

  mtx.v[2][0] = (cosx_siny * cos_z) + (sin_x * sin_z);
  mtx.v[2][1] = (cosx_siny * sin_z) - (sin_x * cos_z);
  mtx.v[2][2] = cos_x * cos_y;

  mtx.t[0] = vtxT.x;
  mtx.t[1] = vtxT.y;
  mtx.t[2] = vtxT.z;
}

/*
 */
void EMath::inverse(const Matrix & mtx, Matrix & inv) {
	Matrix mtxTmp = mtx;
	inv = identityMatrix;

  unsigned int cc;
  unsigned int rowMax; // Points to max abs value row in this column
  unsigned int row;
  float tmp;

  // Go through columns
  for (int c=0; c<3; c++) {
    // Find the row with max value in this column
    rowMax = c;
    for (int r=c+1; r<3; r++) {
      if (fabs(mtxTmp.v[c][r]) > fabs(mtxTmp.v[c][rowMax])) {
        rowMax = r;
      }
    }

    // If the max value here is 0, we can't invert.  Return identity.
    if (mtx.v[rowMax][c] == 0.0f) {
			inv = identityMatrix;
      return;
		}

    // Swap row "rowMax" with row "c"
    for (cc=0; cc<3; cc++) {
      tmp = mtxTmp.v[cc][c];
      mtxTmp.v[cc][c] = mtxTmp.v[cc][rowMax];
      mtxTmp.v[cc][rowMax] = tmp;
      tmp = inv.v[cc][c];
      inv.v[cc][c] = inv.v[cc][rowMax];
      inv.v[cc][rowMax] = tmp;
    }

    // Now everything we do is on row "c".
    // Set the max cell to 1 by dividing the entire row by that value
    tmp = mtxTmp.v[c][c];
    for (cc=0; cc<3; cc++) {
      mtxTmp.v[cc][c] /= tmp;
      inv.v[cc][c] /= tmp;
    }

    // Now do the other rows, so that this column only has a 1 and 0's
    for (row = 0; row < 3; row++) {
      if (row != c) {
        tmp = mtxTmp.v[c][row];
        for (cc=0; cc<3; cc++) {
          mtxTmp.v[cc][row] -= mtxTmp.v[cc][c] * tmp;
          inv.v[cc][row] -= inv.v[cc][c] * tmp;
        }
      }
    }

  }

/*
	for (int a=0; a<3; a++) {
		for (int b=0; b<3; b++) {
			inv.v[a][b] = -mtx.v[a][b];
		}
	}*/

	inv.t[0] = -mtx.t[0];
	inv.t[1] = -mtx.t[1];
	inv.t[2] = -mtx.t[2];
}

/* mtxA or mtxB NOT allowed to be the same as out.
 */
void EMath::matrixMulti(const Matrix & mtxA, const Matrix & mtxB, Matrix & mtxOut) {
  for (int a=0; a<3; a++) {
  	for (int b=0; b<3; b++) {
	 		mtxOut.v[a][b] =
				(mtxA.v[0][b] * mtxB.v[a][0]) +
				(mtxA.v[1][b] * mtxB.v[a][1]) +
				(mtxA.v[2][b] * mtxB.v[a][2]);
    }

    mtxOut.t[a] =
			(mtxA.t[0] * mtxB.v[a][0]) +
			(mtxA.t[1] * mtxB.v[a][1]) +
		  (mtxA.t[2] * mtxB.v[a][2]) +
		  mtxB.t[a];
	}
}


/*
 */
void EMath::normalizeVector(Vertex3D & vtx) {
	float length_1;
	float length = EMath::vectorLength(vtx);
	if (EM_ZERO(length)) return;
	length_1 = 1.0 / length;
	vtx.x *= length_1;
	vtx.y *= length_1;
	vtx.z *= length_1;
}

/*
 */
void EMath::scaleVector(Vertex3D & vtx, float sc) {
	vtx.x *= sc;
	vtx.y *= sc;
	vtx.z *= sc;
}

/* Projection of vector A onto B.
 */
float EMath::projection(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut) {
	float k =(vtxA.x * vtxB.x  +  vtxA.y * vtxB.y  +  vtxA.z * vtxB.z) /
		(vtxB.x * vtxB.x  +  vtxB.y * vtxB.y  +  vtxB.z * vtxB.z);

		vtxOut.x = k * vtxB.x;
		vtxOut.y = k * vtxB.y;
		vtxOut.z = k * vtxB.z;

		return k;
}

/*
 */
float EMath::polygonZNormal(const Vertex3D & edgeA, const Vertex3D & edgeB, const Vertex3D & edgeC) {
	return ((edgeB.x - edgeA.x) * (edgeC.y - edgeB.y)) - ((edgeC.x - edgeB.x) * (edgeB.y - edgeA.y));
}


/* First the projection of vtxIn onto vtxWall is counted.
 * the reflection is then vtxOut = vtxIn - 2*vtxPro.
 * If k > 0 the vtxIn vector is comming from behind the wall and
 * no reflection is made.
 */
void EMath::reflection(const Vertex3D & vtxIn, const Vertex3D & vtxWall, Vertex3D & vtxOut, bool bBehind) {
	Vertex3D vtxPro;
	float k =(vtxIn.x * vtxWall.x  +  vtxIn.y * vtxWall.y  +  vtxIn.z * vtxWall.z) /
		(vtxWall.x * vtxWall.x  +  vtxWall.y * vtxWall.y  +  vtxWall.z * vtxWall.z);

	if ( k < 0 || bBehind)	{
		vtxPro.x = k * vtxWall.x;
		vtxPro.y = k * vtxWall.y;
		vtxPro.z = k * vtxWall.z;

		vtxOut.x = - vtxPro.x - vtxPro.x + vtxIn.x;
		vtxOut.y = - vtxPro.y - vtxPro.y + vtxIn.y;
		vtxOut.z = - vtxPro.z - vtxPro.z + vtxIn.z;
	}
}

/* First the projection of vtxIn onto vtxWall is counted.
 * the reflection is then vtxOut = vtxIn - 2*vtxPro.
 * If k > 0 the vtxIn vector is comming from behind the wall and
 * no reflection is made.
 */
void EMath::reflectionDamp(const Vertex3D & vtxIn, const Vertex3D & vtxWall, Vertex3D & vtxOut, float damp, float extra, float scale, bool bBehind) {
	Vertex3D vtxPro;
	float k =(vtxIn.x * vtxWall.x  +  vtxIn.y * vtxWall.y  +  vtxIn.z * vtxWall.z) /
		(vtxWall.x * vtxWall.x  +  vtxWall.y * vtxWall.y  +  vtxWall.z * vtxWall.z);

	if ( k < 0 || bBehind) {
		vtxPro.x = k * vtxWall.x;
		vtxPro.y = k * vtxWall.y;
		vtxPro.z = k * vtxWall.z;

		vtxOut.x = (vtxIn.x - vtxPro.x - vtxPro.x * damp + vtxWall.x * extra) * scale;
		vtxOut.y = (vtxIn.y - vtxPro.y - vtxPro.y * damp + vtxWall.y * extra) * scale;
		vtxOut.z = (vtxIn.z - vtxPro.z - vtxPro.z * damp + vtxWall.z * extra) * scale;
	}
}

/*
 */
float EMath::vectorLength(const Vertex3D & vtx) {
	return EMath::emSqrt(vtx.x * vtx.x + vtx.y * vtx.y + vtx.z * vtx.z);
}

/*
 */
float EMath::volume(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC) {
	return vtxA.x*( vtxB.y*vtxC.z - vtxB.z*vtxC.y ) - vtxA.y*( vtxB.x*vtxC.z - vtxB.z*vtxC.x ) + vtxA.z*( vtxB.x*vtxC.y - vtxB.y*vtxC.x );
}
