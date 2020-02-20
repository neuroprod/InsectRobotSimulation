#pragma once
#include <math.h>


#include "ModelConfig.h"
#include "ModelControl.h"
#include "ModelLeg.h"
#include "Vector3.h"
#include "Matrix44.h"

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
		FR.setBase(config->frontLegStart, Vector3(0, config->frontLegAngle, 3.1415 / 2), config, false);

		MR.setBase(config->middleLegStart, Vector3(0, config->middleLegAngle, 3.1415 / 2), config,false);

		BR.setBase(config->backLegStart, Vector3(0, config->backLegAngle, 3.1415 / 2), config,false);


		Vector3 frontstart = config->frontLegStart;
		frontstart.z *= -1;
		FL.setBase(frontstart, Vector3(0, -config->frontLegAngle + 3.1415, 3.1415 / 2), config,true);

		Vector3 middlestart = config->middleLegStart;
		middlestart.z *= -1;
		ML.setBase(middlestart, Vector3(0, -config->middleLegAngle + 3.1415, 3.1415 / 2), config,true);

		Vector3 backstart = config->backLegStart;
		backstart.z *= -1;
		BL.setBase(backstart, Vector3(0, -config->backLegAngle + 3.1415, 3.1415 / 2), config,true);


	}
	void setControl(ModelControl *control) 
	{




		mControl = control;

	

	}


	void update(float delta)
	{	



	
		currentTime += delta/mControl->timeScale;
		
		if (isHome) 
		{
			if (mControl->moveDistance != 0 || mControl->turnAngle != 0) 
			{
				currentTime = mConfig->stepTime + 0.01;
				isHome = false;
				startHome = false;
				//legSwitch = !legSwitch;
			}
			else 
			{
			
				return;
			}
		
		
		}
		if (currentTime > mConfig->stepTime)
		{
			if (startHome == true && isHome == false) 
			{
				isHome = true;
			
			}

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
			Vector3 dirMove = Vector3(cos(mControl->moveAngle), 0, sin(mControl->moveAngle))*(mControl->moveDistance);
			if (mControl->moveDistance == 0 && mControl->turnAngle == 0) 
			{
				startHome = true;
			}
			else {
				startHome = false;

			}
			for (int i = 0; i < 6; i++)
			{
				float factor = 1;

				if (!legs[i]->isForward) factor = -1;
				
				legs[i]->reset(dirMove *factor, mControl->turnAngle*factor);
				
			
			}
		}
	
		float posTime = currentTime / mConfig->stepTime;
	



		if ((!legSwitch && legs[0]->state==1)   || (legSwitch && legs[1]->state == 0)) {
			move = legs[0]->targetMoveVec *(delta/mConfig->stepTime*2.f/mControl->timeScale);
			rot = delta/mConfig->stepTime*legs[1]->targetTurnAngle*2.f/mControl->timeScale;;
		}
		else 
		{
			move = legs[1]->targetMoveVec *(delta/mConfig->stepTime*2.f/mControl->timeScale);
			rot = delta/mConfig->stepTime*legs[0]->targetTurnAngle*2.f/mControl->timeScale;;
		}
	
		float h = mControl->rootHeight;// +sinf((posTime)*3.1415 * 2) * 5 * (mControl->moveDistance / 60);
		/// Inverse kinematics calculation
		rootMatrix = Matrix44();
		rootMatrix = Matrix44::translate(rootMatrix, Vector3(mControl->rootOffX, h, mControl->rootOffZ));

		rootMatrix = Matrix44::rotate(rootMatrix, mControl->rootRotX, Vector3(1, 0, 0));
		rootMatrix = Matrix44::rotate(rootMatrix, mControl->rootRotY, Vector3(0, 1, 0)); 
		rootMatrix = Matrix44::rotate(rootMatrix, mControl->rootRotZ, Vector3(0, 0, 1));


		for (int i = 0; i < 6; i++)
		{
			legs[i]->resolve(rootMatrix, posTime);

		}


	}
	void updateIK() 
	{
		float h = mControl->rootHeight;
		rootMatrix = Matrix44();
		rootMatrix = Matrix44::translate(rootMatrix, Vector3(mControl->rootOffX, h, mControl->rootOffZ));

		rootMatrix = Matrix44::rotate(rootMatrix, mControl->rootRotX, Vector3(1, 0, 0));
		rootMatrix = Matrix44::rotate(rootMatrix, mControl->rootRotY, Vector3(0, 1, 0));
		rootMatrix = Matrix44::rotate(rootMatrix, mControl->rootRotZ, Vector3(0, 0, 1));


		for (int i = 0; i < 6; i++)
		{
			legs[i]->resolveKinematics(rootMatrix);

		}
	
	}
	bool startHome =false;
	bool isHome = false;
	float currentTime =0;
	bool legSwitch = false;
	Vector3 move;
	float rot = 0;
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
	Matrix44 rootMatrix;




};