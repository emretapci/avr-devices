#include <util/circle_drawer.h>
#include <math.h>
#include <avr/interrupt.h>

void CircleDrawer::Midpoint2D(DISTANCE radius, ANGLE startAngle, ANGLE endAngle, void *instance, Bool (*next)(void *instance, Position current))
{
	if (startAngle == endAngle)
	{
		return;
	}

	int startQuadrant, endQuadrant, startAngleM, endAngleM;

	startQuadrant = startAngle / (300 * 90);
	endQuadrant = (endAngle - 1) / (300 * 90);
	startAngleM = startAngle - startQuadrant * (300 * 90);
	endAngleM = endAngle - endQuadrant * (300 * 90);

	if (startQuadrant == endQuadrant)
	{
		MidPoint2D_1(startQuadrant, radius, startAngleM, endAngleM, instance, next);
	}
	else
	{
		for (int quadrant = startQuadrant; ; quadrant = (quadrant + 1) % 4)
		{
			if (quadrant == startQuadrant)
			{
				MidPoint2D_1(quadrant, radius, startAngleM, 300 * 90, instance, next);
			}
			else if (quadrant == endQuadrant)
			{
				MidPoint2D_1(quadrant, radius, 0, endAngle, instance, next);
			}
			else
			{
				MidPoint2D_1(quadrant, radius, 0, 300 * 90, instance, next);
			}

			if (quadrant == endQuadrant)
			{
				break;
			}
		}
	}
}

void CircleDrawer::MidPoint2D_1(uint8_t quadrant, DISTANCE radius, ANGLE startAngle, ANGLE endAngle, void *instance, Bool (*next)(void *instance, Position current))
{
	switch (quadrant)
	{
		case 0:
			MidPoint2D_0(radius, startAngle, endAngle, instance, next, 1, 0, 0, 1);
			break;
		case 1:
			MidPoint2D_0(radius, startAngle, endAngle, instance, next, 0, -1, 1, 0);
			break;
		case 2:
			MidPoint2D_0(radius, startAngle, endAngle, instance, next, -1, 0, 0, -1);
			break;
		case 3:
			MidPoint2D_0(radius, startAngle, endAngle, instance, next, 0, 1, -1, 0);
			break;
	}
}

void CircleDrawer::MidPoint2D_0(DISTANCE radius, ANGLE startAngle, ANGLE endAngle, void *instance, Bool (*next)(void *instance, Position current),
	uint8_t m11, uint8_t m12, uint8_t m21, uint8_t m22)
{
	DISTANCE xx = radius, xxp = xx;
	DISTANCE yy = 0, yyp = yy;
	DISTANCE d2 = 1 - xx, d2p = d2;

	DISTANCE x0, x1, y0, y1;
	Position cur;

	if (startAngle > endAngle)
	{
		return;
	}

	if (startAngle < 45 * 300)
	{
		x1 = DISTANCE_MAX;
		y0 = radius * sin(startAngle / 300.0 * M_PI / 180.0);
	}
	else if (startAngle > 45 * 300)
	{
		double dd = startAngle / 300.0 * M_PI / 180.0;
		double dd2 = cos(dd);
		x1 = radius * cos(startAngle / 300.0 * M_PI / 180.0);
		y0 = DISTANCE_MAX;
	}
	else
	{
		x1 = DISTANCE_MAX;
		y0 = DISTANCE_MAX;
	}

	if (endAngle < 45 * 300)
	{
		x0 = DISTANCE_MAX;
		y1 = radius * sin(endAngle / 300.0 * M_PI / 180.0);
	}
	else if (endAngle > 45 * 300)
	{
		x0 = radius * cos(endAngle / 300.0 * M_PI / 180.0);
		y1 = DISTANCE_MAX;
	}
	else
	{
		x0 = DISTANCE_MAX;
		y1 = DISTANCE_MAX;
	}

	while (yy <= xx)
	{
		if (y0 <= yy && yy <= y1)
		{
			cur.x = m11 * xx + m12 * yy;
			cur.y = m21 * xx + m22 * yy;
			if(!next(instance, cur))
			{
				break;
			}
		}
		yyp = yy;
		yy++;
		if (d2 <= 0)
		{
			d2p = d2;
			d2 += 2 * yy + 1;
		}
		else
		{
			xxp = xx;
			xx--;
			d2p = d2;
			d2 += 2 * (yy - xx) + 1;
		}
	}

	d2 -= 1.4142 * radius;

	while (xx >= 0)
	{
		if (x0 <= xx && xx <= x1)
		{
			cur.x = m11 * xx + m12 * yy;
			cur.y = m21 * xx + m22 * yy;
			if(!next(instance, cur))
			{
				break;
			}
		}
		xx--;
		if (d2 > 0)
		{
			d2p = d2;
			d2 += 1 - 2 * xx;
		}
		else
		{
			yy++;
			d2p = d2;
			d2 += 2 * (yy - xx) + 1;
		}
	}
}
