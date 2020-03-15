#pragma once


class Anime 
{
public:
	Anime() {}
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
			*target = startVal + changeVal;
			return;
		}
		else 
		{
			float val = easeInOutQuad(time);
			*target = startVal + changeVal*val;
		
		}
	
	}

	float easeInOutQuad(float t) {
		return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
	}

	void startTween(float endVal, float time, float delay) 
	{
		isActive = true;

		currentTime = -delay;
		totalTime = time;
		startVal = *target;
		changeVal = endVal - startVal;

	}


	bool isActive = false;
	float *target;
	float currentTime = 0;
	float totalTime = 0;
	float startVal = 0;
	float changeVal = 0;
};