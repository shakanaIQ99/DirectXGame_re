#include "WorldTransform.h"
#include<cmath>

void WorldTransform::UpdateMatrix()
{
	matWorld_.IdentityMatrix4();
	matWorld_ *= ChengeScr();
	matWorld_ *= ChengeRot();
	matWorld_ *= ChengePos();
	if (parent_)
	{
		matWorld_ *= parent_->matWorld_;
	}

}

Matrix4 WorldTransform::ChengeRot()
{
	Matrix4 matRot;
	Matrix4 matRotX, matRotY, matRotZ;

	matRot.IdentityMatrix4();
	matRotX.IdentityMatrix4();
	matRotY.IdentityMatrix4();
	matRotZ.IdentityMatrix4();

	matRotX.m[1][1] =cos(rotation_.x);
	matRotX.m[1][2] =sin(rotation_.x);
	matRotX.m[2][1] =-sin(rotation_.x);
	matRotX.m[2][2] =cos(rotation_.x);

	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);

	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;
}

Matrix4 WorldTransform::ChengeScr()
{
	Matrix4 matscale;
	matscale.IdentityMatrix4();

	matscale.m[0][0] = scale_.x;
	matscale.m[1][1] = scale_.y;
	matscale.m[2][2] = scale_.z;

	return matscale;
}

Matrix4 WorldTransform::ChengePos()
{
	Matrix4 matTrans;
	matTrans.IdentityMatrix4();

	matTrans.m[3][0] += translation_.x;
	matTrans.m[3][1] += translation_.y;
	matTrans.m[3][2] += translation_.z;

	return matTrans;
}

