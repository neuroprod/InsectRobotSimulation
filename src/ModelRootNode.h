#pragma once

#include "ModelNode.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;
using namespace std;
class ModelRootNode 
{

public:
		
	ModelNode *children[6];


	void setBase(glm::vec3 position, glm::vec3 rotation)
	{
		baseMatrix = glm::mat4();
		baseMatrix = glm::translate(baseMatrix, position);


		baseMatrix = glm::rotate(baseMatrix, rotation.x, glm::vec3(1, 0, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.y, glm::vec3(0, 1, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.z, glm::vec3(0, 0, 1));

	}
	void update() 
	{
	
		for (int i = 0; i < 1; i++) 
		{
			glm::vec4 worldPos = baseMatrix*children[i]->baseMatrix  *glm::vec4(0, 0, 0,1);
			glm::vec4 worldPosDir = baseMatrix*children[i]->baseMatrix  *glm::vec4(-1, 0, 0, 1);
			vec4 dir =worldPosDir- worldPos;


			children[i]->worldPos = worldPos;
			console() << worldPos<< dir << endl;
		
		}
	}

	glm::vec3 intersectPoint(glm::vec3 rayVector, glm::vec3 rayPoint) {
		glm::vec3 diff = rayPoint;
		float prod1 = glm::dot(diff,glm::vec3(0,1,0));
		float prod2 = glm::dot(rayVector,glm::vec3(0, 1, 0));
		float prod3 = prod1 / prod2;
		return rayPoint - rayVector * prod3;
	}

	glm::mat4 baseMatrix;
};
