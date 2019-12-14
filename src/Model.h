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
		FR.setBase(config->frontLegStart, glm::vec3(0, config->frontLegAngle, 3.1415 / 2), config);

		MR.setBase(config->middleLegStart, glm::vec3(0, config->middleLegAngle, 3.1415 / 2), config);

		BR.setBase(config->backLegStart, glm::vec3(0, config->backLegAngle, 3.1415 / 2), config);


		glm::vec3 frontstart = config->frontLegStart;
		frontstart.z *= -1;
		FL.setBase(frontstart, glm::vec3(0, -config->frontLegAngle + 3.1415, 3.1415 / 2), config);

		glm::vec3 middlestart = config->middleLegStart;
		middlestart.z *= -1;
	
		ML.setBase(middlestart, glm::vec3(0, -config->middleLegAngle + 3.1415, 3.1415 / 2), config);

		glm::vec3 backstart = config->backLegStart;
		backstart.z *= -1;
		BL.setBase(backstart, glm::vec3(0, -config->backLegAngle + 3.1415, 3.1415 / 2), config);


	}
	void setControl(ModelControl *control) 
	{
		rootMatrix = glm::mat4();
		rootMatrix = glm::translate(rootMatrix, glm::vec3(0, control->rootHeight,0));
		rootMatrix = glm::rotate(rootMatrix, control->rootRotX, glm::vec3(1, 0, 0));
		rootMatrix = glm::rotate(rootMatrix, control->rootRotY, glm::vec3(0, 1, 0));
		rootMatrix = glm::rotate(rootMatrix, control->rootRotZ, glm::vec3(0, 0, 1));
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




	/*void setBase(glm::vec3 position, glm::vec3 rotation)
	{
		baseMatrix = glm::mat4();
		baseMatrix = glm::translate(baseMatrix, position);


		baseMatrix = glm::rotate(baseMatrix, rotation.x, glm::vec3(1, 0, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.y, glm::vec3(0, 1, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.z, glm::vec3(0, 0, 1));

	}
	void setup(ModelConfig *config) 
	{



		modelRootnode.children[0] = makeLeg(config->frontLegStart, vec3(0, config->frontLegAngle, 3.1415 / 2));
		modelRootnode.children[0]->restPoint = config->frontLegStart;
		modelRootnode.children[0]->restPoint.y = 0;
		
		console() << modelRootnode.children[0]->restPoint << endl;;

		glm::vec3 dir = vec3(sin(config->frontLegAngle), 0, cos(config->frontLegAngle));
		console() << modelRootnode.children[0]->restPoint << endl;;


		modelRootnode.children[1] = makeLeg(config->frontLegStart, vec3(0, config->frontLegAngle, 3.1415 / 2));
		modelRootnode.children[2] = makeLeg(config->frontLegStart, vec3(0, config->frontLegAngle, 3.1415 / 2));

		modelRootnode.children[3] = makeLeg(config->frontLegStart, vec3(0, config->frontLegAngle, 3.1415 / 2));
		modelRootnode.children[4] = makeLeg(config->frontLegStart, vec3(0, config->frontLegAngle, 3.1415 / 2));
		modelRootnode.children[5] = makeLeg(config->frontLegStart, vec3(0, config->frontLegAngle, 3.1415 / 2));
	
	}
	ModelNode * makeLeg(vec3 pos,vec3 rot) 
	{
		ModelNode *n = new ModelNode();
		n->setBase(pos, rot);
		return n;
	}
	void update() 
	{
		
			
		//modelRootnode.update();
	
	}

	ModelRootNode modelRootnode;*/
};