#pragma once
#include "Matrix44.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "ModelConfig.h"
#include <math.h>

class ModelLeg
{
public:
	ModelLeg() 
	{
	
	}
	void setBase(Vector3 position, Vector3 rotation, ModelConfig * config, bool flip)
	{
		mFlip = flip;
		baseMatrix = Matrix44();
		baseMatrix = Matrix44::translate(baseMatrix, position);
		baseMatrix = Matrix44::rotate(baseMatrix, rotation.x, Vector3(1, 0, 0));
		baseMatrix = Matrix44::rotate(baseMatrix, rotation.y, Vector3(0, 1, 0));
		baseMatrix = Matrix44::rotate(baseMatrix, rotation.z, Vector3(0, 0, 1));

		dir = Vector3(sin(rotation.y), 0, cos(rotation.y));


		homePoint = position + (dir*config->homeDistance);
		homePoint.y = 0;
		homeAngle  = atan2(homePoint.z, homePoint.x);
		homeRadius = Vector3::length(homePoint);

		targetPoint = homePoint;
		upperLegSize = config->upperLegSize;
		lowerLegSize = config->lowerLegSize;

		stepHeight = &config->stepHeight;
		stepPower = config->stepPower;
	

	}

	int state = 0;

	float homeAngle=0;
	float homeRadius=0;

	float oldDistance=0;



	Vector3 targetMoveVec = Vector3();
	float targetTurnAngle = 0;



	Vector3 newMoveVec = Vector3();
	float newTurnAngle=0;
	Vector3 oldMoveVec = Vector3();
	float oldTurnAngle = 0;


	void reset(Vector3 _newMoveVec,float _newTurnAngle)
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


	void resolve(Matrix44 &rootMatrix, float time)
	{
		
		
		float y = 0;
		

		if (isForward == 1)
		{
			y = 0;
			if (time < 0.5)
			{
				float stepr = 1 - time * 2;
				y = (1 - pow(stepr, stepPower))* (*stepHeight);
			}
			if (time >= 0.5)
			{
				float stepr = (time - 0.5) * 2;
				y = (1 - pow(stepr, stepPower)) * (*stepHeight);
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
	

		
		Vector3 moveVec = targetMoveVec*time;

		float angleStep = homeAngle+ targetTurnAngle*time;
		Vector3 angleVec = Vector3(cos(angleStep)*homeRadius, 0, sin(angleStep)*homeRadius) - homePoint;
		
		/////////////////////////////////
		/////////////////////////////////
		Vector3 move = moveVec + angleVec;
	

		targetPoint = homePoint+ move;
		
		targetPoint.y = y;


	
		resolveKinematics(rootMatrix);
	}
	void resolveKinematics(Matrix44 &rootMatrix)
	{

		//***show resolve Joint 1 in UI***

		Matrix44 inversMatrix = Matrix44::inverse(rootMatrix*baseMatrix);

		//transform targetpoint to joint1 local space
		targetPointLocal = inversMatrix*Vector4(targetPoint.x, targetPoint.y, targetPoint.z, 1);

		targetPointLocalFlat = Vector3(0, targetPointLocal.y, targetPointLocal.z);
		//calculate angle of joint 1
		shoulder1Angle = atan2(0, 1) - atan2(targetPointLocalFlat.y, targetPointLocalFlat.z);


		//***show resolve Joint 23 in UI***
		//transform target point to joint 1 local space after rotation
		Matrix44 shoulder1Matrix = Matrix44::rotate(baseMatrix, shoulder1Angle, Vector3(1, 0, 0));
		shoulder1GlobalMatrix = rootMatrix*shoulder1Matrix;
		inversMatrix = Matrix44::inverse(shoulder1GlobalMatrix);
		Vector4 targetPointLocal2 = inversMatrix*Vector4(targetPoint.x, targetPoint.y, targetPoint.z, 1);


		//reduce to 2D problem 
		posJoint = Vector2(0, 24);//0
		float r0 = upperLegSize;
		posTarget = Vector2(targetPointLocal2.x, targetPointLocal2.z);//1
		float r1 = lowerLegSize;
		// intersection 2 circles

		float dx = posTarget.x - posJoint.x;
		float dy = posTarget.y - posJoint.y;
		// Determine the straight-line distance between the centers. 
		float d = sqrt((dy*dy) + (dx*dx));

		if (d > (r0 + r1)) {
			// no solution. circles do not intersect. 
			
			return;
		}
		if (d < abs(r0 - r1)) {
			// no solution. one circle is contained in the other
			
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
		if (solution2.x > solution1.x)
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

	Vector3 targetPoint;



	//////////////////////////
	bool isSet1;
	bool isForward;
	float shoulder1Angle =0;
	float shoulder2Angle = 0;
	float shoulder3Angle = 0;
	float upperLegSize;
	float lowerLegSize;
	float * stepHeight;
	int stepPower;
	bool mFlip = false;

	Vector3 dir;
	Matrix44 baseMatrix;
	Vector3 homePoint;
	Vector4 targetPointLocal;
	Vector3 targetPointLocalFlat;

	Matrix44 shoulder1GlobalMatrix;
	Vector2 posJoint;
	Vector2 posTarget;
	
	Vector2 solution1;
	Vector2 solution2;

	Vector2 solution;
	
};