#pragma
#include "Node.h"
#include "cinder/gl/gl.h"
using namespace ci;
using namespace ci::app;
using namespace std;

NodeRef Node::create()
{
	NodeRef ref = std::make_shared<Node>();
	return ref;
}

void Node::addChild(NodeRef ref) 
{
	ref->parent = shared_from_this();
	children.push_back(ref);

}
void  Node::removeAllChildren() 
{
	for (auto child : children)
	{
		child->removeAllChildren();
	}
	children.clear();
}

void Node::setBase(vec3 position)
{
	baseMatrix = glm::mat4();
	baseMatrix = glm::translate(baseMatrix, position);



}
void Node::setBase(vec3 position, vec3 rotation)
{
	baseMatrix =  glm::mat4();
	baseMatrix =glm::translate(baseMatrix, position);
	
	
	baseMatrix =glm::rotate(baseMatrix, rotation.x, vec3(1, 0, 0));
	baseMatrix = glm::rotate(baseMatrix, rotation.y, vec3(0, 1, 0));
	baseMatrix = glm::rotate(baseMatrix, rotation.z, vec3(0, 0, 1));

}
void Node::setRotation(float r) 
{
	currentRotation = r;
}
void Node::update() 
{
	
	localMatrix = baseMatrix;
	localMatrix = glm::rotate(localMatrix, currentRotation, vec3(1, 0, 0));
	if (parent) 
	{
		globalMatrix = parent->globalMatrix *localMatrix;
	}
	else 
	{
		globalMatrix = localMatrix;
	}
	
	for (auto child : children) 
	{
		child->update();
	}

}
void Node::drawShadow()
{
	gl::pushMatrices();

	gl::setModelMatrix(globalMatrix);
	for (auto m : meshes)
	{
		m->drawShadow();
	}

	//gl::drawCoordinateFrame();
	gl::popMatrices();


	for (auto child : children)
	{
		child->drawShadow();
	}
}
void Node::draw() 
{
	gl::pushMatrices();
	
	gl::setModelMatrix(globalMatrix);
	
	for (auto m : meshes) 
	{
		m->draw();
	}
	
	gl::drawCoordinateFrame(35,10,2);
	gl::popMatrices();


	for (auto child : children)
	{
		child->draw();
	}
}

void Node::addMesh(MeshRef m) 
{
	meshes.push_back(m);
}