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

	
	renderer.setup(root);
	controlGui.setup(&control);
	configGui.setup(&config);


}
void InsectRobotSimulationApp::buildRobot() 
{
	MP()->clear();
	root->removeAllChildren();

	frontRight = Leg::create();
	frontRight->setup("FR", root, vec3(70, 0, 80), 3.1415f / 4.f, false, &config);

	middleRight = Leg::create();
	middleRight->setup("MR", root, vec3(0, 0, 100), 0, false, &config);

	backRight = Leg::create();
	backRight->setup("BR", root, vec3(-70, 0, 80), -3.1415f / 4.f, false, &config);


	frontLeft = Leg::create();
	frontLeft->setup("FL", root, vec3(70, 0, -80), 3.1415f / 4.f * 3, true, &config);

	middleLeft = Leg::create();
	middleLeft->setup("ML", root, vec3(0, 0, -100), 3.1415f, true, &config);

	backLeft = Leg::create();
	backLeft->setup("BL", root, vec3(-70, 0, -80), -3.1415f / 4.f * 3, true, &config);

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
		buildRobot();
	}
	if (control.isDirty) {
		root->setBase(vec3(0, control.rootHeight, 0));
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
	gl::clear( Color( 0, 0, 0 ) ); 
	renderer.draw();
}

CINDER_APP( InsectRobotSimulationApp, RendererGl(RendererGl::Options().msaa(16)))
