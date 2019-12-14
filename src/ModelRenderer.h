#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Model.h" 
#include "Node.h" 
class ModelRenderer 
{
public:
	ModelRenderer() {}
	Model * model;
	void drawHome();
	void drawResolveJoint1(NodeRef root);
	void drawResolveJoint23(NodeRef root);

};