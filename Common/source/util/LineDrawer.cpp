#include <util/line_drawer.h>

void LineDrawer::Bresenham3D(Position from, Position to, void *instance, Bool (*next)(void *instance, Position current))
{
	DISTANCE i, l, m, n, err_1, err_2, point[3];
	Position dpos, incpos, d2pos, cur;

	point[0] = from.x;
	point[1] = from.y;
	point[2] = from.z;
	dpos.x = to.x - from.x;
	dpos.y = to.y - from.y;
	dpos.z = to.z - from.z;
	incpos.x = (dpos.x < 0) ? -1 : 1;
	l = ABS(dpos.x);
	incpos.y = (dpos.y < 0) ? -1 : 1;
	m = ABS(dpos.y);
	incpos.z = (dpos.z < 0) ? -1 : 1;
	n = ABS(dpos.z);
	d2pos.x = l << 1;
	d2pos.y = m << 1;
	d2pos.z = n << 1;

	if (l >= m && l >= n)
	{
		err_1 = d2pos.y - l;
		err_2 = d2pos.z - l;
		for (i = 0; i < l; i++)
		{
			cur.x = point[0];
			cur.y = point[1];
			cur.z = point[2];
			if(!next(instance, cur))
			{
				break;
			}
			if (err_1 > 0)
			{
				point[1] += incpos.y;
				err_1 -= d2pos.x;
			}
			if (err_2 > 0)
			{
				point[2] += incpos.z;
				err_2 -= dpos.x;
			}
			err_1 += d2pos.y;
			err_2 += d2pos.z;
			point[0] += incpos.x;
		}
	}
	else if (m >= l && m >= n)
	{
		err_1 = d2pos.x - m;
		err_2 = d2pos.z - m;
		for (i = 0; i < m; i++)
		{
			cur.x = point[0];
			cur.y = point[1];
			cur.z = point[2];
			if(!next(instance, cur))
			{
				break;
			}
			if (err_1 > 0)
			{
				point[0] += incpos.x;
				err_1 -= d2pos.y;
			}
			if (err_2 > 0)
			{
				point[2] += incpos.z;
				err_2 -= d2pos.y;
			}
			err_1 += d2pos.x;
			err_2 += d2pos.z;
			point[1] += incpos.y;
		}
	}
	else
	{
		err_1 = d2pos.y - n;
		err_2 = d2pos.x - n;
		for (i = 0; i < n; i++)
		{
			cur.x = point[0];
			cur.y = point[1];
			cur.z = point[2];
			if(!next(instance, cur))
			{
				break;
			}
			if (err_1 > 0)
			{
				point[1] += incpos.y;
				err_1 -= d2pos.z;
			}
			if (err_2 > 0)
			{
				point[0] += incpos.x;
				err_2 -= d2pos.z;
			}
			err_1 += d2pos.y;
			err_2 += d2pos.x;
			point[2] += incpos.z;
		}
	}
	cur.x = point[0];
	cur.y = point[1];
	cur.z = point[2];
	next(instance, cur);
}
