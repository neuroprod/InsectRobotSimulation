#pragma once
#include "cinder/app/App.h"
#include <memory>
#include "Mesh.h"

class Node;
typedef std::shared_ptr<Node> NodeRef;

class Node : public std::enable_shared_from_this<Node>
{
public:
	static NodeRef create();
	Node() {};
	void removeAllChildren();

	void addChild(NodeRef ref);
	void setBase(glm::vec3 position);
	void setBase(glm::vec3 position, glm::vec3 rotation);
	void setRotation(float r);
	void addMesh(MeshRef m);
	void update();

	void draw();
	void drawShadow();
	
	NodeRef parent =nullptr;
	std::vector<NodeRef> children;

	float currentRotation = 0;
	ci::mat4 baseMatrix;
	ci::mat4 localMatrix;
	ci::mat4 globalMatrix;

	std::vector<MeshRef> meshes;

};