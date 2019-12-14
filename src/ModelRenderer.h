#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Model.h" 
class ModelRenderer 
{
public:
	ModelRenderer() {}
	Model * model;
	void drawHome();
};