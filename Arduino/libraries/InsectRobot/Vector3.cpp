#include "Vector3.h"

float Vector3::length(const Vector3 &in)
{

	return sqrt(in.x*in.x + in.y*in.y + in.z*in.z);
}