#include "ModelControlGui.h"
#include "CinderImGui.h"
void ModelControlGui::setup(ModelControl * _control) 
{
	control = _control;
}
void  ModelControlGui:: show() 
{
	ui::ScopedWindow window("Control");
	if (ui::DragFloat("Camera Theta", &control->rootHeight, 1, 0, 300)) control->isDirty = true;
}