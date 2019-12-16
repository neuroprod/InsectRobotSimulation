#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ModelConfig.h"
#include "cinder/app/App.h"
using namespace ci::app;
using namespace std;

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

		dir = glm::vec3(sin(rotation.y), 0, cos(rotation.y));


		homePoint = position + (dir*config->homeDistance);
		homePoint.y = 0;
	
		upperLegSize = config->upperLegSize;
		lowerLegSize = config->lowerLegSize;
	}
	void resolve(glm::mat4 &rootMatrix)
	{

		glm::vec3 targetPoint = homePoint;//+targetVec

		//start resolving kinematics
		//***show resolve Joint 1 in UI***

		glm::mat4 inversMatrix = glm::inverse(rootMatrix*baseMatrix);
		//transform targetpoint to joint1 local space
		targetPointLocal = inversMatrix*glm::vec4(targetPoint.x, targetPoint.y, targetPoint.z, 1);
		
		targetPointLocalFlat = glm::vec3(0, targetPointLocal.y, targetPointLocal.z);
		
		shoulder1Angle =  atan2(0, 1)- atan2(targetPointLocalFlat.y, targetPointLocalFlat.z);

		
		//***show resolve Joint 23 in UI***
		//transform target point to joint 1 local space after rotation
		glm::mat4 shoulder1Matrix = glm::rotate(baseMatrix, shoulder1Angle, glm::vec3(1, 0, 0));
		shoulder1GlobalMatrix = rootMatrix*shoulder1Matrix;
		inversMatrix = glm::inverse(shoulder1GlobalMatrix);
		glm::vec4 targetPointLocal2 = inversMatrix*glm::vec4(targetPoint.x, targetPoint.y, targetPoint.z, 1);
		
	
		//reduce to 2D problem 
		posJoint = glm::vec2(0, 24);
		float r1 = upperLegSize;
		posTarget = glm::vec2(targetPointLocal2.x, targetPointLocal2.z);
		float r2 = lowerLegSize;
		// intersection 2 circles


	}

	float shoulder1Angle =0;

	float upperLegSize;
	float lowerLegSize;



	glm::vec3 dir;
	glm::mat4 baseMatrix;
	glm::vec3 homePoint;
	glm::vec4 targetPointLocal;
	glm::vec3 targetPointLocalFlat;

	glm::mat4 shoulder1GlobalMatrix;
	glm::vec2 posJoint;
	glm::vec2 posTarget;
	
};