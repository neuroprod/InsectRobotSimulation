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
Matrix44 Matrix44::translate(const Matrix44 &mat, const  Vector3 &v) 
{

	return Matrix44();
}
Matrix44 Matrix44::rotate(const Matrix44 &mat,float rad, const  Vector3 &v)
{

	return Matrix44();
}
Matrix44 Matrix44::inverse(const Matrix44 &mat)
{

	return Matrix44();
}