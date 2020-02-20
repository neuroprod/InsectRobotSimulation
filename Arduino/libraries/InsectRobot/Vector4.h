#pragma once
class Vector4
{


public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 1.f;
	Vector4() {};
	Vector4(float _x, float _y, float _z,float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	};


};
