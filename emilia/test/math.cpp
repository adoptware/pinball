/***************************************************************************
 Math test. Debug the math functions.
 ***************************************************************************/

#include "EMath.h"

#define COUT_MTX(mtx) {\
	cout << mtx.v[0][0] <<" "<< mtx.v[0][1] <<" "<< mtx.v[0][2] << endl; \
	cout << mtx.v[1][0] <<" "<< mtx.v[1][1] <<" "<< mtx.v[1][2] << endl; \
	cout << mtx.v[2][0] <<" "<< mtx.v[2][1] <<" "<< mtx.v[2][2] << endl; \
	cout << "t " << mtx.t[0] <<" "<< mtx.t[1] <<" "<< mtx.t[2] << endl; \
}
/**
 * Main
 */
int main(int argc, char *argv[]) {
	Matrix a = identityMatrix;
	Matrix b;
	Matrix c = {{{2,0,0},{0,2,0},{0,0,2},},{0,0,0}};
	Matrix d = {{{2,1,0},{1,2,0},{0,1,2},},{0,0,0}};

	EMath::inverse(a, b);
	COUT_MTX(b);

	EMath::inverse(a, b);
	COUT_MTX(b);

	EMath::inverse(c, b);
	COUT_MTX(b);

	EMath::inverse(d, b);
	COUT_MTX(b);
return 0;
}
