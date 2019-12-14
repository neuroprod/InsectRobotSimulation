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
	root->addChild(sholderLR);


	float startAngle = -3.1415 / 2;
	if (flip) startAngle *= -1;

	
	sholderUD = Node::create();
	sholderUD->setBase(vec3(0, 0, 24), vec3(0, 0, startAngle));
	sholderUD->setRotation(0);
	sholderUD->addMesh(MP()->getMesh("DMXaxis"));

	sholderLR->addMesh(MP()->getMesh("XC430"));
	sholderLR->addChild(sholderUD);
	
	knee = Node::create();
	knee->setBase(vec3(0, 0, config->upperLegSize), vec3(3.1415 / 2, 0, 0));
	knee->setRotation(0);
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

void  Leg::update() 
{
	currentPos = knee->globalMatrix *vec4(0, 0, 0, 1);

}
void  Leg::draw()
{

	

}

