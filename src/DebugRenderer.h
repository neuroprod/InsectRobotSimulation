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
	void update();


	void draw();
	void showRenderWindow();
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


	MeshRef	floor;
	
	float cameraDistance = 1500;
	float cameraTheta = 1.1;
	float cameraPhi = 1;
	bool drawHome = true;
	bool drawMove = true;
	bool showMesh = true;
	bool showFloor = false;
	bool showCoordinates= false;
	bool showJoint1 =false;
	bool showJoint23 = false;
	bool showHomePos = true;
	bool showMoveVec = true;
};