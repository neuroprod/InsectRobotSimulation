#include "ModelControlGui.h"
#include "CinderImGui.h"
void ModelControlGui::setup(ModelControl * _control) 
{
	control = _control;
}
void  ModelControlGui:: show() 
{
	ui::ScopedWindow window("Control");
	if (ui::DragFloat("move Distance", &control->moveDistance, 1, 0, 100)) control->isDirty = true;
	if (ui::DragFloat("move Angle", &control->moveAngle, 0.01, -3.1415, 3.1415)) control->isDirty = true;

	if (ui::DragFloat("turnAngle", &control->turnAngle, 0.01, -3.1415 / 4, 3.1415 / 4)) control->isDirty = true;

	ui::Separator();

	if (ui::DragFloat("body height", &control->rootHeight, 1, 0, 300)) control->isDirty = true;
	if (ui::DragFloat("body offset forward", &control->rootOffX, 1, -100, 100)) control->isDirty = true;
	if (ui::DragFloat("body offset sideways", &control->rootOffZ, 1, -100, 100)) control->isDirty = true;


	if (ui::DragFloat("body rotX", &control->rootRotX, 0.01, -3.1415, 3.1415)) control->isDirty = true;
	if (ui::DragFloat("body rotY", &control->rootRotY, 0.01, -3.1415, 3.1415)) control->isDirty = true;
	if (ui::DragFloat("body rotZ", &control->rootRotZ, 0.01, -3.1415, 3.1415)) control->isDirty = true;



	
	if (ui::Button("reset")) 
	{ 
		control->isDirty = true;
		control->rootOffX = 0;
		control->rootOffZ = 0;
		control->rootRotX = 0;
		control->rootRotY = 0;
		control->rootRotZ = 0;
	}
}