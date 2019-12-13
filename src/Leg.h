#pragma once

#include "cinder/app/App.h"
#include "Node.h"

class Leg;
typedef std::shared_ptr<Leg> LegRef;

class Leg 
{
public:
	static LegRef create();
	Leg() {};
	void setup(std::string name, NodeRef root, ci::vec3 startPos, float startRot, bool flip );
	
	std::string mName;

	NodeRef sholderLR;
	NodeRef sholderUD;
	NodeRef knee;

	void update();
	void draw();

	ci::vec4 currentPos;

};