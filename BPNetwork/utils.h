#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <string>

using namespace std;

//----------------------------------------------------------------------------
//	some random number functions.
//----------------------------------------------------------------------------

//returns a random integer between x and y
inline int	  RandInt(int x, int y) { return rand() % (y - x + 1) + x; }

//returns a random float between zero and 1
inline double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }

//returns a random bool
inline bool   RandBool()
{
	if (RandInt(0, 1)) return true;

	else return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return 2 * ((rand()) / (RAND_MAX + 1.0)) - 1; }

#endif