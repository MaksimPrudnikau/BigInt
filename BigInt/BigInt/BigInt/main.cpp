#include "BigInt.h"
#include <cassert>

int main()
{
	BigInt a = 999;
	BigInt b = 1234;
	cout << a + b;

	assert(BigInt(0) + BigInt(1234) == 1234);
	assert(BigInt(1234) + BigInt(1234) == 2468);

	return 0;
}