#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Node.h"
#include "Leg.h"
#include "DebugRenderer.h"
#include "MeshPool.h"

#include "CinderImGui.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class InsectRobotSimulationApp : public App {
  public:
	void setup() override;
	void mouseDown(MouseEvent event) override;

	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;


	NodeRef root;

	LegRef frontRight;
	LegRef middleRight;
	LegRef backRight;

	LegRef frontLeft;
	LegRef middleLeft;
	LegRef backLeft;



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


	MP()->setup();


	root = Node::create();
	root->setBase(vec3(0, 130, 0), vec3(0, 0, 0));
	root->setRotation(0);


	frontRight = Leg::create();
	frontRight->setup("FR", root, vec3(70, 0, 80), 3.1415f / 4.f, false);

	middleRight = Leg::create();
	middleRight->setup("MR", root, vec3(0, 0, 100), 0, false);

	backRight = Leg::create();
	backRight->setup("BR", root, vec3(-70, 0, 80), -3.1415f / 4.f, false);


	frontLeft = Leg::create();
	frontLeft->setup("FL", root, vec3(70, 0, -80), 3.1415f / 4.f * 3, true);

	middleLeft = Leg::create();
	middleLeft->setup("ML", root, vec3(0, 0, -100), 3.1415f, true);

	backLeft = Leg::create();
	backLeft->setup("BL", root, vec3(-70, 0, -80), -3.1415f / 4.f * 3, true);


	renderer.setup(root);
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
void InsectRobotSimulationApp::mouseDown( MouseEvent event )
{
}

void InsectRobotSimulationApp::update()
{
	root->update();
	renderer.update();


	static bool showDemoWindow = false;
	static bool showRenderWindow = false;
	{
		ui::ScopedMainMenuBar menuBar;
		if (ui::BeginMenu("View")) {
			ui::MenuItem("RenderWindow", nullptr, &showRenderWindow);
			ui::MenuItem("TestWindow", nullptr, &showDemoWindow);
			ui::EndMenu();
		}
	}
	if (showDemoWindow) {
		ui::ShowDemoWindow();
	}
	if (showRenderWindow) {
		renderer.showRenderWindow();
	}
}

void InsectRobotSimulationApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	renderer.draw();
}

CINDER_APP( InsectRobotSimulationApp, RendererGl(RendererGl::Options().msaa(16)))
