#include "ModelConfigGui.h"
#include "CinderImGui.h"
void ModelConfigGui::setup(ModelConfig * _config)
{
	config = _config;
}
void  ModelConfigGui::show()
{
	ui::ScopedWindow window("Config");
	if (ui::DragFloat("Upper Leg Size", &config->upperLegSize, 1, 0, 300)) config->isDirty = true;
	if (ui::DragFloat("Lower Leg Size", &config->lowerLegSize, 1, 0, 300)) config->isDirty = true;
}