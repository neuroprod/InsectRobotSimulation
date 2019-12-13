#pragma once
#include "ModelConfig.h"
class ModelConfigGui
{

public:
	ModelConfigGui() {};
	void setup(ModelConfig * config);
	void show();

	ModelConfig * config;


};