#include "MeshPool.h"
using namespace ci;
using namespace ci::app;
using namespace std;


MeshPool::MeshPool() { 

}
void MeshPool::setup(ModelConfig *_config)
{
	config = _config;
	mGlslShadow = gl::GlslProg::create(loadAsset("shadow_shader.vert"), loadAsset("shadow_shader.frag"));

}
void  MeshPool::clear() 
{

	pool.clear();
}
MeshRef MeshPool::getMesh(std::string name)
{
	for (auto mesh : pool) 
	{
		if (mesh->mName == name) return mesh;
	}

	MeshRef m = Mesh::create();
	
	if (name == "floor") { m->setup(name, geom::Plane().size(vec2(10000,10000)), vec3(0, 0, 0),Color(0.8,0.8,0.82), mGlslShadow); };
	if (name == "DMXaxis") { m->setup(name, geom::Cylinder().radius(10.25).height(38).direction(vec3(1,0,0)),vec3(-38 / 2, 0, 0), Color(0.831, 0.475, 0.416), mGlslShadow); };
	if (name == "2XL430") { m->setup(name, geom::Cube().size(36,	36.0f, 46.5f), vec3(0, 0, 46.5f/2-11.25),Color(0.149, 0.357, 0.416), mGlslShadow); };
	if (name == "XC430") { m->setup(name, geom::Cube().size(28.5, 34.0f, 46.5f), vec3(0, 0,config->upperLegSize + 11.25), Color(0.149, 0.357, 0.416), mGlslShadow); };
	if (name == "feedR") { m->setup(name, geom::Cone().base(10).apex(0).height(config->lowerLegSize).direction(vec3(0, 0, 1)),vec3(0, 0, 0), Color(0.494, 0.741, 0.569), mGlslShadow); };
	if (name == "feedL") { m->setup(name, geom::Cone().base(10).apex(0).height(config->lowerLegSize).direction(vec3(0, 0, -1)), vec3(0, 0, 0), Color(0.494, 0.741, 0.569), mGlslShadow); };
	
	pool.push_back(m);
	return m;
}


/*

*** Primary color :

shade 0 = #AA4839 = rgb(170, 72, 57) = rgba(170, 72, 57, 1) = rgb0(0.667, 0.282, 0.224)
shade 1 = #FFB6AA = rgb(255, 182, 170) = rgba(255, 182, 170, 1) = rgb0(1, 0.714, 0.667)
shade 2 = #D4796A = rgb(212, 121, 106) = rgba(212, 121, 106, 1) = rgb0(0.831, 0.475, 0.416)
shade 3 = #802415 = rgb(128, 36, 21) = rgba(128, 36, 21, 1) = rgb0(0.502, 0.141, 0.082)
shade 4 = #550C00 = rgb(85, 12, 0) = rgba(85, 12, 0, 1) = rgb0(0.333, 0.047, 0)

*** Secondary color(1) :

	shade 0 = #AA7239 = rgb(170, 114, 57) = rgba(170, 114, 57, 1) = rgb0(0.667, 0.447, 0.224)
	shade 1 = #FFD5AA = rgb(255, 213, 170) = rgba(255, 213, 170, 1) = rgb0(1, 0.835, 0.667)
	shade 2 = #D4A06A = rgb(212, 160, 106) = rgba(212, 160, 106, 1) = rgb0(0.831, 0.627, 0.416)
	shade 3 = #804B15 = rgb(128, 75, 21) = rgba(128, 75, 21, 1) = rgb0(0.502, 0.294, 0.082)
	shade 4 = #552B00 = rgb(85, 43, 0) = rgba(85, 43, 0, 1) = rgb0(0.333, 0.169, 0)

	*** Secondary color(2) :

	shade 0 = #265B6A = rgb(38, 91, 106) = rgba(38, 91, 106, 1) = rgb0(0.149, 0.357, 0.416)
	shade 1 = #6B939F = rgb(107, 147, 159) = rgba(107, 147, 159, 1) = rgb0(0.42, 0.576, 0.624)
	shade 2 = #447684 = rgb(68, 118, 132) = rgba(68, 118, 132, 1) = rgb0(0.267, 0.463, 0.518)
	shade 3 = #0F414F = rgb(15, 65, 79) = rgba(15, 65, 79, 1) = rgb0(0.059, 0.255, 0.31)
	shade 4 = #022A35 = rgb(2, 42, 53) = rgba(2, 42, 53, 1) = rgb0(0.008, 0.165, 0.208)

	*** Complement color :

shade 0 = #2A7E43 = rgb(42, 126, 67) = rgba(42, 126, 67, 1) = rgb0(0.165, 0.494, 0.263)
shade 1 = #7EBD91 = rgb(126, 189, 145) = rgba(126, 189, 145, 1) = rgb0(0.494, 0.741, 0.569)
shade 2 = #4F9D66 = rgb(79, 157, 102) = rgba(79, 157, 102, 1) = rgb0(0.31, 0.616, 0.4)
shade 3 = #105E27 = rgb(16, 94, 39) = rgba(16, 94, 39, 1) = rgb0(0.063, 0.369, 0.153)
shade 4 = #003F13 = rgb(0, 63, 19) = rgba(0, 63, 19, 1) = rgb0(0, 0.247, 0.075)

*/