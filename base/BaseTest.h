/***************************************************************************
                          BaseTest.h  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

/****************************************************************************
 * Unit testing
 ***************************************************************************/

#ifndef BASETEST_H
#define BASETEST_H

#if EM_UNIT_TEST

#include "EMath.h"
#include "Shape3D.h"
#include "Polygon.h"

#include <cppunit/TestFixture.h>
#include <cppunit/Test.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>

class EMathTest : public CppUnit::TestFixture {
 public:
  void setUp() {
  }

  void tearDown() {
  }

  void testTrigonometry() {
  }

  void testInterpolation() {
  }
 
	CPPUNIT_TEST_SUITE(EMathTest);

	CPPUNIT_TEST(testTrigonometry);
	CPPUNIT_TEST(testInterpolation);
	
	CPPUNIT_TEST_SUITE_END();
};

class ShapeTest : public CppUnit::TestFixture {
 private:
	Shape3D * shape;
	Polygon * poly;
 public:
  void setUp() {
		shape = new Shape3D();
		shape->add(0,0,0, 0,0,0,0, 0,0);
		shape->add(0,0,0, 0,0,0,0, 0,0);
		shape->add(0,0,0, 0,0,0,0, 0,0);
		shape->add(0,0,0, 0,0,0,0, 0,0);
		shape->add(0,0,0, 0,0,0,0, 0,0);
		shape->add(0,0,0, 0,0,0,0, 0,0);
		poly = new Polygon(shape);
		poly->add(1);
		poly->add(2);
		poly->add(4);
		poly->add(3);
  }

  void tearDown() {
		delete poly;
		delete shape;
  }

	void testIndex() {
		CPPUNIT_ASSERT(poly->getIndex(0) == 1);
		CPPUNIT_ASSERT(poly->getIndex(1) == 2);
		CPPUNIT_ASSERT(poly->getIndex(2) == 4);
		CPPUNIT_ASSERT(poly->getIndex(3) == 3);
	}

	void testIncludes() {
		CPPUNIT_ASSERT(poly->includes(0) == -1);
		CPPUNIT_ASSERT(poly->includes(1) == 0);
		CPPUNIT_ASSERT(poly->includes(2) == 1);
		CPPUNIT_ASSERT(poly->includes(3) == 3);
		CPPUNIT_ASSERT(poly->includes(4) == 2);
		CPPUNIT_ASSERT(poly->includes(5) == -1);
	}

	void testConnected() {
		CPPUNIT_ASSERT(poly->connected(1,2) == true);
		CPPUNIT_ASSERT(poly->connected(2,1) == true);
		CPPUNIT_ASSERT(poly->connected(2,4) == true);
		CPPUNIT_ASSERT(poly->connected(4,3) == true);
		CPPUNIT_ASSERT(poly->connected(1,3) == true);
		CPPUNIT_ASSERT(poly->connected(1,0) == false);
		CPPUNIT_ASSERT(poly->connected(0,2) == false);
		CPPUNIT_ASSERT(poly->connected(1,4) == false);
	}

	CPPUNIT_TEST_SUITE(ShapeTest);

	CPPUNIT_TEST(testIndex);
	CPPUNIT_TEST(testIncludes);
	CPPUNIT_TEST(testConnected);
	
	CPPUNIT_TEST_SUITE_END();
};

#endif //EM_UNIT_TEST

#endif // EMATH_H
