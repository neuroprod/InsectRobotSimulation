#include "ModelRenderer.h"
using namespace ci;
using namespace ci::app;
using namespace std;

void ModelRenderer::drawHome() 
{
	gl::color(1, 0, 0);
	gl::lineWidth(1);
	for (int i = 0; i < 6; i++) 
	{
		
		gl::pushMatrices();
		gl::translate(model->legs[i]->homePoint);
		
		gl::drawLine(vec2(0,10),vec2(0,-10));
		gl::drawLine(vec2(10, 0), vec2(-10, 0));
		gl::drawLine(vec3(0, 0,10), vec3(0, 0,-10));
	




		gl::popMatrices();
	}
}
void ModelRenderer::drawMove()
{
	
	gl::lineWidth(1);
	ModelControl * control = model->mControl;
	for (int i = 0; i < 6; i++)
	{
	
		gl::pushMatrices();
		//float angleMove = control->moveAngle;
		//float angleTurn = model->legs[i]->angleTurn;



		vec3 home = model->legs[i]->startPoint;
		vec3 moveVec = model->legs[i]->targetMoveVec;

		float startAngle = model->legs[i]->startAngle; 
		float radius = model->legs[i]->turnRadius;
		float angle = model->legs[i]->targetTurnAngle;
		
		
		vec3 prevPosTurn = home;
		vec3 prevPosMove = home;
		vec3 prevPos = home;
		int numSteps = 20;

	
		for (int j = 0; j <= numSteps; j++) 
		{
			

			gl::color(0, 0, 1);
			float step = (float)j / (numSteps);
			

		

			vec3 posMove = home + moveVec*step;
			gl::drawLine(prevPosMove, posMove);
			prevPosMove = posMove;

			float nangle = startAngle + angle*step;
			vec3 posTurn = vec3(cos(nangle), 0, sin(nangle))*radius;
			gl::drawLine(posTurn, prevPosTurn);
			prevPosTurn = posTurn;


		

			//vec3 pos = posTurn + posMove - home;

			//posTurn.y = y;
			
			
			/*gl::color(1,1, 1);
		
			gl::drawLine(pos, prevPos);
			prevPos= pos;*/
			
		}

		gl::popMatrices();
		
	}
}
void ModelRenderer::drawResolveJoint1(NodeRef root)
{
	
	gl::lineWidth(1);
	for (int i = 0; i < 6; i++)
	{
		gl::pushMatrices();
		gl::setModelMatrix(root->children[i]->globalMatrixB);
		gl::drawCoordinateFrame(20);

		gl::pushMatrices();
		gl::translate(vec3(model->legs[i]->targetPointLocal));
		gl::drawCoordinateFrame(20);
		gl::popMatrices();

		gl::color(0, 1, 0);
		gl::drawLine(vec3(0, 0, 0), model->legs[i]->targetPointLocalFlat);
		gl::color(1, 1,1);
		gl::drawLine(vec3(0, 0, 0), vec3(0, 0, 150));
	
		gl::popMatrices();
	}
}

void ModelRenderer::drawResolveJoint23(NodeRef root)
{
	
	gl::lineWidth(1);
	for (int i = 0; i < 6; i++)
	{
		gl::color(1, 1, 1);
		gl::pushMatrices();
	//	gl::setModelMatrix(root->children[i]->globalMatrix);
		gl::setModelMatrix(model->legs[i]->shoulder1GlobalMatrix);
		gl::drawCoordinateFrame(20);

	


	
		gl::pushMatrices();
		gl::rotate(3.1415 / 2, vec3(1, 0, 0));
		gl::drawStrokedCircle(model->legs[i]->posTarget,  model->legs[i]->lowerLegSize);
		gl::drawStrokedCircle(model->legs[i]->posJoint, model->legs[i]->upperLegSize);

		gl::drawLine(model->legs[i]->posTarget, model->legs[i]->posJoint);
		gl::drawLine(model->legs[i]->solution1, model->legs[i]->solution2);
		gl::color(1, 0, 0);
		gl::drawSolidCircle(model->legs[i]->solution1, 3);
		gl::drawSolidCircle(model->legs[i]->solution2, 3);
		
		gl::color(0, 1, 0);
		gl::drawLine(model->legs[i]->solution, model->legs[i]->posJoint);
		gl::drawLine(model->legs[i]->posTarget, model->legs[i]->solution);
		
		gl::popMatrices();
		
		gl::popMatrices();
	}
}
/*
void ModelRenderer::drawHomeInverse()
{
	gl::color(0, 0, 1);
	gl::lineWidth(2);
	for (int i = 0; i < 6; i++)
	{

		gl::pushMatrices();
	
		gl::translate(vec3(model->legs[i]->targetPointR));
		gl::rotate(3.1415 / 2, 1, 0, 0);
		gl::drawLine(vec2(0, 10), vec2(0, -10));
		gl::drawLine(vec2(10, 0), vec2(-10, 0));
		gl::drawStrokedCircle(vec2(0, 0), 5);
		gl::popMatrices();
	}
}
void ModelRenderer::drawSholder()
{
	gl::color(0, 1, 0);
	gl::lineWidth(2);
	for (int i = 0; i < 6; i++)
	{

		gl::pushMatrices();
		gl::translate(vec3(model->legs[i]->sholderPointR));
		gl::rotate(3.1415 / 2, 1, 0, 0);
		gl::drawLine(vec2(0, 10), vec2(0, -10));
		gl::drawLine(vec2(10, 0), vec2(-10, 0));
		gl::drawStrokedCircle(vec2(0, 0), 5);
		gl::popMatrices();

		gl::pushMatrices();
		gl::translate(vec3(model->legs[i]->sholderPointR2));
		gl::rotate(3.1415 / 2, 1, 0, 0);
		gl::drawLine(vec2(0, 10), vec2(0, -10));
		gl::drawLine(vec2(10, 0), vec2(-10, 0));
		gl::drawStrokedCircle(vec2(0, 0), 5);
		gl::popMatrices();
	}
}*/