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
	ImGui::Separator();
	if (ui::DragFloat("Front Leg Angle", &config->frontLegAngle, 0.01,-3.1415, 3.1415)) config->isDirty = true;
	if (ui::DragFloat3("Front Leg Pos", &config->frontLegStart[0], 1)) config->isDirty = true;
	ImGui::Separator();
	if (ui::DragFloat("Middle Leg Angle", &config->middleLegAngle, 0.01, -3.1415, 3.1415)) config->isDirty = true;
	if (ui::DragFloat3("Middle Leg Pos", &config->middleLegStart[0], 1)) config->isDirty = true;
	ImGui::Separator();
	if (ui::DragFloat("Back Leg Angle", &config->backLegAngle, 0.01, -3.1415, 3.1415)) config->isDirty = true;
	if (ui::DragFloat3("Back Leg Pos", &config->backLegStart[0], 1)) config->isDirty = true;




	
	
}