#pragma once

#include "Vector3.h"

class Anime3
{
public:
	Anime3() {}
	void update(float delta)
	{

		currentTime += delta;
		float time = currentTime / totalTime;



		//delay
		if (time <= 0.f) {
			return;
		}
		//end
		else if (time >= 1.f)
		{
			isActive = false;
			target->x = startValX + changeValX;
			target->y = startValY + changeValY;
			target->z = startValZ + changeValZ;
			return;
		}
		//tween
		else
		{
			
			float val = easeInOutQuad(time);
			
			target->x = startValX + changeValX*val;
			target->y = startValY + changeValY*val;
			target->z = startValZ + changeValZ*val;
		}

	}

	float easeInOutQuad(float t) {
		return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
	}

	void startTween(float endValX, float endValY,float endValZ, float time, float delay)
	{
		isActive = true;

		currentTime = -delay;
		totalTime = time;
		startValX = target->x;
		changeValX = endValX - startValX;
		
		startValY = target->y;
		changeValY = endValY - startValY;

		startValZ = target->z;
		changeValZ= endValZ - startValZ;

	}


	bool isActive = false;
	Vector3 *target;
	float currentTime = 0;
	float totalTime = 0;

	float startValX = 0;
	float changeValX = 0;

	float startValY = 0;
	float changeValY = 0;

	float startValZ = 0;
	float changeValZ = 0;
};