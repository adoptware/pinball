/***************************************************************************
 Unit test runner
 ***************************************************************************/

#include "Private.h"
#include "TextureUtil.h"

#if EM_UNIT_TEST
#include <cppunit/ui/text/TestRunner.h>
#include "Private.h"
#include "SrcTest.h"
#include "BaseTest.h"
#endif

/** Main */
int main(int argc, char *argv[]) {
#if EM_UNIT_TEST
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(EMathTest::suite());
	runner.addTest(LoaderTest::suite());
	runner.addTest(ScriptTest::suite());
	runner.addTest(StateBehaviorTest::suite());
	runner.addTest(ShapeTest::suite());
  runner.run();
#else
  cerr << "Unittest support not found, install cppunit and reconfigure!" << endl;
#endif
  return 0;
}

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif
