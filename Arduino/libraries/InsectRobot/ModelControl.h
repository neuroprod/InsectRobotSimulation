#pragma once
#include "Animator.h"
class ModelControl
{
public:
	bool isDirty = true;
	float rootHeight=80;
	float rootOffX = 0;
	float rootOffZ= 0;

	float rootRotX =0;
	float rootRotY = 0;
	float rootRotZ = 0;
	
	float turnAngle = 0;
	 
	float moveAngle = 0.0;
	float moveDistance =0;
	float timeScale = 1;

	int state = 0;



	void registerAnime(Animator *animator)
	{
		animator->reg(&rootHeight);
		animator->reg(&rootOffX);
		animator->reg(&rootOffZ);

		animator->reg(&rootRotX);
		animator->reg(&rootRotY);
		animator->reg(&rootRotZ);

	}

};


