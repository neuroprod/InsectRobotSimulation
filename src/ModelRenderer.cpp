#include "ModelRenderer.h"
using namespace ci;
using namespace ci::app;
using namespace std;

void ModelRenderer::drawHome() 
{
	gl::color(1, 0, 0);
	gl::lineWidth(2);
	for (int i = 0; i < 6; i++) 
	{
		
		gl::pushMatrices();
		gl::translate(model->legs[i]->homePoint);
		gl::rotate(3.1415 / 2, 1, 0, 0);
		gl::drawLine(vec2(0,10),vec2(0,-10));
		gl::drawLine(vec2(10, 0), vec2(-10, 0));
		gl::drawStrokedCircle(vec2(0, 0), 5);
		gl::popMatrices();
	}
}
void ModelRenderer::drawResolveJoint1(NodeRef root)
{
	gl::color(0.5, 0.5, 0.5);
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


		gl::drawLine(vec3(0, 0, 0), model->legs[i]->targetPointLocalFlat);
		gl::drawLine(vec3(model->legs[i]->targetPointLocal), model->legs[i]->targetPointLocalFlat);
		gl::drawLine(vec3(0, 0, 0), vec3(0, 0, 150));
	
		gl::popMatrices();
	}
}

void ModelRenderer::drawResolveJoint23(NodeRef root)
{
	gl::color(0.5, 0.5, 0.5);
	gl::lineWidth(1);
	for (int i = 0; i < 6; i++)
	{
		gl::pushMatrices();
	//	gl::setModelMatrix(root->children[i]->globalMatrix);
		gl::setModelMatrix(model->legs[i]->shoulder1GlobalMatrix);
		gl::drawCoordinateFrame(20);

	


	
		gl::pushMatrices();
		gl::rotate(3.1415 / 2, vec3(1, 0, 0));
		gl::drawStrokedCircle(model->legs[i]->posTarget,  model->legs[i]->lowerLegSize);
		gl::drawStrokedCircle(model->legs[i]->posJoint, model->legs[i]->upperLegSize);
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