#include <iostream>
using namespace std;

typedef struct { float x, y; }Point;

Point PointOnCubicBezier(Point* cp, float t)
{
	float ax, bx, cx, ay, by, cy, tsquared, tcube;
	Point result;

	cx = 3.0f * (cp[1].x - cp[0].x);
	bx = 3.0f * (cp[2].x - cp[1].x) - cx;
	ax = (cp[3].x - cp[0].x) - cx - bx;

	cy = 3.0f * (cp[1].y - cp[0].y);
	by = 3.0f * (cp[2].y - cp[1].y) - cy;
	ay = (cp[3].y - cp[0].y) - cy - by;

	tsquared = t * t;
	tcube = tsquared * t;

	result.x = (ax * tcube) + (bx * tsquared) + (cx * t) + cp[0].x;
	result.y = (ay * tcube) + (bx * tsquared) + (cy * t) + cp[0].y;

	return result;
}

void ComputeBezier(Point* cp, Point* curve)
{
	float dt;
	int i;
	dt = 1.f / 3;

	for (i = 0; i < 4; i++)
	{
		curve[i] = PointOnCubicBezier(cp, i * dt);
	}
}

int main()
{
	Point temp[4];
	Point curve[4];
	int i = 0;
	temp[0].x = 0;
	temp[0].y = 0;

	temp[1].x = 0;
	temp[1].y = 10;
	
	temp[2].x = 10;
	temp[2].y = 10;
	
	temp[3].x = 10;
	temp[3].y = 0;

	ComputeBezier(temp, curve);

	for (auto& t : curve)
	{
		cout << "지나는 점 : " << t.x << ", " << t.y << endl;
	}



	return 0;
}