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
	EM_COUT("loading unknown", 0);
	string str;
	file >> str; if (str != "{") throw string("parse error");

	int brackets = 1;
	while (brackets > 0) {
		file >> str;
		if (str == "{") brackets++;
		if (str == "}") brackets--;
	}
}

void Shape3DUtil::readTrans(ifstream & file, Shape3D* shape) {
	EM_COUT("loading transparent", 1);
	string str;
	file >> str; 
	if (str != "{") throw string("parse error");
	file >> str; 
	if (str != "}") throw string("parse error");
	shape->setProperty(EM_SHAPE3D_TRANS);
}

void Shape3DUtil::readDouble(ifstream & file, Shape3D* shape) {
	EM_COUT("loading double sided", 1);
	string str;
	file >> str; 
	if (str != "{") throw string("parse error");
	file >> str; 
	if (str != "}") throw string("parse error");
	shape->setProperty(EM_SHAPE3D_DOUBLE);
}

void Shape3DUtil::readFlat(ifstream & file, Shape3D* shape) {
	EM_COUT("loading flat", 1);
	string str;
	file >> str; 
	if (str != "{") throw string("parse error");
	file >> str; 
	if (str != "}") throw string("parse error");
	shape->setProperty(EM_SHAPE3D_FLAT);
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
	THROW_EOF(file); file >> r;
	THROW_EOF(file); file >> g;
	THROW_EOF(file); file >> b;
	THROW_EOF(file); file >> a;

	THROW_EOF(file); file >> str; if (str != "}") throw string("parse error");

	poly->add(i, u, v, r, g, b, a);

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
		} else if (str == "tpt") {
			Shape3DUtil::readTrans(file, shape);
		} else if (str == "dbl") {
			Shape3DUtil::readDouble(file, shape);
		} else if (str == "flt") {
			Shape3DUtil::readFlat(file, shape);
		} else {
			Shape3DUtil::readUnknown(file);
		}
		THROW_EOF(file); file >> str;
	}
}

Shape3D* Shape3DUtil::loadShape3D(const char* filename) {
	int a;
	Shape3D * shape = NULL;
	ifstream file(filename);
	try {
		if (!file) {
			throw string("Shape3DUtil::load() : file not found");
		}
		EM_COUT("Shape3DUtil::load() : file found", 1);
		
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
