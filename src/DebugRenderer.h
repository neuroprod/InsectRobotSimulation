#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Node.h"
#include "Mesh.h"
#include "Model.h"
#include "ModelRenderer.h"

class DebugRenderer 
{
public:
	DebugRenderer() {}
	void setup(NodeRef root,Model * model);
	void update(glm::vec3 move,float rot);


	void draw();
	void showRenderWindow(float fps);
	void updateCamera();
	ci::CameraPersp	mCam;
	NodeRef mRoot;

	ModelRenderer modelRenderer;
	ci::gl::FboRef			mFbo;

	ci::CameraPersp			mLightCam;
	ci::vec3				mLightPos;
	ci::gl::GlslProgRef		mGlsl;
	ci::gl::Texture2dRef	mShadowMapTex;
	int	fboSize = 2048;

	glm::mat4 floorMatrix;
	MeshRef	floor;
	
	float cameraDistance = 1500;
	float cameraTheta = 1.1;
	float cameraPhi = 1;
	bool drawHome = true;
	bool drawMove = true;
	bool showMesh = true;
	bool showFloor = true;
	bool showCoordinates= false;
	bool showJoint1 =false;
	bool showJoint23 = false;
	bool showHomePos = false;
	bool showMoveVec = false;
};