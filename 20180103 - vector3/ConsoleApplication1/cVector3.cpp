#include "stdafx.h"
#include "cVector3.h"


cVector3::cVector3()
{
}

cVector3::cVector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}


cVector3::~cVector3()
{
}

// ������ �������̵�
cVector3 cVector3::operator+(CONST cVector3 & v) const
{
	cVector3 resultV;
	
	resultV.x = x + v.x;
	resultV.y = y + v.y;
	resultV.z = z + v.z;
	
	return resultV;
}

cVector3 cVector3::operator-(CONST cVector3& v) const
{
	cVector3 resultV;

	resultV.x = x - v.x;
	resultV.y = y - v.y;
	resultV.z = z - v.z;

	return resultV;
}

cVector3 cVector3::operator*(float s) const
{
	cVector3 resultV;

	resultV.x = x * s;
	resultV.y = y * s;
	resultV.z = z * s;

	return resultV;
}
bool cVector3::operator==(CONST cVector3& v) const
{
	if (x == v.x
		&& y == v.y
		&& z == v.z)
		return true;

	if (fabs(x - v.x) < FLT_EPSILON
		&& fabs(y - v.y) < FLT_EPSILON
		&& fabs(z - v.z) < FLT_EPSILON)
		return true;

	return false;
}

bool cVector3::operator!=(CONST cVector3& v) const
{
	if (fabs(x - v.x) < FLT_EPSILON
		&& fabs(y - v.y) < FLT_EPSILON
		&& fabs(z - v.z) < FLT_EPSILON)
		return false;

	return true;
}

// ������ ����(ũ��)
float cVector3::Length()
{
	return sqrt(LengthSq()); // ��Ʈ�Լ�
}

// ������ ���� ����
float cVector3::LengthSq()
{
	return pow(x, 2) + pow(y, 2) + pow(z, 2); // �����Լ�(��, �¼�)
}

// ����ȭ �Լ�(��ֶ�����) -��������
cVector3 cVector3::Normalize()
{
	cVector3 NormalV;

	NormalV.x = x / Length();
	NormalV.y = y / Length();
	NormalV.z = z / Length();

	return NormalV;
}

// ���� (�� ���δ�Ʈ)
float cVector3::Dot(cVector3 & v1, cVector3 & v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// ���� (ũ�ν� ���δ�Ʈ)
cVector3 cVector3::Cross(cVector3 & v1, cVector3 & v2)
{
	cVector3 CrossV;

	CrossV.x = (v1.y * v2.z) - (v1.z * v2.y);
	CrossV.y = (v1.z * v2.x) - (v1.x * v2.z);
	CrossV.z = (v1.x * v2.y) - (v1.y * v2.x);

	return CrossV;
}
