

#define LOCKAL_DEV

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
#include "cinder/Serial.h"


#include "cinder/Log.h"
using namespace ci;
using namespace ci::app;
using namespace std;

union U {
	int16_t s;

	struct Byte {
		int8_t c1, c2;
	}
	byte;
};


class InsectRobotSimulationApp : public App {
  public:
	void setup() override;


	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void buildRobot();
	void updateGui();
	void updateSerial();
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
	double previousTime;

	vector<LegRef> legs;
	bool useIK=true;
	bool useWalk =false;
	int statsSelection1 = 0;
	int statsType1 = 0;
	int statsSelection2 = 3;
	int statsType2 = 0;
	bool showStats = true;

	int8_t input[16];
	int8_t inByte;
	int inCount;
	SerialRef	mSerialIn;

	SerialRef	mSerialOut;
};

void InsectRobotSimulationApp::setup()
{
	try {
		Serial::Device dev = Serial::Device("COM6");
		
		mSerialOut = Serial::create(dev, 115200);
		if (mSerialOut->getNumBytesAvailable() > 0)
		{
			mSerialOut->flush();
		}
	}
	catch (SerialExc &exc) {
		CI_LOG_EXCEPTION("coult not initialize the serial OUT", exc);
		
	}
	try {
		Serial::Device dev = Serial::Device("COM5");

		mSerialIn = Serial::create(dev, 115200);
		if (mSerialIn->getNumBytesAvailable() > 0)
		{
			mSerialIn->flush();
		}
	}
	catch (SerialExc &exc) {
		CI_LOG_EXCEPTION("coult not initialize the serial IN", exc);

	}

	setWindowSize(1920, 1080);
	setWindowPos(0, 0);
	gl::enableVerticalSync();
	setFrameRate(30);
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
	root->addMesh(MP()->getMesh("body"));

	buildRobot();

	
	renderer.setup(root,&model);
	controlGui.setup(&control);
	configGui.setup(&config);

}
void InsectRobotSimulationApp::buildRobot() 
{
	MP()->clear();
	root->removeAllChildren();
	legs.clear();

	frontRight = Leg::create();
	frontRight->setup("FR", root, config.frontLegStart, config.frontLegAngle, false, &config);
	legs.push_back(frontRight);
	middleRight = Leg::create();
	middleRight->setup("MR", root, config.middleLegStart, config.middleLegAngle, false, &config);
	legs.push_back(middleRight);
	backRight = Leg::create();
	backRight->setup("BR", root, config.backLegStart, config.backLegAngle, false, &config);
	legs.push_back(backRight);

	frontLeft = Leg::create();
	vec3 frontstart = config.frontLegStart;
	frontstart.z *= -1;
	frontLeft->setup("FL", root, frontstart, -config.frontLegAngle +3.1415, true, &config);
	legs.push_back(frontLeft);

	vec3 middlestart = config.middleLegStart;
	middlestart.z *= -1;
	middleLeft = Leg::create();
	middleLeft->setup("ML", root, middlestart, -config.middleLegAngle + 3.1415, true, &config);
	legs.push_back(middleLeft);

	vec3 backstart = config.backLegStart;
	backstart.z *= -1;
	backLeft = Leg::create();
	backLeft->setup("BL", root, backstart, -config.backLegAngle + 3.1415, true, &config);
	legs.push_back(backLeft);
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
		root->setBase(vec3(control.rootOffX, control.rootHeight, control.rootOffZ),vec3(control.rootRotX, control.rootRotY, control.rootRotZ));
		model.setControl(&control);
	}
	double currentTime = getElapsedSeconds();
	float delta =  currentTime-previousTime;

	previousTime = currentTime;
	if (useWalk && useIK) {
		model.update(delta);
	}
	else 
	{
		model.updateIK();
	}
	root->baseMatrix = model.rootMatrix;
	for (int i = 0; i < 6; i++) 
	{
		if (useWalk && useIK) {
			legs[i]->setRotationData(model.legs[i]->shoulder1Angle, model.legs[i]->shoulder2Angle, model.legs[i]->shoulder3Angle);
		}
		else if (useIK) {
			legs[i]->setRotationData(model.legs[i]->shoulder1Angle, model.legs[i]->shoulder2Angle, model.legs[i]->shoulder3Angle);
		}
		legs[i]->update();
	
	}
	
	root->update();
	renderer.update(model.move,model.rot);

	config.isDirty = false;
	control.isDirty = false;
	updateSerial();
}
void InsectRobotSimulationApp::updateSerial()
{
	if (mSerialIn) 
	{
		while (mSerialIn->getNumBytesAvailable() > 0)
		{
			int8_t currentByte = mSerialIn->readByte();
			
			if ((int)inByte == 100 && currentByte == 100)
			{
				inCount = 0;
			}
			else
			{
				input[inCount] = currentByte;
				
				inCount++;
				if (inCount == 14)
				{
					U u;
					u.byte.c1 = input[0];
					u.byte.c2 = input[1];
					float lX = u.s;
				
					u.byte.c1 = input[2];
					u.byte.c2 = input[3];
					float lY = u.s;

					u.byte.c1 = input[4];
					u.byte.c2 = input[5];
					float lZ = u.s;

					u.byte.c1 = input[6];
					u.byte.c2 = input[7];
					float rX = u.s;

					u.byte.c1 = input[8];
					u.byte.c2 = input[9];
					float rY = u.s;

					u.byte.c1 = input[10];
					u.byte.c2 = input[11];
					float rZ = u.s;

					vec2 r = vec2(rX, rY);
					float size =min(1000.f, glm::length(r))/1000.f;
					float angle = atan2(r.x, r.y);

					control.isDirty = true;
					control.moveAngle = angle;
					control.moveDistance = size * 45;
					control.turnAngle = rZ / 1000.0f *0.13f;
					if(lY<0){
						control.timeScale =1+ lY / -500;
					}
					else {
						control.timeScale =1.f - (lY /1000.f*0.8f);
					}

					

					


					inCount = 0;
					

				}
			}
			inByte = currentByte;

		}
	}

	
	if (mSerialOut)
	{
		if (mSerialOut->getNumBytesAvailable() > 1)
		{
			mSerialOut->flush();
			//console() << mSerial->readStringUntil('\n') << endl;
		}
		const int numPos = 38;
		int8_t outBuffer[numPos];
		int posCount = 0;
		outBuffer[posCount++] = 100;
		outBuffer[posCount++] = 100;
		float adj =- 0.141;
		U u;
		for (int i = 0; i < 6; i++)
		{
			float factor = 1;
			if (i > 2)factor = -1;

			int s1 =-1* legs[i]->shoulder1Angle / 3.14159 * 2048 + 2048;
			u.s = s1;
			outBuffer[posCount++] =u.byte.c1;
			outBuffer[posCount++] = u.byte.c2;


			int s2 =factor* legs[i]->shoulder2Angle / 3.14159 * 2048 + 2048;
			u.s = s2;
			outBuffer[posCount++] = u.byte.c1;
			outBuffer[posCount++] = u.byte.c2;

			int s3 = (legs[i]->kneeAngle+ adj*factor) / 3.14159 * 2048 + 2048;
			u.s = s3;
			outBuffer[posCount++] = u.byte.c1;
			outBuffer[posCount++] = u.byte.c2;
		
		}
	
		mSerialOut->writeBytes(outBuffer, numPos);
	
	
	}
}
void InsectRobotSimulationApp::updateGui() 
{

	static bool showDemoWindow = false;
	static bool showRenderWindow = true;
	static bool showControlGui = true;
	static bool showConfigGui = true;
	static bool showLegs= true;
	{
		ui::ScopedMainMenuBar menuBar;
		if (ui::BeginMenu("Window")) {
			ui::MenuItem("View", nullptr, &showRenderWindow);
			ui::MenuItem("Control", nullptr, &showControlGui);
			ui::MenuItem("Config", nullptr, &showConfigGui);
			ui::MenuItem("Legs", nullptr, &showLegs);
			ui::MenuItem("Stats", nullptr, &showStats);
			//ui::MenuItem("TestWindow", nullptr, &showDemoWindow);
			ui::EndMenu();
		}
	}
	if (showDemoWindow) {
		ui::ShowDemoWindow();
	}
	if (showRenderWindow) {
		renderer.showRenderWindow(getAverageFps());
	}
	if (showControlGui) {
		controlGui.show();
	}
	if (showConfigGui) 
	{
		configGui.show();
	}
	if (showLegs)
	{
		
		ui::ScopedWindow window("Legs");
		ui::Checkbox("use IK", &useIK); 
		ui::SameLine();
		ui::Checkbox("use Walk", &useWalk);
		if (!useIK) {
			for (int i = 0; i < 6; i++)
			{
				string name = legs[i]->mName;

				ImGui::Text(name.c_str());

				ImGui::Indent();

				ImGui::SliderFloat((name + "_hipUD").c_str(), &legs[i]->shoulder1Angle, -1.f, 1.f);
				ImGui::SliderFloat((name + "_hipLR").c_str(), &legs[i]->shoulder2Angle, -1.f, 1.f);
				ImGui::SliderFloat((name + "_knee").c_str(), &legs[i]->kneeAngle, -1.f, 1.f);

				ImGui::Unindent();


			}
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				string name = legs[i]->mName;
				ImGui::SliderFloat3(name.c_str(), &model.legs[i]->targetPoint[0], -300,300);

			}
		
		}
	}
	if (showStats) {
		ui::ScopedWindow window("stats");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "hipUD"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "hipLR"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(.0f, 0.0f, 1.0f, 1.0f), "knee");
		ImGui::Combo("leg1", &statsSelection1, "Front right\0Middle Right\0Back Right\0Front Left\0Middle Left\0Back Left\0\0");
		ImGui::Combo("type1", &statsType1, "rotation\0speed\0acceleration\0\0");

		ImGui::Combo("leg2", &statsSelection2, "Front right\0Middle Right\0Back Right\0Front Left\0Middle Left\0Back Left\0\0");
		ImGui::Combo("type2", &statsType2, "rotation\0speed\0acceleration\0\0");
	}
}
void InsectRobotSimulationApp::draw()
{
	gl::clear( Color( 0, 0, 0) );
	renderer.draw();
	if (showStats) 
	{
		gl::pushMatrices();
		gl::translate(vec2(10, 770));
		gl::disableDepthWrite();
		gl::pushMatrices();
		
		gl::color(ColorA(0.1, 0.1, 0.1, 0.8));
		gl::drawSolidRect(Rectf(0, 0, 1040, 300));
		gl::color(ColorA(0, 0, 0, 1));
		gl::drawLine(vec2(0, 150), vec2(1040, 150));
		gl::color(ColorA(1, 1, 1, 0.5));
		gl::drawLine(vec2(0, 75), vec2(1040, 75));
		gl::drawLine(vec2(0,225), vec2(1040, 225));
		gl::popMatrices();

		gl::pushMatrices();
		gl::translate(vec2(20, 950-75-800));
		gl::drawString(legs[statsSelection1]->mName,vec2(-10,-70));
		legs[statsSelection1]->draw(statsType1);
		gl::popMatrices();
		
	
		gl::pushMatrices();
		gl::translate(vec2(20, 950+75-800));
		gl::drawString(legs[statsSelection2]->mName, vec2(-10, -70));
		legs[statsSelection2]->draw(statsType2);
		gl::popMatrices();
		gl::enableDepthWrite();
		gl::popMatrices();
	}
	
}

CINDER_APP( InsectRobotSimulationApp, RendererGl(RendererGl::Options().msaa(16)))
