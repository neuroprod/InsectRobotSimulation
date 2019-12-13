#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
class Mesh;
typedef std::shared_ptr<Mesh> MeshRef;

class Mesh 
{
public:
	static MeshRef create();
	Mesh() {};
	void setup(std::string name, const ci::geom::Source &source,ci::vec3 pos, ci::Color color, ci::gl::GlslProgRef mGlslShadow);
	void draw();
	void drawShadow();
	ci::vec3 mPos;
	std::string mName;
	ci::Color mColor;
	ci::gl::BatchRef mainBatch;
	ci::gl::BatchRef shadowedBatch;


};