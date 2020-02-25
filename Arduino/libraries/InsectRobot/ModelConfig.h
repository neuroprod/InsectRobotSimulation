#pragma once

#include "Vector3.h"
class ModelConfig 
{
public: 
	bool isDirty = true;

	float homeDistance = 100;

	float upperLegSize = 80;
	float lowerLegSize = 130;
	


	float stepHeight =30;
	int stepPower =3;
	float stepTime = 1.f;


	float frontLegAngle =3.1415/4 ;
	Vector3 frontLegStart = Vector3(70, 0, 50);

	float middleLegAngle = 0;
	Vector3 middleLegStart = Vector3(0, 0, 70);

	float backLegAngle = -3.1415 / 4;
	Vector3 backLegStart = Vector3(-70, 0, 50);
};