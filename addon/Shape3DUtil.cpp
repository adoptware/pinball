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

#define EM_READ(file, str) if (!(file >> str)) throw string("EOF reached too soon");

#define EM_READ_CMP(file, str, cmp) \
	if (!(file >> str)) {             \
		throw string("EOF reached too soon");   \
	}                                 \
	if (str != cmp) {                 \
		throw string("Parse error, unexpected token " + str + ", expected " + cmp); \
	}

void Shape3DUtil::readUnknown(ifstream & file) {
	EM_COUT("loading unknown", 0);
	string str;
	EM_READ_CMP(file, str, "{");

	int brackets = 1;
	while (brackets > 0) {
		EM_READ(file, str);
		if (str == "{") brackets++;
		if (str == "}") brackets--;
	}
}

void Shape3DUtil::readPolygon(ifstream & file, Shape3D* shape) {
	EM_COUT("loading poly", 0);
	string str;
	Polygon * poly = new Polygon(shape);

	EM_READ_CMP(file, str, "{");

	EM_READ(file, str);
	while (str != "}") {
		if (str == "tpt") {
			EM_COUT("loading transparent", 1);
			Shape3DUtil::readUnknown(file);
			poly->setProperty(EM_POLY_TRANS);
			shape->setProperty(EM_SHAPE3D_USE_TRANS);
		} else if (str == "dbl") {
			EM_COUT("loading double sided", 1);
			Shape3DUtil::readUnknown(file);
			poly->setProperty(EM_POLY_DOUBLE);
		} else if (str == "ccw") {
			EM_COUT("loading double sided", 1);
			Shape3DUtil::readUnknown(file);
			poly->setProperty(EM_POLY_CCW_VIEW);
		} else if (str == "flt") {
			EM_COUT("loading flat", 1);
			Shape3DUtil::readUnknown(file);
			poly->setProperty(EM_POLY_FLAT);
		} else if (str == "ple") {
			Shape3DUtil::readPolygonEdge(file, poly);
		} else {
			Shape3DUtil::readUnknown(file);
		}
		EM_READ(file, str);
	}
	shape->add(poly);
}

void Shape3DUtil::readPolygonEdge(ifstream & file, Polygon* poly) {
	EM_COUT("loading polyedge", 0);
	string str;
	int i;
	float u, v, a, r, g, b;

	EM_READ_CMP(file, str, "{");

	EM_READ(file, i);
	EM_READ(file, u);
	EM_READ(file, v);
	EM_READ(file, r);
	EM_READ(file, g);
	EM_READ(file, b);
	EM_READ(file, a);

	EM_READ_CMP(file, str, "}");

	poly->add(i, u, v, r, g, b, a);

}

void Shape3DUtil::readVertex(ifstream & file, Shape3D* shape) {
	EM_COUT("loading vertex", 0);
	float x, y, z;
	string str;

	EM_READ_CMP(file, str, "{");

	EM_READ(file, x);
	EM_READ(file, y);
	EM_READ(file, z);

	EM_READ_CMP(file, str, "}");

	shape->add(x, y, z);
}

void Shape3DUtil::readShape3D(ifstream & file, Shape3D* shape) {
	EM_COUT("loading shape", 0);

	string str;
	EM_READ(file, str);
	while (str != "{") {
		EM_READ(file, str);
	}

	EM_READ(file, str);
	while (str != "}") {
		if (str == "vtx") {
			Shape3DUtil::readVertex(file, shape);
		} else if (str == "ply") {
			Shape3DUtil::readPolygon(file, shape);
// 		} else if (str == "tpt") {
// 			Shape3DUtil::readTrans(file, shape);
// 		} else if (str == "dbl") {
// 			Shape3DUtil::readDouble(file, shape);
// 		} else if (str == "flt") {
// 			Shape3DUtil::readFlat(file, shape);
		} else {
			Shape3DUtil::readUnknown(file);
		}
		EM_READ(file, str);
	}
}

Shape3D* Shape3DUtil::loadShape3D(const char* filename) {
	Shape3D * shape = NULL;
	ifstream file(filename);
	try {
		if (!file) {
			throw string("Shape3DUtil::load() : file not found");
		}
		EM_COUT("Shape3DUtil::load() : file found" << filename, 1);
		
		string str;
		file >> str;
		while(file) {
			if (str == "shp") {
				EM_COUT("Shape3DUtil::load() : loading shape", 0);
				shape = new Shape3D();
				Shape3DUtil::readShape3D(file, shape);
				shape->countNormals();
			}
			if (file) file >> str;
		}
	} catch (string str) {
		cerr << str << endl;
		cerr << "When loading file: " << filename << endl;
	}
	return shape;
}
