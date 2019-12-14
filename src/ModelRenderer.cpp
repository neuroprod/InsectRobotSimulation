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