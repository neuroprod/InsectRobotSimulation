#include "Leg.h"
#include "MeshPool.h"

using namespace ci;
using namespace ci::app;
using namespace std;

LegRef Leg::create()
{
	LegRef ref = std::make_shared<Leg>();
	return ref;
}

void Leg::setup(std::string name, NodeRef root, ci::vec3 startPos, float startRot, bool flip, ModelConfig * config)
{
	mName = name;

	sholderLR = Node::create();
	sholderLR->setBase(startPos, vec3(0, startRot, 3.1415 / 2));
	sholderLR->setRotation(0);
	sholderLR->addMesh(MP()->getMesh("DMXaxis"));
	sholderLR->addMesh(MP()->getMesh("2XL430"));
	sholderLR->name = mName + "_sholderLR";
	root->addChild(sholderLR);


	float startAngle = -3.1415 / 2;
	if (flip) startAngle *= -1;

	
	sholderUD = Node::create();
	sholderUD->setBase(vec3(0, 0, 24), vec3(0, 0, startAngle));
	sholderUD->setRotation(0);
	sholderUD->addMesh(MP()->getMesh("DMXaxis"));
	sholderUD->addMesh(MP()->getMesh("XC430"));
	sholderUD->name = mName + "_sholderUD";
	sholderLR->addChild(sholderUD);
	
	knee = Node::create();
	knee->setBase(vec3(0, 0, config->upperLegSize), vec3(3.1415 / 2, 0, 0));
	knee->setRotation(0);
	knee->name = mName + "_knee";
	knee->addMesh(MP()->getMesh("DMXaxis"));
	if (flip) 
	{
		knee->addMesh(MP()->getMesh("feedL"));
	}
	else 
	{
		knee->addMesh(MP()->getMesh("feedR"));
	}
	
	sholderUD->addChild(knee);

}
void Leg::setRotationData(float _shoulder1Angle, float _shoulder2Angle, float _kneeAngle)
{
	shoulder1Angle = _shoulder1Angle;
	shoulder2Angle = _shoulder2Angle;
	if (_kneeAngle < -3.1415) _kneeAngle += 3.141592 * 2;
	if (_kneeAngle > 3.1415) _kneeAngle -= 3.141592 * 2;
	kneeAngle = _kneeAngle;

}
void  Leg::update() 
{
	
	sholderLR->setRotation(shoulder1Angle);
	sholderUD->setRotation(shoulder2Angle);
	knee->setRotation(kneeAngle);
	currentPos = knee->globalMatrix *vec4(0, 0, 0, 1);

}
void  Leg::draw(int type)
{
	
		gl::color(1, 0, 0);
		sholderLR->drawStats(type);
	
	
		gl::color(0, 1, 0);
		sholderUD->drawStats(type);
	
		gl::color(0, 0, 1);
		knee->drawStats(type);
	
	

}

