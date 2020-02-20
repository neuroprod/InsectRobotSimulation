#include "Matrix44.h"



Matrix44::Matrix44() 
{
	identity();

}
void Matrix44::set(float n11, float n12, float n13, float n14, float n21, float n22, float n23, float n24, float n31, float n32, float n33, float n34, float n41, float n42, float n43, float n44) 
{

	s[0] = n11; s[4] = n12; s[8] = n13; s[12] = n14;
	s[1] = n21; s[5] = n22; s[9] = n23; s[13] = n24;
	s[2] = n31; s[6] = n32; s[10] = n33; s[14] = n34;
	s[3] = n41; s[7] = n42; s[11] = n43; s[15] = n44;


}

void Matrix44::identity() 
{

	set(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);


}

//statics
Vector4 Matrix44::multiply(const Matrix44 &a, const Vector4 &b)
{

	float x = b.x, y = b.y, z = b.z, w = b.w;


	Vector4 res;
	res.x = a.s[0] * x + a.s[4] * y + a.s[8] * z + a.s[12] * w;
	res.y = a.s[1] * x + a.s[5] * y + a.s[9] * z + a.s[13] * w;
	res.z = a.s[2] * x + a.s[6] * y + a.s[10] * z + a.s[14] * w;
	res.w = a.s[3] * x + a.s[7] * y + a.s[11] * z + a.s[15] * w;

	return res;
}


Matrix44 Matrix44::multiply(const Matrix44 &a, const Matrix44 &b) 
{

	float  a11 = a.s[0], a12 = a.s[4], a13 = a.s[8], a14 = a.s[12];
	float a21 = a.s[1], a22 = a.s[5], a23 = a.s[9], a24 = a.s[13];
	float a31 = a.s[2], a32 = a.s[6], a33 = a.s[10], a34 = a.s[14];
	float a41 = a.s[3], a42 = a.s[7], a43 = a.s[11], a44 = a.s[15];

	float b11 = b.s[0], b12 = b.s[4], b13 = b.s[8], b14 = b.s[12];
	float b21 = b.s[1], b22 = b.s[5], b23 = b.s[9], b24 = b.s[13];
	float b31 = b.s[2], b32 = b.s[6], b33 = b.s[10], b34 = b.s[14];
	float b41 = b.s[3], b42 = b.s[7], b43 = b.s[11], b44 = b.s[15];


	Matrix44 res;

	res.s[0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
	res.s[4] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
	res.s[8] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
	res.s[12] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

	res.s[1] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
	res.s[5] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
	res.s[9] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
	res.s[13] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

	res.s[2] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
	res.s[6] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
	res.s[10] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
	res.s[14] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

	res.s[3] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
	res.s[7] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
	res.s[11] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
	res.s[15] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

	return res;
}

Matrix44 Matrix44::translate(const Matrix44 &mat, const  Vector3 &v) 
{
	Matrix44 res;
	res.s[12] = v.x;
	res.s[13] = v.y;
	res.s[14] = v.z;
	return Matrix44::multiply(mat,res);
}
Matrix44 Matrix44::rotate(const Matrix44 &mat,float rad, const  Vector3 &v)
{

	Matrix44 res;
	float c = cos(rad);
	float s = sin(rad);
	float t = 1 - c;
	float x =v.x, y = v.y, z = v.z;
	float tx = t * x, ty = t * y;

	res.set(

		tx * x + c, tx * y - s * z, tx * z + s * y, 0,
		tx * y + s * z, ty * y + c, ty * z - s * x, 0,
		tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
		0, 0, 0, 1

	);

	return Matrix44::multiply(mat, res);
}
Matrix44 Matrix44::inverse(const Matrix44 &mat)
{
	Matrix44 res;
		

	float n11 = mat.s[0], n21 = mat.s[1], n31 = mat.s[2], n41 = mat.s[3];
	float n12 = mat.s[4], n22 = mat.s[5], n32 = mat.s[6], n42 = mat.s[7];
	float n13 = mat.s[8], n23 = mat.s[9], n33 = mat.s[10], n43 = mat.s[11] ;
	float n14 = mat.s[12], n24 = mat.s[13], n34 = mat.s[14], n44 = mat.s[15];

	float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
	float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
	float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
	float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

	float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;

	if (det ==  0) {

		

		//fail
		return res;
	}

	float detInv = 1 / det;

	res.s[0] = t11 * detInv;
	res.s[1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * detInv;
	res.s[2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * detInv;
	res.s[3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * detInv;

	res.s[4] = t12 * detInv;
	res.s[5] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * detInv;
	res.s[6] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * detInv;
	res.s[7] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * detInv;

	res.s[8] = t13 * detInv;
	res.s[9] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * detInv;
	res.s[10] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * detInv;
	res.s[11] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * detInv;

	res.s[12] = t14 * detInv;
	res.s[13] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * detInv;
	res.s[14] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * detInv;
	res.s[15] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * detInv;

	return res;
}