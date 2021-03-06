
#pragma once

class Vector {
public:

	float x;
	float y;
	Vector(float x=0, float y=0);
	float angle();
	void clear();
	float distance(Vector v);
	float dot(Vector);
	float length();
	Vector rotate(float r);
	Vector normalize(float scale = 1);
	

	Vector operator* (const float f)
	{
		return Vector(f*x, f*y);
	}
	Vector operator/ (const float f)
	{
		return Vector(x/f, y/f);
	}
	Vector operator + (const Vector &v)
	{
		return Vector(x + v.x, y + v.y);
	}
	//TODO: implement += operator
	Vector operator - (const Vector &v)
	{
		return Vector(x - v.x, y - v.y);
	}
};