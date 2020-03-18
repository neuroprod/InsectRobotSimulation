#pragma once
#include "Vector3.h"
#include "Anime.h"
#include "Anime3.h"

class Animator 
{

public:
	Animator() {};

	Anime  animes[7];
	int animeCount = 0;

	Anime3  animes3[6];
	int anime3Count = 0;

	void reg(float *target) 
	{
		animes[animeCount].target = target;
		animeCount++;
	}
	void reg(Vector3 *target)
	{
		animes3[anime3Count].target = target;
		
		anime3Count++;

	}
	void tween(float *target, float endVal, float time,float delay =0) 
	{
		
		for (int i = 0; i < animeCount; i++)
		{
		
			if (animes[i].target == target)
			{
			
				animes[i].startTween(endVal, time, delay);
				return;
			}
		}
	}
	void tween(Vector3 *target, float endValX, float endValY, float endValZ, float time, float delay = 0)
	{

	
		for (int i = 0; i < anime3Count; i++)
		{
			
			if (animes3[i].target == target)
			{

				animes3[i].startTween(endValX,endValY,endValZ, time, delay);
				return;
			}
		}
	}
	bool isActive() 
	{
		for (int i = 0; i < animeCount; i++)
		{
			if (animes[i].isActive)
			{
				return true;

			}

		}
		for (int i = 0; i < anime3Count; i++)
		{
			if (animes3[i].isActive)
			{
				return true;

			}

		}
	
		return false;
	}
	void update(float delta) 
	{
		for (int i = 0; i < animeCount; i++) 
		{
			if (animes[i].isActive) 
			{
				animes[i].update(delta);
			
			}
		
		}
		for (int i = 0; i < anime3Count; i++)
		{
			if (animes3[i].isActive)
			{
				animes3[i].update(delta);

			}

		}
	
	}


};