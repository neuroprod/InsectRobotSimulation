#pragma once
#include "Singleton.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Mesh.h"
#include "ModelConfig.h"
class MeshPool 
{

public:
	MeshPool();
	void setup(ModelConfig *_config);
	MeshRef getMesh(std::string name);
	void clear();
	ModelConfig *config;
	std::vector<MeshRef> pool;
	ci::gl::GlslProgRef	mGlslShadow;
};


typedef Singleton<MeshPool > MeshPoolSingleton;

inline MeshPool * MP() {
	return MeshPoolSingleton::Instance();
}
