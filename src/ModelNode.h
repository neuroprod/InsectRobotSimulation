#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class ModelNode
{
public:
	ModelNode() {};

	ModelNode *child = nullptr;
	ModelNode *parent = nullptr;

	void setBase(glm::vec3 _position, glm::vec3 rotation)
	{
		position = _position;
		baseMatrix = glm::mat4();
		baseMatrix = glm::translate(baseMatrix, position);


		baseMatrix = glm::rotate(baseMatrix, rotation.x, glm::vec3(1, 0, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.y, glm::vec3(0, 1, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.z, glm::vec3(0, 0, 1));

	}
	void update() 
	{
	
	
	}
	glm::vec4 worldPos;
	glm::vec3 position;
	glm::vec3 restPoint;
	glm::mat4 baseMatrix;
};
