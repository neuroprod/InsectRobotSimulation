#include "ModelControlGui.h"
#include "CinderImGui.h"
void ModelControlGui::setup(ModelControl * _control) 
{
	control = _control;
}
void  ModelControlGui:: show() 
{
	ui::ScopedWindow window("Control");
	if (ui::DragFloat("body height", &control->rootHeight, 1, 0, 300)) control->isDirty = true;

	if (ui::DragFloat("body rotX", &control->rootRotX, 0.01, -3.1415, 3.1415)) control->isDirty = true;
	if (ui::DragFloat("body rotY", &control->rootRotY, 0.01, -3.1415, 3.1415)) control->isDirty = true;
	if (ui::DragFloat("body rotZ", &control->rootRotZ, 0.01, -3.1415, 3.1415)) control->isDirty = true;
}