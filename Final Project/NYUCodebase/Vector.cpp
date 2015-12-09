#include "Vector.h"
#include <math.h>

using namespace std;

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
float Vector::angle()
{
	return atan2(y,x);
}
void Vector::clear()
{
	x = 0;
	y = 0;
	z = 0;
}
float Vector::distance(Vector v)
{
	return this->operator-(v).length();
}
float Vector::length()
{
	return sqrt(x*x + y*y+z*z);


}
void Vector::rotate(float r)
{
	float temp = x*cos(r) - y*sin(r);
	y = x*sin(r) + y*cos(r);
	x = temp;
}
void Vector::normalize(float scale)
{
	if (length() != 0)
	{
		float l = length();
		l /= scale;
		x /= l;
		y /= l;
		z /= l;
	}
}

