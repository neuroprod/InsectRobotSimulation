#pragma once
#include "Singleton.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Mesh.h"
class MeshPool 
{

public:
	MeshPool();
	void setup();
	MeshRef getMesh(std::string name);

	std::vector<MeshRef> pool;
	ci::gl::GlslProgRef	mGlslShadow;
};


typedef Singleton<MeshPool > MeshPoolSingleton;

inline MeshPool * MP() {
	return MeshPoolSingleton::Instance();
}
