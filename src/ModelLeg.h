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
		glm::mat4 InversMatrix = glm::inverse(rootMatrix*baseMatrix);
		targetPointLocal = InversMatrix*glm::vec4(targetPoint.x, targetPoint.y, targetPoint.z, 1);
		
		targetPointLocalFlat = glm::vec3(0, targetPointLocal.y, targetPointLocal.z);
		
	
		shoulder1Angle =  atan2(0, 1)- atan2(targetPointLocalFlat.y, targetPointLocalFlat.z);
		
		console() << targetPointLocalFlat << shoulder1Angle << endl;
		/*
		//vertical and horizontal distance between target and second joint
		sholderPointR2 = rootMatrix*baseMatrix*glm::vec4(0, 0, 24, 1); //second joint in targetR space
		dy= sholderPointR2.y - targetPointR.y;//height
		sholderPointR2.y = 0;
		targetPointR.y = 0;
		dx = glm::distance(sholderPointR2, targetPointR);
		console() << dx << "-" << dy << endl;
		//solve 2D circle intersection (optimised for this case)
	
		
		float d =sqrt((dy*dy) + (dx*dx));

		if (d > (upperLegSize + lowerLegSize)) {
			
			console() << "no solution" << endl;
			return;
		}
		if (d < abs(upperLegSize - lowerLegSize)) {
			console() << "no solution" << endl;
			return ;
		}
		//dist to center
		float a = ((upperLegSize*upperLegSize)- (lowerLegSize*lowerLegSize) + (d*d)) / (2.0 * d);
	//center
		float xm =  (dx * a / d);
		float ym = dy + (dy * a / d);
	
		
		float rx = -dy * (a / d);
		float ry = dx * (a / d);
		float xi = xm + rx;
		float yi = ym + ry;

		console()<<d<<" "<<a<<" " << xm <<","<<ym << endl;
		*/
	}

	float shoulder1Angle =0;

	float upperLegSize;
	float lowerLegSize;



	glm::vec3 dir;
	glm::mat4 baseMatrix;
	glm::vec3 homePoint;
	glm::vec4 targetPointLocal;
	glm::vec3 targetPointLocalFlat;
	/*glm::vec4 targetPointR;
	glm::vec4 sholderPointR;
	glm::vec4 sholderPointR2;*/
};