#pragma once
#include <math.h>
#include <string>

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return 2 * ((rand()) / (RAND_MAX + 1.0)) - 1; }

