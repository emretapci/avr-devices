#ifndef _LINE_DRAWER_H_
#define _LINE_DRAWER_H_

#include <util/Util.h>
#include <util/position.h>

class LineDrawer
{
public:
	static void Bresenham3D(Position from, Position to, void *instance, Bool (*next)(void *instance, Position current));
};

#endif
