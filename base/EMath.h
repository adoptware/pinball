/***************************************************************************
                          EMath.h  -  description
                             -------------------
    begin                : Sat Jan 29 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#ifndef EMATH_H
#define EMATH_H

#define EM_ZERO(a) ( (a) > -0.000001 && (a) < 0.000001 )

#define EMathApplyMatrix(mtx, vtxIn, vtxOut) \
vtxOut.x = vtxIn.x * mtx.v[0][0] + vtxIn.y * mtx.v[0][1] + vtxIn.z * mtx.v[0][2] + mtx.t[0]; \
vtxOut.y = vtxIn.x * mtx.v[1][0] + vtxIn.y * mtx.v[1][1] + vtxIn.z * mtx.v[1][2] + mtx.t[1]; \
vtxOut.z = vtxIn.x * mtx.v[2][0] + vtxIn.y * mtx.v[2][1] + vtxIn.z * mtx.v[2][2] + mtx.t[2];

#define EMathApplyMatrixRot(mtx, vtxIn, vtxOut) \
vtxOut.x = vtxIn.x * mtx.v[0][0] + vtxIn.y * mtx.v[0][1] + vtxIn.z * mtx.v[0][2]; \
vtxOut.y = vtxIn.x * mtx.v[1][0] + vtxIn.y * mtx.v[1][1] + vtxIn.z * mtx.v[1][2]; \
vtxOut.z = vtxIn.x * mtx.v[2][0] + vtxIn.y * mtx.v[2][1] + vtxIn.z * mtx.v[2][2];

#define EM_PI   		3.1415926f
#define EM_PI_DIV_2 1.5707963f
#define EM_PI_2	    6.2831853f
#define EM_SQRT_2		1.4142135f
#define EM_SIN_45		0.7071069f
#define EM_SIN_60   0.8660254f
#define EM_SQRT_3 	1.73205f

#define EM_MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define EM_MIN(a, b) ( (a) < (b) ? (a) : (b) )

#define EM_ABS(a) ( (a) > 0 ? (a) : -(a) )

#if EM_USE_FAST_FLOAT2INT

typedef struct {
  union {
    int i;
    float f;
  };
} IntOrFloat;

extern IntOrFloat gBias;

#define ConvFloat2Int(a) \
a.f += gBias.f; \
a.i -= gBias.i;

#endif // EM_USE_FAST_FLOAT2INT


typedef struct {
  float v[3][3];
  float t[3];
} Matrix;

typedef struct {
  float x, y, z;
} Vertex3D;

typedef struct {
  float u, v;
} TexCoord;

typedef struct {
  float r, g, b, a;
} Color;

typedef struct {
  float x, y, z, w;
} Quaternion;

/** A class that collects the math functions. */
class EMath {
 public:
  EMath();
  ~EMath();

  inline static void applyMatrix(const Matrix & mtx, const Vertex3D & vtxIn, 
				 Vertex3D & vtxOut) {
    vtxOut.x = vtxIn.x * mtx.v[0][0] + vtxIn.y * mtx.v[0][1] 
      + vtxIn.z * mtx.v[0][2] + mtx.t[0];
    vtxOut.y = vtxIn.x * mtx.v[1][0] + vtxIn.y * mtx.v[1][1] 
      + vtxIn.z * mtx.v[1][2] + mtx.t[1];
    vtxOut.z = vtxIn.x * mtx.v[2][0] + vtxIn.y * mtx.v[2][1] 
      + vtxIn.z * mtx.v[2][2] + mtx.t[2];
  };
  static void applyMatrixRot(const Matrix & mtx, const Vertex3D & vtxIn, Vertex3D & vtxOut);
  static void applyMatrixTrans(const Matrix & mtx, const Vertex3D & vtxIn, Vertex3D & vtxOut);
  static void crossProduct(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut);
  // static void getCameraMatrix(Matrix & mtx, const Vertex3D & trans, Vertex3D & front, 
  // Vertex3D & up, float fov, float aspect);
  static void getTransformationMatrix(Matrix & mtx, const Vertex3D & vtxT, const Vertex3D & vtxR);
  static void getTransformationMatrix(Matrix & mtx, const Vertex3D & vtxT, const Quaternion & qRot);
  static void inverse(const Matrix & mtx, Matrix & inv);
  static void matrixMulti(const Matrix & mtxA, const Matrix & mtxB, Matrix & mtxOut);


  static float dotProduct(const Vertex3D & vtxA, const Vertex3D & vtxB);
  static float emAcos(float f);
  static float emAtan(float f);
  static float emCos(float f);
  static float emRand();
  static float emSin(float f);
  static float emSqrt(float f);
  static float emTan(float f);
  static float emPow(float x, float y);

  inline static void add(const Vertex3D & vtxA, Vertex3D & vtxOut) {
    vtxOut.x += vtxA.x;
    vtxOut.y += vtxA.y;
    vtxOut.z += vtxA.z;
  };
  inline static void add(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut) {
    vtxOut.x = vtxA.x + vtxB.x;
    vtxOut.y = vtxA.y + vtxB.y;
    vtxOut.z = vtxA.z + vtxB.z;
  };
  inline static void subst(const Vertex3D & vtxA, Vertex3D & vtxOut) {
    vtxOut.x -= vtxA.x;
    vtxOut.y -= vtxA.y;
    vtxOut.z -= vtxA.z;
  };
  inline static void subst(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut) {
    vtxOut.x = vtxA.x - vtxB.x;
    vtxOut.y = vtxA.y - vtxB.y;
    vtxOut.z = vtxA.z - vtxB.z;
  };
  inline static void scaleVertex(Vertex3D & vtx, float s) {
    vtx.x *= s; 
    vtx.y *= s;
    vtx.z *= s;
  };
  static void normalizeVector(Vertex3D & vtx);
  static float polygonZNormal(const Vertex3D & edgeA, const Vertex3D & edgeB, 
			      const Vertex3D & edgeC);

  void planeLineIntersection(const Vertex3D & nrml, float dist, 
			     const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxDiff);

  /** The projection of vxtA onto vxtB. vtxA and vxtOut is ( in this case )
   * allowed to be the same vector. */
  static float projection(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut);
  /** Get the perpendicular component of the projection */
  static float perpendicular(const Vertex3D & vtxA, const Vertex3D & vtxB, Vertex3D & vtxOut);
  /** Counts the "reflection" vector of vtxIn onto a plane with the normal vtxWall.
   * vtxIn and vtxOut is ( in this case ) allowed to be the same vector. */
  static void reflection(const Vertex3D & vtxIn, const Vertex3D & vtxWall, 
			 Vertex3D & vtxOut, bool bBehind = false);
  /** Counts the "reflection" vector of vtxIn onto a plane with the normal vtxWall.
   * vtxIn and vtxOut is ( in this case ) allowed to be the same vector. The damping
   * factor is 1 for normal, 0 for max damping and over 1 for extra bounce. The wall
   * factor is as if the wall would give a little push, 0 for normal. Scale is the
   * length of the vector, 1 for normal. */
  static void reflectionDamp(const Vertex3D & vtxIn, const Vertex3D & vtxWall, 
			     Vertex3D & vtxOut, float damp, float extra, 
			     float scale, bool bBehind = false);
  static void scaleVector(Vertex3D & vtx, float sc);
  inline static float vectorLength(const Vertex3D & vtx) {
    return EMath::emSqrt(vtx.x * vtx.x + vtx.y * vtx.y + vtx.z * vtx.z);
  };
  /** The vector length but without the square root */
  inline static float vectorLengthSqr(const Vertex3D & vtx) {
    return (vtx.x * vtx.x + vtx.y * vtx.y + vtx.z * vtx.z);
  };

  static float volume(const Vertex3D & vtxA, const Vertex3D & vtxB, const Vertex3D & vtxC);
  static void rotationArc(const Vertex3D & vtxA, const Vertex3D & vtxB, Quaternion & vtxOut);
  static void quaternionMulti(const Quaternion & qA, const Quaternion & qB, Quaternion & qOut);
  /** Quadratic interpolation. f0 is at t=0, f1 is at t=1 and f2 at t=2. You
   * may give any t for recieving values. */
  static float quadratic(float f0, float f1, float f2, float t);
  static float cubic(float f0, float f1, float f2, float f3, float t);

  static const Matrix identityMatrix;
};

#endif // EMATH_H
