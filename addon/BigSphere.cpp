/***************************************************************************
                          BigSphere.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "BigSphere.h"
#include "Polygon.h"

#define MEDIAN(v1, v2, vo) \
  vo.x = v1.x + (v2.x - v1.x)/2; \
  vo.y = v1.y + (v2.y - v1.y)/2; \
  vo.z = v1.z + (v2.z - v1.z)/2;

BigSphere::BigSphere(float fSize, int level, float fR, float fG, float fB, float fA) : Shape3D(1, 1) {
	// top vertex
	this->add(0, -1*fSize, 0, 
						fR, fG, fB, fA,
						0.0f, 0.0f);

	// front left
	this->add(-EM_SIN_45*fSize, 0, EM_SIN_45*fSize,
						fR, fG, fB, fA,
						0.0f, 0.0f);
	// front right
	this->add(EM_SIN_45*fSize, 0, EM_SIN_45*fSize,
						fR, fG, fB, fA,
						0.0f, 0.0f);
	// back left
	this->add(EM_SIN_45*fSize, 0, -EM_SIN_45*fSize,
						fR, fG, fB, fA,
						0.0f, 0.0f);
	// back right
	this->add(-EM_SIN_45*fSize, 0, -EM_SIN_45*fSize,
						fR, fG, fB, fA,
						0.0f, 0.0f);
	// bottom vertex
	this->add(0, 1*fSize, 0,
						fR, fG, fB, fA,
						0.0f, 0.0f);

	// TODO: Check if this causes a memory leak!!!
	vector<int> triangles;

	// push back first eight triangles
	triangles.push_back(0);
	triangles.push_back(1);
	triangles.push_back(2);

	triangles.push_back(0);
	triangles.push_back(2);
	triangles.push_back(3);

	triangles.push_back(0);
	triangles.push_back(3);
	triangles.push_back(4);

	triangles.push_back(0);
	triangles.push_back(4);
	triangles.push_back(1);

	triangles.push_back(5);
	triangles.push_back(2);
	triangles.push_back(1);

	triangles.push_back(5);
	triangles.push_back(3);
	triangles.push_back(2);

	triangles.push_back(5);
	triangles.push_back(4);
	triangles.push_back(3);

	triangles.push_back(5);
	triangles.push_back(1);
	triangles.push_back(4);

	int start = 0;
	int end = triangles.size();

	// split each triangle into three triangles
	while (level-- > 0) {
		for (int a = start; a < end; a += 3) {
			int A = triangles[a];
			int B = triangles[a+1];
			int C = triangles[a+2];
			Vertex3D vtxA = *(this->getVertex3D(triangles[a]));
			Vertex3D vtxB = *(this->getVertex3D(triangles[a+1]));
			Vertex3D vtxC = *(this->getVertex3D(triangles[a+2]));
			
			// new vertices
			Vertex3D vtxD, vtxE, vtxF;
			
			MEDIAN(vtxA, vtxB, vtxD);
			MEDIAN(vtxB, vtxC, vtxE);
			MEDIAN(vtxC, vtxA, vtxF);
			
			// move vertices out to the sphere surface
			EMath::normalizeVector(vtxD);
			EMath::normalizeVector(vtxE);
			EMath::normalizeVector(vtxF);
			EMath::scaleVector(vtxD, fSize);
			EMath::scaleVector(vtxE, fSize);
			EMath::scaleVector(vtxF, fSize);
			
			EM_COUT("BigSphere::BigSphere() vtxA " << vtxA.x <<" "<< vtxA.y <<" "<< vtxA.z, 0);
			EM_COUT("BigSphere::BigSphere() vtxB " << vtxB.x <<" "<< vtxB.y <<" "<< vtxB.z, 0);
			EM_COUT("BigSphere::BigSphere() vtxC " << vtxC.x <<" "<< vtxC.y <<" "<< vtxC.z, 0);
			
			EM_COUT("BigSphere::BigSphere() vtxD " << vtxD.x <<" "<< vtxD.y <<" "<< vtxD.z, 0);
			EM_COUT("BigSphere::BigSphere() vtxE " << vtxE.x <<" "<< vtxE.y <<" "<< vtxE.z, 0);
			EM_COUT("BigSphere::BigSphere() vtxF " << vtxF.x <<" "<< vtxF.y <<" "<< vtxF.z, 0);
			
			int D, E, F;
			if ((D = this->find(vtxD.x, vtxD.y, vtxD.z, 0.001)) == -1) {
				D = this->add(vtxD.x, vtxD.y, vtxD.z,
											fR, fG, fB, fA,	0.0f, 0.0f);
			}
			if ((E = this->find(vtxE.x, vtxE.y, vtxE.z, 0.001)) == -1) {
				E = this->add(vtxE.x, vtxE.y, vtxE.z,
											fR, fG, fB, fA,	0.0f, 0.0f);
			}
			if ((F = this->find(vtxF.x, vtxF.y, vtxF.z, 0.001)) == -1) {
				F = this->add(vtxF.x, vtxF.y, vtxF.z,
											fR, fG, fB, fA,	0.0f, 0.0f);
			}
			
			// push back four new triangles
			triangles.push_back(A);
			triangles.push_back(D);
			triangles.push_back(F);
			
			triangles.push_back(D);
			triangles.push_back(B);
			triangles.push_back(E);
			
			triangles.push_back(F);
			triangles.push_back(E);
			triangles.push_back(C);
			
			triangles.push_back(D);
			triangles.push_back(E);
			triangles.push_back(F);
		}
		// discard the old triangles
		start = end;
		end = triangles.size();
	}

	// add the triangles to the shape
	for (int a = start; a < end; a +=  3) {
		Polygon * p;
		p = new Polygon(this, 3);
		p->add(triangles[a]);
		p->add(triangles[a+1]);
		p->add(triangles[a+2]);
		this->add(p);
	}

	this->countNormals();
}
