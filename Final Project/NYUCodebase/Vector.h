
#pragma once

class Vector {
public:

	float x;
	float y;
	float z;
	Vector(float x=0, float y=0, float z=0);
	float angle();
	void clear();
	float distance(Vector v);
	float length();
	void rotate(float r);
	void normalize(float scale = 1);

	Vector operator* (const float f)
	{
		return Vector(f*x, f*y, f*z);
	}
	Vector operator/ (const float f)
	{
		return Vector(x/f, y/f, z/f);
	}
	Vector operator + (const Vector &v)
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator - (const Vector &v)
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
};