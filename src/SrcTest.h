/***************************************************************************
                            SrcTest.h -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

/****************************************************************************
 * Unit testing
 ***************************************************************************/

#ifndef SRCTEST_H
#define SRCTEST_H

#if EM_UNIT_TEST

#include <fstream>
#include <sstream>
#include <string>

#include "Config.h"
#include "Loader.h"
#include "Script.h"
#include "Group.h"
#include "StateBehavior.h"

#include <cppunit/TestFixture.h>
#include <cppunit/Test.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>

class ScriptTest : public CppUnit::TestFixture {
 public:
  void setUp() {
  }

  void tearDown() {
  }

  void testVariable() {
		Script * script = new Script();
		CPPUNIT_ASSERT(script->getVariable(1) == 0);
		CPPUNIT_ASSERT(script->getVariable(3) == 0);
		script->setVariable(1, 5);
		script->setVariable(3, 7);
		CPPUNIT_ASSERT(script->getVariable(1) == 5);
		CPPUNIT_ASSERT(script->getVariable(3) == 7);
		CPPUNIT_ASSERT(script->getVariable(2) == 0);
		CPPUNIT_ASSERT(script->getVariable(4) == 0);
		delete(script);
  }

	void testScript() {
		Group * group = new Group();
		Script * script = new Script();
		group->setBehavior(script);

		CPPUNIT_ASSERT(script->getVariable(101) == 0);
		CPPUNIT_ASSERT(script->getVariable(102) == 0);

		QueryItem * qi1 = new QueryItem();
		qi1->setQuery(EM_SCRIPT_ONSIGNAL);
		qi1->addQueryParm(11);
		qi1->setAction(EM_SCRIPT_SETVAR);
		qi1->addActionParm(101);
		qi1->addActionParm(1);

		QueryItem * qi2 = new QueryItem();
		qi2->setQuery(EM_SCRIPT_ONSIGNAL);
		qi2->addQueryParm(22);
		qi2->setAction(EM_SCRIPT_SETVAR);
		qi2->addActionParm(101);
		qi2->addActionParm(2);

		script->addQueryItem(qi1);
		script->addQueryItem(qi2);
		
		script->onSignal(11, group); CPPUNIT_ASSERT(script->getVariable(101) == 1);
		script->onSignal(22, group); CPPUNIT_ASSERT(script->getVariable(101) == 2);
		script->onSignal(11, group); CPPUNIT_ASSERT(script->getVariable(101) == 1);

		CPPUNIT_ASSERT(script->getVariable(102) == 0);

		delete(group);
	}

	CPPUNIT_TEST_SUITE(ScriptTest);

	CPPUNIT_TEST(testVariable);
	CPPUNIT_TEST(testScript);
	
	CPPUNIT_TEST_SUITE_END();
};

class LoaderTest : public CppUnit::TestFixture {
 public:
  void setUp() {
  }

  void tearDown() {
  }

  void testSignalVariable() {
		int s1, v1, s2, v2, s3, v3;
		CPPUNIT_ASSERT(Loader::getInstance()->getSignal(1) == NULL);
		CPPUNIT_ASSERT(Loader::getInstance()->getVariable(1) == NULL);
		s1 = Loader::getInstance()->getSignal("sig");
		v1 = Loader::getInstance()->getVariable("var");
		CPPUNIT_ASSERT(s1 > 0);
		CPPUNIT_ASSERT(v1 > 0);
		CPPUNIT_ASSERT(Loader::getInstance()->getSignal(s1) != NULL);
		CPPUNIT_ASSERT(Loader::getInstance()->getVariable(v1) != NULL);
		s2 = Loader::getInstance()->getSignal("sig");
		v2 = Loader::getInstance()->getVariable("var");
		s3 = Loader::getInstance()->getSignal("no_sig");
		v3 = Loader::getInstance()->getVariable("no_var");
		CPPUNIT_ASSERT(Loader::getInstance()->getSignal(s1) == Loader::getInstance()->getSignal(s2));
		CPPUNIT_ASSERT(Loader::getInstance()->getVariable(v1) == Loader::getInstance()->getVariable(v2));
		CPPUNIT_ASSERT(Loader::getInstance()->getSignal(s1) != Loader::getInstance()->getSignal(s3));
		CPPUNIT_ASSERT(Loader::getInstance()->getVariable(v1) != Loader::getInstance()->getVariable(v3));
  }

	void testReadToken() {
		// make sure there is a 'token.txt' file in the test directory that looks like:
		// a b
		// aa bb cc
		// 
		// aaaa bbbb
		// 12
		// aaaaaa bbbbbb
		Loader * loader = Loader::getInstance();
		loader->m_iLineNumber = 0;
		ifstream file("token.txt");
		string str("");
		istringstream ist("");
		int i;
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "a");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "b");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "aa");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "bb");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "cc");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "aaaa");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "bbbb");
		loader->readNextToken(file, ist, i); CPPUNIT_ASSERT(i == 12);
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "aaaaaa");
		loader->readNextToken(file, ist, str); CPPUNIT_ASSERT(str == "bbbbbb");
	}

	void testReadScript() {
		// make sure there is a 'script.txt' file in the test directory that looks like:
		// {
    //   onsignal 1 11 setvar 101 1
    //   onsignal 1 22 setvar 101 2
		// }
		Loader * loader = Loader::getInstance();
		loader->m_iLineNumber = 0;
		try {
			ifstream file("script.txt");
			string str("");
			istringstream ist("");
			Group * group = new Group();
			loader->loadScript(file, ist, NULL, group);
			Script * script = (Script*) group->getBehavior();
			
			CPPUNIT_ASSERT(script->getVariable(101) == 0);
			CPPUNIT_ASSERT(script->getVariable(102) == 0);
			
			script->onSignal(11, group); CPPUNIT_ASSERT(script->getVariable(101) == 1);
			script->onSignal(22, group); CPPUNIT_ASSERT(script->getVariable(101) == 2);
			script->onSignal(11, group); CPPUNIT_ASSERT(script->getVariable(101) == 1);
			
			CPPUNIT_ASSERT(script->getVariable(102) == 0);

			delete(group);
		} catch (string str) {
			cerr << str << endl;
			cerr << "At line: " << loader->m_iLineNumber << endl;
			CPPUNIT_ASSERT(false);
		}
	}

	void testModule() {
		// make sure there is a 'module.txt' file in the test directory that looks like:
		// {
    //   libemilia_module_test.la
		// }
		// and that there is a modlib.cpp that is compile to a libemilia_module.la in test
		Config::getInstance()->setDataDir("/home/henqvist/Develop/pinball/test");
		Config::getInstance()->setSubDir("");
		Loader * loader = Loader::getInstance();
		loader->m_iLineNumber = 0;
		try {
			ifstream file("module.txt");
			string str("");
			istringstream ist("");
			Group * group = new Group();
			loader->loadModule(file, ist, NULL, group);
			delete(group);
		} catch (string str) {
			cerr << str << endl;
			cerr << "At line: " << loader->m_iLineNumber << endl;
			CPPUNIT_ASSERT(false);
		}
	}

	CPPUNIT_TEST_SUITE(LoaderTest);

	CPPUNIT_TEST(testSignalVariable);
	CPPUNIT_TEST(testReadToken);
	CPPUNIT_TEST(testReadScript);
	CPPUNIT_TEST(testModule);
	
	CPPUNIT_TEST_SUITE_END();
};


class StateBehaviorTest : public CppUnit::TestFixture {
 public:
  void setUp() {
  }

  void tearDown() {
  }

	void testShapeEnable() {
		StateItem * stateitem = new StateItem();
		CPPUNIT_ASSERT(stateitem->getShapeEnable(-1) == false);
		stateitem->addShapeEnable(2, true); CPPUNIT_ASSERT(stateitem->getShapeEnable(2) == true);
		CPPUNIT_ASSERT(stateitem->getShapeEnable(1) == false);
		stateitem->addShapeEnable(1, true); CPPUNIT_ASSERT(stateitem->getShapeEnable(1) == true);
		stateitem->addShapeEnable(2, true); CPPUNIT_ASSERT(stateitem->getShapeEnable(2) == true);
		stateitem->addShapeEnable(2, false); CPPUNIT_ASSERT(stateitem->getShapeEnable(2) == false);
		CPPUNIT_ASSERT(stateitem->getShapeEnable(-1) == false);
	}

	CPPUNIT_TEST_SUITE(StateBehaviorTest);

	CPPUNIT_TEST(testShapeEnable);
	
	CPPUNIT_TEST_SUITE_END();
};


#endif // EM_UNIT_TEST

#endif // SRCTEST_H

