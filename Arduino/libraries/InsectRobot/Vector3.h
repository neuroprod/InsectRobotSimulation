#pragma once
#include <math.h>
class Vector3 
{


public:
	float x =0.f;
	float y=0.f;
	float z=0.f;

	Vector3() {};
	Vector3(float _x,float _y,float _z) 
	{
		x = _x;
		y = _y;
		z = _z;
	
	};


	static float length(const Vector3 &in);





	Vector3 operator + (const Vector3 &in) {
		Vector3 res(x+in.x, y + in.y, z + in.z);
		return res;
	}
	
	Vector3 operator - (const Vector3 &in) {
		Vector3 res(x - in.x, y - in.y, z - in.z);
		return res;
	}

	Vector3 operator * (float a) {
		Vector3 res(x*a,y*a,z*a);
		return res;
	}

	Vector3 operator / (float a) {
		Vector3 res(x/a, y/a, z/a);
		return res;
	}
	
};