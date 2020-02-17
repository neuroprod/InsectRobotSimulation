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
	void setBase(glm::vec3 position, glm::vec3 rotation, ModelConfig * config, bool flip)
	{
		mFlip = flip;
		baseMatrix = glm::mat4();
		baseMatrix = glm::translate(baseMatrix, position);
		baseMatrix = glm::rotate(baseMatrix, rotation.x, glm::vec3(1, 0, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.y, glm::vec3(0, 1, 0));
		baseMatrix = glm::rotate(baseMatrix, rotation.z, glm::vec3(0, 0, 1));

		dir = glm::vec3(sin(rotation.y), 0, cos(rotation.y));


		homePoint = position + (dir*config->homeDistance);
		homePoint.y = 0;
		homeAngle  = atan2(homePoint.z, homePoint.x);
		homeRadius = length(homePoint);

		targetPoint = homePoint;
		upperLegSize = config->upperLegSize;
		lowerLegSize = config->lowerLegSize;

		stepHeight = config->stepHeight;
		stepPower = config->stepPower;


	}
	int state = 0;

	float homeAngle;
	float homeRadius;

	float oldDistance;



	glm::vec3 targetMoveVec = glm::vec3();
	float targetTurnAngle = 0;



	glm::vec3 newMoveVec;
	float newTurnAngle;
	glm::vec3 oldMoveVec = glm::vec3();
	float oldTurnAngle = 0;


	void reset(glm::vec3 _newMoveVec,float _newTurnAngle) 
	{
		newMoveVec = _newMoveVec;
		newTurnAngle = _newTurnAngle;
		state = 0;
			
	

	

	}
	void setNextState() 
	{
		state = 1;
		
		targetTurnAngle =  newTurnAngle;
		
		targetMoveVec = newMoveVec;

		
	



		oldMoveVec = newMoveVec;
		oldTurnAngle = newTurnAngle;
		
	}


	void resolve(glm::mat4 &rootMatrix, float time)
	{
		
		float y = 0;
		

		if (isForward == 1)
		{
			y = 0;
			if (time < 0.5)
			{
				float stepr = 1 - time * 2;
				y = (1 - pow(stepr, stepPower))* stepHeight;
			}
			if (time >= 0.5)
			{
				float stepr = (time - 0.5) * 2;
				y = (1 - pow(stepr, stepPower)) * stepHeight;
			}

		}
	

		if (time < 0.5) 
		{
			time =1- time * 2;
		
		}
		else
		{
			if (state == 0) 
			{
				setNextState();	
				time = 0.5;
			}
			time = (time - 0.5)*2;	
		}
	

		
		glm::vec3 moveVec = targetMoveVec*time;

		float angleStep = homeAngle+ targetTurnAngle*time;
		glm::vec3 angleVec = glm::vec3(cos(angleStep)*homeRadius, 0, sin(angleStep)*homeRadius) - homePoint;
		
		/////////////////////////////////
		/////////////////////////////////
		glm::vec3 move = moveVec + angleVec;
	

		targetPoint = homePoint+ move;
		
		targetPoint.y = y;


	
		resolveKinematics(rootMatrix);
	}
	void resolveKinematics(glm::mat4 &rootMatrix)
	{

		//***show resolve Joint 1 in UI***

		glm::mat4 inversMatrix = glm::inverse(rootMatrix*baseMatrix);
		//transform targetpoint to joint1 local space
		targetPointLocal = inversMatrix*glm::vec4(targetPoint.x, targetPoint.y, targetPoint.z, 1);

		targetPointLocalFlat = glm::vec3(0, targetPointLocal.y, targetPointLocal.z);

		shoulder1Angle = atan2(0, 1) - atan2(targetPointLocalFlat.y, targetPointLocalFlat.z);


		//***show resolve Joint 23 in UI***
		//transform target point to joint 1 local space after rotation
		glm::mat4 shoulder1Matrix = glm::rotate(baseMatrix, shoulder1Angle, glm::vec3(1, 0, 0));
		shoulder1GlobalMatrix = rootMatrix*shoulder1Matrix;
		inversMatrix = glm::inverse(shoulder1GlobalMatrix);
		glm::vec4 targetPointLocal2 = inversMatrix*glm::vec4(targetPoint.x, targetPoint.y, targetPoint.z, 1);


		//reduce to 2D problem 
		posJoint = glm::vec2(0, 24);//0
		float r0 = upperLegSize;
		posTarget = glm::vec2(targetPointLocal2.x, targetPointLocal2.z);//1
		float r1 = lowerLegSize;
		// intersection 2 circles

		float dx = posTarget.x - posJoint.x;
		float dy = posTarget.y - posJoint.y;
		// Determine the straight-line distance between the centers. 
		float d = sqrt((dy*dy) + (dx*dx));

		if (d > (r0 + r1)) {
			// no solution. circles do not intersect. 
			console() << "no solution" << endl;
			return;
		}
		if (d < abs(r0 - r1)) {
			// no solution. one circle is contained in the other
			console() << "no solution" << endl;
			return;
		}
		/* 'point 2' is the point where the line through the circle
		* intersection points crosses the line between the circle
		* centers.
		*/

		//Determine the distance from point 0 to point 2. 
		float a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d);

		//Determine the coordinates of point 2. 
		float x2 = posJoint.x + (dx * a / d);
		float y2 = posJoint.y + (dy * a / d);

		//Determine the distance from point 2 to either of the intersection points.

		float h = sqrt((r0*r0) - (a*a));


		//Now determine the offsets of the intersection points from point 2.

		float rx = -dy * (h / d);
		float ry = dx * (h / d);

		//Determine the absolute intersection points. 2 solutions
		solution1.x = x2 + rx;
		solution2.x = x2 - rx;

		solution1.y = y2 + ry;
		solution2.y = y2 - ry;

		//we want the top solution
		if (solution2.y > solution1.y)
		{
			solution = solution2;
		}
		else
		{
			solution = solution1;
		}


		//final angles

		shoulder2Angle = atan2(solution.y - posJoint.y, solution.x - posJoint.x) - atan2(1, 0);
		if (mFlip)  shoulder2Angle *= -1;
		shoulder3Angle = atan2(posJoint.y - solution.y, posJoint.x - solution.x) - atan2(posTarget.y - solution.y, posTarget.x - solution.x);
		shoulder3Angle = -shoulder3Angle + 3.1415 / 2;
		if (mFlip)  	shoulder3Angle *= -1;
	}

	glm::vec3 targetPoint;



	//////////////////////////
	bool isSet1;
	bool isForward;
	float shoulder1Angle =0;
	float shoulder2Angle = 0;
	float shoulder3Angle = 0;
	float upperLegSize;
	float lowerLegSize;
	float stepHeight;
	int stepPower;
	bool mFlip = false;

	glm::vec3 dir;
	glm::mat4 baseMatrix;
	glm::vec3 homePoint;
	glm::vec4 targetPointLocal;
	glm::vec3 targetPointLocalFlat;

	glm::mat4 shoulder1GlobalMatrix;
	glm::vec2 posJoint;
	glm::vec2 posTarget;
	
	glm::vec2 solution1;
	glm::vec2 solution2;

	glm::vec2 solution;
	
};