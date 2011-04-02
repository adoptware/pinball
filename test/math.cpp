/***************************************************************************
 Math profiling. 
 ***************************************************************************/

#include "Private.h"
#include "Engine.h"
#include "EMath.h"
#include <sys/time.h>
#include <iostream>

#ifdef EM_USE_FAST_FLOAT2INT

#define MACRO_A(a_, b_) \
a_.f += 1.1;             \
ConvFloat2Int(a_);      \
b_ += a_.i;

#define MACRO_B(a_, b_) \
a_ += 1.1;               \
b_ += (int)a_;

#define MACRO_AA(a_, b_) \
MACRO_A(a_, b_) \
MACRO_A(a_, b_) \
MACRO_A(a_, b_) \
MACRO_A(a_, b_)

#define MACRO_AAA(a_, b_) \
MACRO_AA(a_, b_) \
MACRO_AA(a_, b_) \
MACRO_AA(a_, b_) \
MACRO_AA(a_, b_)

#define MACRO_AAAA(a_, b_) \
MACRO_AAA(a_, b_) \
MACRO_AAA(a_, b_) \
MACRO_AAA(a_, b_) \
MACRO_AAA(a_, b_)

#define MACRO_AAAAA(a_, b_) \
MACRO_AAAA(a_, b_) \
MACRO_AAAA(a_, b_) \
MACRO_AAAA(a_, b_) \
MACRO_AAAA(a_, b_)

#define MACRO_BB(a_, b_) \
MACRO_B(a_, b_) \
MACRO_B(a_, b_) \
MACRO_B(a_, b_) \
MACRO_B(a_, b_)

#define MACRO_BBB(a_, b_) \
MACRO_BB(a_, b_) \
MACRO_BB(a_, b_) \
MACRO_BB(a_, b_) \
MACRO_BB(a_, b_)

#define MACRO_BBBB(a_, b_) \
MACRO_BBB(a_, b_) \
MACRO_BBB(a_, b_) \
MACRO_BBB(a_, b_) \
MACRO_BBB(a_, b_)

#define MACRO_BBBBB(a_, b_) \
MACRO_BBBB(a_, b_) \
MACRO_BBBB(a_, b_) \
MACRO_BBBB(a_, b_) \
MACRO_BBBB(a_, b_)
 
/** Main */
int main(int argc, char *argv[]) {
	cerr << "Math profiling." << endl;

	gBias.i = (23+127) << 23;

	long aaa = 0, bbb = 0;

	struct timeval tv;
	struct timezone tz;
	
	IntOrFloat aa;
	aa.f = 0.0f;
	int bb = 0;
	float cc = 0.0f;
	int dd;

	for (int a=0; a<500; a++) {
		gettimeofday(&tv, &tz);
		long msec = tv.tv_usec;
		for (int b=0; b<100000; b++) {
			MACRO_BBBBB(cc, bb);
		}
		dd += bb;
		gettimeofday(&tv, &tz);
		bbb += (tv.tv_usec - msec);
		msec = tv.tv_usec;
		for (int b=0; b<100000; b++) {
			MACRO_AAAAA(aa, bb);
		}
		dd += bb;
		gettimeofday(&tv, &tz);
		aaa += (tv.tv_usec - msec);
		//cerr << msec << endl;
	}

	//for (int a=0; a<16; a++) {
	cerr << "slow " << bbb << endl;
	cerr << "fast " << aaa << endl; 
	//}

	return 0;
}

#else
int main() {
	cerr << "not using fast float2int" << endl;
	return 0;
}
#endif

#if EM_USE_ALLEGRO
END_OF_MAIN();
#endif

