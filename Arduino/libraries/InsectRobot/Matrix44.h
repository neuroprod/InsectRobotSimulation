#pragma once
#include "Vector3.h"
#include "Vector4.h"

class Matrix44 
{
public:
	Matrix44();

	float s[16];

	void set(float n11, float n12, float n13, float n14, float n21, float n22, float n23, float n24, float n31, float n32, float n33, float n34, float n41, float n42, float n43, float n44);
	void identity();

	static Vector4 multiply(const Matrix44 &a, const Vector4 &b);
	static Matrix44 multiply(const Matrix44 &a, const Matrix44 &b);
	static Matrix44 translate(const Matrix44 &mat, const  Vector3 &v);
	static Matrix44 rotate(const Matrix44 &mat,float rad, const  Vector3 &v);
	static Matrix44 inverse(const Matrix44 &mat);


	Matrix44  operator * (Matrix44  a) {
		Matrix44  res = Matrix44::multiply(*this, a);
		return res;
	}
	Vector4  operator * (Vector4  a) {
		Vector4  res = Matrix44::multiply(*this, a);
		return res;
	}
};