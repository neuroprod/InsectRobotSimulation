#include "Convertions.h"

glm::vec2  Con::to(const Vector2 & in)
{
	return glm::vec2(in.x, in.y);

}

glm::vec3  Con::to(const Vector3 & in)
{
	return glm::vec3(in.x, in.y, in.z);

}

glm::vec4  Con::to(const Vector4 & in)
{
	return glm::vec4(in.x, in.y, in.z, in.w);

}

glm::mat4 Con::to(const Matrix44 & in)
{
	glm::mat4 m;
	m[0][0] = in.s[0];
	m[0][1] = in.s[1];
	m[0][2] = in.s[2];
	m[0][3] = in.s[3];

	m[1][0] = in.s[4];
	m[1][1] = in.s[5];
	m[1][2] = in.s[6];
	m[1][3] = in.s[7];

	m[2][0] = in.s[8];
	m[2][1] = in.s[9];
	m[2][2] = in.s[10];
	m[2][3] = in.s[11];

	m[3][0] = in.s[12];
	m[3][1] = in.s[13];
	m[3][2] = in.s[14];
	m[3][3] = in.s[15];




	return m;

}