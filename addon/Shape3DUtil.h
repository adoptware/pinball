/***************************************************************************
                          S3DUtil.h  -  description
                             -------------------
    begin                : Thu Jan 27 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef SHAPE3DUTIL_H
#define SHAPE3DUTIL_H

#include <fstream>
#include <iostream>
#include <string>

class Shape3D;
class Polygon;

/** <p>Loads a Shape3D object from a "emi" file. </p>
 */

class Shape3DUtil {
 public:
	/** <p>loadShape3D( filename ). Creates a new Shape3D object.</p> */
	static Shape3D* loadShape3D(const char *);

 private:
	static void readShape3D(ifstream & file, Shape3D* shape);
	static void readPolygon(ifstream & file, Shape3D* shape);
	static void readPolygonEdge(ifstream & file, Polygon* poly);
	static void readVertex(ifstream & file, Shape3D* shape);
	static void readUnknown(ifstream & file);
};

#endif // SHAPE3DUTIL_H
