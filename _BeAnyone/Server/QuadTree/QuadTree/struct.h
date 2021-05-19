#pragma once

typedef struct Point
{
	float x;
	float z;
	Point() { x = 0.f, z = 0.f; }
	Point(const float& x, const float& z)
	{
		this->x = x;
		this->z = z;
	}

};