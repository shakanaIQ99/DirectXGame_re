#include "Matrix4.h"



void Matrix4::IdentityMatrix4()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				m[i][j] = 1;
			}
			else
			{
				m[i][j] = 0;
			}
		}
	}


}

Matrix4& Matrix4::operator*(float s)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] *= s;

		}

	}

	return *this;
}





//Matrix4& Matrix4::operator*=(const Matrix4& m2)
//{
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			m[i][j] *= m2.m[i][j];
//		}
//	}
//
//	return *this;
//}

//const Matrix4 operator*(const Matrix4& v, const Matrix4& v2)
//{
//	Matrix4 temp(v);
//	Matrix4 temp2(v2);
//
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			temp.m[i][j] *= temp2.m[i][j];
//
//		}
//
//	}
//
//	return temp;
//}
