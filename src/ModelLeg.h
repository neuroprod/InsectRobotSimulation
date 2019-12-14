#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ModelConfig.h"
class ModelLeg
{
public:
	ModelLeg() 
	{
	
	}
	void setBase(glm::vec3 position, glm::vec3 rotation, ModelConfig * config)
	{
		baseMatrix = glm::mat4();
		baseMatrix = glm::translate(baseMatrix, position);
		baseMatrix = glm::rotate(baseMatrix, rotation.x, glm::vec3(1, 0, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.y, glm::vec3(0, 1, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.z, glm::vec3(0, 0, 1));

		glm::vec3 dir = glm::vec3(sin(rotation.y), 0, cos(rotation.y));


		homePoint = position + (dir*config->homeDistance);
		

	}
	glm::mat4 baseMatrix;
	glm::vec3 homePoint;
};