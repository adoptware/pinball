/***************************************************************************
                          S3DEmi.cpp  -  description
                             -------------------
    begin                : Thu Jan 27 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "Shape3DUtil.h"
#include "Polygon.h"
#include "Private.h"
#include "Shape3D.h"

#define THROW_EOF(file) if (!file) throw string("eof reached too soon");


void Shape3DUtil::readUnknown(ifstream & file) {
	EM_COUT("loading unknown", 1);
	string str;
	file >> str; if (str != "{") throw string("parse error");

	int brackets = 1;
	while (brackets > 0) {
		file >> str;
		if (str == "{") brackets++;
		if (str == "}") brackets--;
	}
}

void Shape3DUtil::readPolygon(ifstream & file, Shape3D* shape) {
	EM_COUT("loading poly", 0);
	string str;
	Polygon * poly = new Polygon(shape);

	THROW_EOF(file); file >> str; if (str != "{") throw string("parse error");

	THROW_EOF(file); file >> str;
	while (str != "}") {
		if (str == "ple") {
			Shape3DUtil::readPolygonEdge(file, poly);
		} else {
			Shape3DUtil::readUnknown(file);
		}

		THROW_EOF(file); file >> str;
	}
	shape->add(poly);
}

void Shape3DUtil::readPolygonEdge(ifstream & file, Polygon* poly) {
	EM_COUT("loading polyedge", 0);
	string str;
	int i;
	float u, v, a, r, g, b;

	THROW_EOF(file); file >> str; if (str != "{") throw string("parse error");
	
	THROW_EOF(file); file >> i;
	THROW_EOF(file); file >> u;
	THROW_EOF(file); file >> v;
	THROW_EOF(file); file >> a;
	THROW_EOF(file); file >> r;
	THROW_EOF(file); file >> g;
	THROW_EOF(file); file >> b;

	THROW_EOF(file); file >> str; if (str != "}") throw string("parse error");

	poly->add(i, u, v, a, r, g, b);

}

void Shape3DUtil::readVertex(ifstream & file, Shape3D* shape) {
	EM_COUT("loading vertex", 0);
	float x, y, z;
	string str;

	THROW_EOF(file); file >> str; if (str != "{") throw string("parse error");
	
	THROW_EOF(file); file >> x;
	THROW_EOF(file); file >> y;
	THROW_EOF(file); file >> z;

	THROW_EOF(file); file >> str; if (str != "}") throw string("parse error");

	shape->add(x, y, z);
}

void Shape3DUtil::readShape3D(ifstream & file, Shape3D* shape) {
	EM_COUT("loading shape", 0);

	string str;
	THROW_EOF(file); file >> str;
	while (str != "{") {
		THROW_EOF(file); file >> str;
	}

	THROW_EOF(file); file >> str;
	while (str != "}") {
		if (str == "vtx") {
			Shape3DUtil::readVertex(file, shape);
		} else if (str == "ply") {
			Shape3DUtil::readPolygon(file, shape);
		} else {
			Shape3DUtil::readUnknown(file);
		}
		THROW_EOF(file); file >> str;
	}
}

Shape3D* Shape3DUtil::loadShape3D(const char* fileName) {
	int a;
	Shape3D * shape = NULL;
	ifstream file(fileName);
	try {
		if (!file) {
			throw string("Shape3DUtil::load() : file not found: ");
		}
		cerr << "Shape3DUtil::load() : file found" << endl;
		
		string str;
		file >> str;
		while(file) {
			if (str == "shp") {
				EM_COUT("Shape3DUtil::load() : loading shape", 0);
				shape = new Shape3D();
				Shape3DUtil::readShape3D(file, shape);
			}
			if (file) file >> str;
		}
	} catch (string str) {
		cerr << str << endl;
	}
	return shape;
}

Shape3D* Shape3DUtil::loadShape3Dold(const char* fileName) {
	int a;
	ifstream file(fileName);
	if (!file) {
		cerr << "Shape3DUtil::loadShape3D() file not found: \"" << fileName << "\"" << endl;
		return NULL;
	}
	cerr << "Shape3DUtil::loadShape3D() file found" << endl;
	
	string str1;
	string str2;
	
	file >> str1;
	file >> str2;
	if (str1 != "emilia" || str2 != "shape3d") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'emilia shape3d' missing", 10);
		return NULL;
	}
		
	file >> str1;
	if (str1 != "vertices") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'vertices' missing", 10);
		return NULL;
	}
	
	int tiVertices;
	if (! (file >> tiVertices)) {
		EM_COUT("Shape3DUtil::loadShape3D() : vertices count missing", 10);
		return NULL;
	}
	
	file >> str1;
	if (str1 != "polygons") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'polygons' missing", 10);
		return NULL;
	}
	
	int tiPolygons;
	if (! (file >> tiPolygons)) {
		EM_COUT("Shape3DUtil::loadShape3D() : polygon count missing", 10);
		return NULL;
	}
	
	Shape3D* toShape3D = new Shape3D(tiVertices, tiPolygons);
	EM_COUT("Shape3DUtil::loadShape3D() new Shape3D(" << tiVertices <<", "<< tiPolygons <<")"
					, 10);
	
	file >> str1;
	file >> str2;
	if (str1 != "start" || str2 != "vertices") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'start vertices' missing", 10);
		return NULL;
	}
	
	for (a=0; a<tiVertices; a++) {
		float x, y, z;
		file >> x;
		file >> y;
		file >> z;
		toShape3D->add(x, y, z);
	}
	
	file >> str1;
	file >> str2;
	if (str1 != "end" || str2 != "vertices") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'end vertices' missing", 10);
		return NULL;
	}
	
	file >> str1;
	file >> str2;
	if (str1 != "start" || str2 != "polygons") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'start polygons' missing", 10);
		return NULL;
	}
	
  for (a=0; a<tiPolygons; a++) {
   	int tiEdges;
   	file >> tiEdges;
   	Polygon * toPoly = new Polygon(toShape3D, tiEdges);
   	
   	file >> str1;
 		file >> str2;
   	if (str1 != "texture") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'texture' missing", 10);
		return NULL;
   	}
  		// TODO: textures.
   	EM_COUT("added polygon ", 0);
   	for (int b=0; b<tiEdges; b++) {
   		int i;
			float u, v;
   		float a, r, g, b;
   		file >> i;
   		file >> u;
   		file >> v;
   		file >> a;
   		file >> r;
   		file >> g;
   		file >> b;
			toPoly->add(i, u, v, a, r, g, b);
			EM_COUT(i << " ", 0);
   	}
   	toShape3D->add(toPoly);
  	EM_COUT(endl, 0);
  }
	
	file >> str1;
	file >> str2;
	if (str1 != "end" || str2 != "polygons") {
		EM_COUT("Shape3DUtil::loadShape3D() : 'end polygons' missing", 10);
		return NULL;
	}
	
	toShape3D->countNormals();
	EM_COUT("Shape3DUtil::loadShape3D() Done." << endl, 0);
	return toShape3D;
}
