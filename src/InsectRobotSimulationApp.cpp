



#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Node.h"
#include "Leg.h"
#include "DebugRenderer.h"
#include "MeshPool.h"
#include "ModelConfig.h"
#include "ModelConfigGui.h"
#include "ModelControl.h"
#include "ModelControlGui.h"
#include "CinderImGui.h"
#include "Model.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class InsectRobotSimulationApp : public App {
  public:
	void setup() override;


	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void buildRobot();
	void updateGui();

	NodeRef root;

	LegRef frontRight;
	LegRef middleRight;
	LegRef backRight;

	LegRef frontLeft;
	LegRef middleLeft;
	LegRef backLeft;

	ModelControl control;
	ModelControlGui controlGui;

	ModelConfig config;
	ModelConfigGui configGui;

	DebugRenderer renderer;

	Model model;
};

void InsectRobotSimulationApp::setup()
{
	setWindowSize(1920, 1080);
	

	ui::initialize();
	ui::GetStyle().WindowRounding = 0.0f;
	ui::GetStyle().ChildRounding = 0.0f;
	ui::GetStyle().FrameRounding = 0.0f;
	ui::GetStyle().GrabRounding = 0.0f;
	ui::GetStyle().PopupRounding = 0.0f;
	ui::GetStyle().ScrollbarRounding = 0.0f;


	MP()->setup(&config);
	root = Node::create();
	root->setBase(vec3(0,control.rootHeight, 0), vec3(0, 0, 0));
	root->setRotation(0);


	buildRobot();

	
	renderer.setup(root,&model);
	controlGui.setup(&control);
	configGui.setup(&config);


}
void InsectRobotSimulationApp::buildRobot() 
{
	MP()->clear();
	root->removeAllChildren();

	frontRight = Leg::create();
	frontRight->setup("FR", root, config.frontLegStart, config.frontLegAngle, false, &config);

	middleRight = Leg::create();
	middleRight->setup("MR", root, config.middleLegStart, config.middleLegAngle, false, &config);

	backRight = Leg::create();
	backRight->setup("BR", root, config.backLegStart, config.backLegAngle, false, &config);


	frontLeft = Leg::create();
	vec3 frontstart = config.frontLegStart;
	frontstart.z *= -1;
	frontLeft->setup("FL", root, frontstart, -config.frontLegAngle +3.1415, true, &config);

	vec3 middlestart = config.middleLegStart;
	middlestart.z *= -1;
	middleLeft = Leg::create();
	middleLeft->setup("ML", root, middlestart, -config.middleLegAngle + 3.1415, true, &config);

	vec3 backstart = config.backLegStart;
	backstart.z *= -1;
	backLeft = Leg::create();
	backLeft->setup("BL", root, backstart, -config.backLegAngle + 3.1415, true, &config);

}
void InsectRobotSimulationApp::keyDown(KeyEvent event)
{

	if (event.getCode() == KeyEvent::KEY_p) {

		time_t t = std::time(0);
		long int now = static_cast<long int> (t);
		ci::writeImage(getAssetPath("/screens/").string() + to_string(now) + ".png", copyWindowSurface());
		console() << "printscreen" << endl;
	}
}


void InsectRobotSimulationApp::update()
{
	updateGui();

	if (config.isDirty) {
		model.setConfig(&config);
		buildRobot();
		control.isDirty = true;
	}
	if (control.isDirty) {
		root->setBase(vec3(0, control.rootHeight, 0),vec3(control.rootRotX, control.rootRotY, control.rootRotZ));
		model.setControl(&control);
	}
	model.update();
	for (int i = 0; i < 6; i++) 
	{
		if (i == 0) {
		//	console() << model.legs[i]->shoulder1Angle << endl;
		}
		root->children[i]->setRotation(model.legs[i]->shoulder1Angle);
	
	
	}

	root->update();
	renderer.update();

	config.isDirty = false;
	control.isDirty = false;

}
void InsectRobotSimulationApp::updateGui() 
{
	static bool showDemoWindow = false;
	static bool showRenderWindow = true;
	static bool showControlGui = true;
	static bool showConfigGui = true;
	{
		ui::ScopedMainMenuBar menuBar;
		if (ui::BeginMenu("Window")) {
			ui::MenuItem("View", nullptr, &showRenderWindow);
			ui::MenuItem("Control", nullptr, &showControlGui);
			ui::MenuItem("Config", nullptr, &showConfigGui);
			//ui::MenuItem("TestWindow", nullptr, &showDemoWindow);
			ui::EndMenu();
		}
	}
	if (showDemoWindow) {
		ui::ShowDemoWindow();
	}
	if (showRenderWindow) {
		renderer.showRenderWindow();
	}
	if (showControlGui) {
		controlGui.show();
	}
	if (showConfigGui) 
	{
		configGui.show();
	}

}
void InsectRobotSimulationApp::draw()
{
	gl::clear( Color( 0, 0, 0) );
	renderer.draw();
}

CINDER_APP( InsectRobotSimulationApp, RendererGl(RendererGl::Options().msaa(16)))
