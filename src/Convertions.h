#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix44.h"
#include "glm/glm.hpp"

class Con 
{


public:
	Con() {};

	static glm::vec2 to(const Vector2 & in);
	static glm::vec3 to(const Vector3 & in);
	static glm::vec4 to(const Vector4 & in);
	static glm::mat4 to(const Matrix44 & in);



};