#ifndef _POSITION_H_
#define _POSITION_H_

#include <util/Util.h>

//-2147483647 through 2147483647: -13421772 127/160 mm through 13421772 127/160 mm, in 1/160 mm increments
#define DISTANCE int32_t
#define DISTANCE_MIN -2147483647
#define DISTANCE_MAX 2147483647

class Position
{
public:
	DISTANCE x;
	DISTANCE y;
	DISTANCE z;
};

#endif
