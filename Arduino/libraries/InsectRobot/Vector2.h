#pragma once
class Vector2
{


public:
	float x = 0.f;
	float y = 0.f;


	Vector2() {};
	Vector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	

	};
	float length()
	{

		return sqrt(x*x + y*y );
	}

};
