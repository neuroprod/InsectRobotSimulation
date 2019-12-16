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

		legA[0] = &FR;
		legA[1] = &BR;
		legA[2] = &ML;
		legB[0] = &FL;
		legB[1] = &BL;
		legB[2] = &MR;

		legA[0]->isSet1 = true;
		legA[1]->isSet1 = true;
		legA[2]->isSet1 = true;

		legB[0]->isSet1 = false;
		legB[1]->isSet1 = false;
		legB[2]->isSet1 = false;
	}
	void setConfig(ModelConfig * config) 
	{
		mConfig = config;
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




		mControl = control;

	

	}
	void update(float delta)
	{	
		currentTime2 += delta;
		currentTime += delta;
		if (currentTime2 > mConfig->stepTime * 2) 
		{
			currentTime2 = 0;
		}
		if (currentTime > mConfig->stepTime)
		{
			currentTime = 0;
			legSwitch = !legSwitch;
			
			if (legSwitch)
			{
				for (int i = 0; i < 6; i++)
				{
					legs[i]->isForward = legs[i]->isSet1;
				}
			}
			else
			{
				for (int i = 0; i < 6; i++)
				{
					legs[i]->isForward = !legs[i]->isSet1;
				}
			}
			for (int i = 0; i < 6; i++)
			{
				

				
				legs[i]->dirMove = glm::vec3(cos(mControl->moveAngle), 0, sin(mControl->moveAngle));
				legs[i]->angleTurn = mControl->turnAngle;
				legs[i]->moveDistance = mControl->moveDistance;
				legs[i]->reset();
			}
		}
		float posTime2 = currentTime2 / (mConfig->stepTime*2);
		float posTime = currentTime / mConfig->stepTime;
		float h = mControl->rootHeight +sinf(( posTime)*3.1415*2) * 5 * (mControl->moveDistance/60);

		/// Inverse kinematics calculation
		rootMatrix = glm::mat4();
		rootMatrix = glm::translate(rootMatrix, glm::vec3(mControl->rootOffX, h, mControl->rootOffZ));

		rootMatrix = glm::rotate(rootMatrix, mControl->rootRotX, glm::vec3(1, 0, 0));
		rootMatrix = glm::rotate(rootMatrix, mControl->rootRotY+ sinf((posTime2)*3.1415 * 2) * 0.04f * (mControl->moveDistance / 60), glm::vec3(0, 1, 0));
		rootMatrix = glm::rotate(rootMatrix, mControl->rootRotZ+ (mControl->moveDistance / 60)*0.05f, glm::vec3(0, 0, 1));


		for (int i = 0; i < 6; i++)
		{
			legs[i]->resolve(rootMatrix, posTime);

		}


	}
	float currentTime2 = 0;
	float currentTime =0;
	bool legSwitch = false;
	ModelControl *mControl;
	ModelConfig *mConfig;
	ModelLeg FR;
	ModelLeg MR;
	ModelLeg BR;

	ModelLeg FL;
	ModelLeg ML;
	ModelLeg BL;

	ModelLeg* legs[6];

	ModelLeg* legA[3];
	ModelLeg* legB[3];
	glm::mat4 rootMatrix;




};