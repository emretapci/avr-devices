#ifndef _CIRCLE_DRAWER_H_
#define _CIRCLE_DRAWER_H_

#include <util/Util.h>
#include <util/position.h>

#define ANGLE INT32 //in 1/300 degrees increments

class CircleDrawer
{
public:
	//draws an arc counter clockwise.
	//x, y: center; r: radius
	//startAngle: start angle (ccw from +x axis) range: 0 - 360 degrees
	//endAngle: end angle (ccw from +x axis) range: 0 - 360 degrees
	static void Midpoint2D(DISTANCE radius, ANGLE startAngle, ANGLE endAngle, void *instance, Bool (*next)(void *instance, Position current));

private:
	//draws an arc counter clockwise in the given quadrant of a circle.
	//quadrant: 0 - 3, 0: +x & +y quadrant, 1 - 3: other quadrants in ccw direction.
	//startAngle: start angle (ccw from +x axis) range: 0 - 90 degrees
	//endAngle: end angle (ccw from +x axis) range: 0 - 90 degrees
	static void MidPoint2D_1(uint8_t quadrant, DISTANCE radius, ANGLE startAngle, ANGLE endAngle, void *instance, Bool (*next)(void *instance, Position current));

	//draws an arc centered at (0, 0) counter clockwise in 1st quadrant of a circle.
	//startAngle: start angle (ccw from +x axis) range: 0 - 90 degrees
	//endAngle: end angle (ccw from +x axis) range: 0 - 90 degrees
	//m11, m12, m21, m22: Elements of 2x2 matrix by which the "current" parameter's x and y elements are multiplied with before calling next function.
	static void MidPoint2D_0(DISTANCE radius, ANGLE startAngle, ANGLE endAngle, void *instance, Bool (*next)(void *instance, Position current),
		uint8_t m11, uint8_t m12, uint8_t m21, uint8_t m22);
};

#endif
