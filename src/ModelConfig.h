#pragma once
#include "glm\glm.hpp"
class ModelConfig 
{
public: 
	bool isDirty = true;

	float homeDistance = 100;

	float upperLegSize = 80;
	float lowerLegSize = 130;
	


	float frontLegAngle =3.1415/4 ;
	glm::vec3 frontLegStart = glm::vec3(70, 0, 80);

	float middleLegAngle = 0;
	glm::vec3 middleLegStart = glm::vec3(0, 0, 100);

	float backLegAngle = -3.1415 / 4;
	glm::vec3 backLegStart = glm::vec3(-70, 0, 80);
};