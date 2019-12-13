#pragma once
#include "ModelControl.h"
class ModelControlGui 
{

public:
	ModelControlGui() {};
	void setup(ModelControl * control);
	void show();
	
	ModelControl * control;


};