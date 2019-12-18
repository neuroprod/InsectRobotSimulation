#include "DebugRenderer.h"
#include "MeshPool.h"
#include "CinderImGui.h"
using namespace ci;
using namespace ci::app;
using namespace std;

void DebugRenderer::setup(NodeRef root, Model *model)
{
	mRoot = root;
	
	updateCamera();
	modelRenderer.model = model;

	

	
	gl::Texture2d::Format depthFormat;
	depthFormat.setInternalFormat(GL_DEPTH_COMPONENT32F);
	depthFormat.setCompareMode(GL_COMPARE_REF_TO_TEXTURE);
	depthFormat.setMagFilter(GL_LINEAR);
	depthFormat.setMinFilter(GL_LINEAR);
	depthFormat.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	depthFormat.setCompareFunc(GL_LEQUAL);
	mShadowMapTex = gl::Texture2d::create(fboSize, fboSize, depthFormat);

	gl::Fbo::Format fboFormat;
	fboFormat.attachment(GL_DEPTH_ATTACHMENT, mShadowMapTex);
	mFbo = gl::Fbo::create(fboSize, fboSize, fboFormat);



	mLightPos = vec3(500.0f, 1000.0f, 0.0f);
	mLightCam.setPerspective(60.0f, mFbo->getAspectRatio(), 10.f, 2000.0f);
	mLightCam.lookAt(mLightPos, vec3(0.0f));
	
	mGlsl = gl::GlslProg::create(loadAsset("shadow_shader.vert"), loadAsset("shadow_shader.frag"));
	floor = MP()->getMesh("floor");

	

	gl::enableDepthRead();
	gl::enableDepthWrite();

}
void  DebugRenderer::update(vec3 move,float rot) {

	if (showFloor) {
		mat4 t = mat4();
		
		t =glm::translate(t, move);
		t = glm::rotate(t, rot, vec3(0, 1, 0));
		floorMatrix =t*floorMatrix;
	
	}
	gl::enable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 2.0f);

	gl::ScopedFramebuffer fbo(mFbo);
	gl::ScopedViewport viewport(vec2(0.0f), mFbo->getSize());
	gl::clear(Color::black());
	gl::color(Color::white());
	gl::setMatrices(mLightCam);
	if(showMesh)mRoot->drawShadow();
	

	//floor->drawShadow();


	gl::disable(GL_POLYGON_OFFSET_FILL);
	

}


void DebugRenderer::draw()
{
	gl::setMatrices(mCam);
	//gl::drawCoordinateFrame();

	//mRoot->draw();

	gl::ScopedTextureBind texScope(mShadowMapTex, (uint8_t)0);
	vec3 mvLightPos = vec3(gl::getModelView() * vec4(mLightPos, 1.0f));
	mat4 shadowMatrix = mLightCam.getProjectionMatrix() * mLightCam.getViewMatrix();


	MP()->mGlslShadow->uniform("uShadowMap", 0);
	MP()->mGlslShadow->uniform("uLightPos", mLightPos);
	MP()->mGlslShadow->uniform("uShadowMatrix", shadowMatrix);
	MP()->mGlslShadow->uniform("alpha", 1.f);



	mRoot->draw(showMesh, showCoordinates);


	if (showHomePos)modelRenderer.drawHome();
	if(showMoveVec)modelRenderer.drawMove();
	if (showJoint1)modelRenderer.drawResolveJoint1(mRoot);
	if (showJoint23)modelRenderer.drawResolveJoint23(mRoot);

	if (showFloor) {
		MP()->mGlslShadowFloor->uniform("uShadowMap", 0);
		MP()->mGlslShadowFloor->uniform("uLightPos", mLightPos);
		MP()->mGlslShadowFloor->uniform("uShadowMatrix", shadowMatrix);
		MP()->mGlslShadowFloor->uniform("alpha", 1.f);
		gl::pushMatrices();
		gl::setModelMatrix(floorMatrix);
		
		floor->draw();
		gl::popMatrices();
	}
}
void DebugRenderer::updateCamera()
{
	float x = cameraDistance * sinf(cameraTheta)*cosf(cameraPhi);
	float z = cameraDistance * sinf(cameraTheta)*sinf(cameraPhi);
	float y = cameraDistance * cosf(cameraTheta);

	mCam.lookAt(vec3(x, y, z), vec3(0));
	mCam.setPerspective(30, getWindowAspectRatio(), 100, 4000);
}

void DebugRenderer::showRenderWindow()
{



		ui::ScopedWindow window("Renderer");
		if (ui::DragFloat("Camera Theta", &cameraTheta,0.01,0.001,3.1415/2)) updateCamera();
		if (ui::DragFloat("Camera Phi", &cameraPhi, 0.01, -3.1415, 3.1415)) updateCamera();
		if (ui::DragFloat("Camera distance", &cameraDistance, 1, 500, 2000)) updateCamera();
		ui::Checkbox("show Mesh", &showMesh);
		ui::Checkbox("show Floor", &showFloor);
		ui::Checkbox("show Coordinates", &showCoordinates);
		ui::Checkbox("show homePos", &showHomePos);
		ui::Checkbox("show moveVec", &showMoveVec);
		ui::Checkbox("show Resolve Joint1", &showJoint1);
		ui::Checkbox("show Resolve Joint23", &showJoint23);
}