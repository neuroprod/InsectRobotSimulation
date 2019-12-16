#pragma once

#include "ModelConfig.h";
#include "ModelControl.h";
#include "ModelLeg.h";

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Model 
{

public:
	Model()
	{
		legs[0] = &FR;
		legs[1] = &MR;
		legs[2] = &BR;

		legs[3] = &FL;
		legs[4] = &ML;
		legs[5] = &BL;
	}
	void setConfig(ModelConfig * config) 
	{
		
		FR.setBase(config->frontLegStart, glm::vec3(0, config->frontLegAngle, 3.1415 / 2), config, false);

		MR.setBase(config->middleLegStart, glm::vec3(0, config->middleLegAngle, 3.1415 / 2), config,false);

		BR.setBase(config->backLegStart, glm::vec3(0, config->backLegAngle, 3.1415 / 2), config,false);


		glm::vec3 frontstart = config->frontLegStart;
		frontstart.z *= -1;
		FL.setBase(frontstart, glm::vec3(0, -config->frontLegAngle + 3.1415, 3.1415 / 2), config,true);

		glm::vec3 middlestart = config->middleLegStart;
		middlestart.z *= -1;
	
		ML.setBase(middlestart, glm::vec3(0, -config->middleLegAngle + 3.1415, 3.1415 / 2), config,true);

		glm::vec3 backstart = config->backLegStart;
		backstart.z *= -1;
		BL.setBase(backstart, glm::vec3(0, -config->backLegAngle + 3.1415, 3.1415 / 2), config,true);


	}
	void setControl(ModelControl *control) 
	{

		/// Inverse kinematics calculation
		rootMatrix = glm::mat4();
		rootMatrix = glm::translate(rootMatrix, glm::vec3(control->rootOffX, control->rootHeight, control->rootOffZ));
	
		rootMatrix = glm::rotate(rootMatrix, control->rootRotX, glm::vec3(1, 0, 0));
		rootMatrix = glm::rotate(rootMatrix, control->rootRotY, glm::vec3(0, 1, 0));
		rootMatrix = glm::rotate(rootMatrix, control->rootRotZ, glm::vec3(0, 0, 1));
		

		for (int i = 0; i < 6; i++) 
		{
			legs[i]->resolve(rootMatrix);
		
		}

	}
	void update()
	{	

	}

	ModelLeg FR;
	ModelLeg MR;
	ModelLeg BR;

	ModelLeg FL;
	ModelLeg ML;
	ModelLeg BL;

	ModelLeg* legs[6];

	glm::mat4 rootMatrix;




};