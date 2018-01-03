#pragma once
class cVector3
{
public:
	float x, y, z;

public:
	cVector3();
	cVector3(float _x, float _y, float _z);
	~cVector3();

	/* 연산자 오버라이딩 구현
	+
	-
	* (스칼라 곱)
	==
	!=

	ex)
	cVector3 v1, v2 , v3;
	v3 = v1 + v2;
	if (v1 == v2) cout << "같다";
	*/

	cVector3 operator + (CONST cVector3& v) const;
	cVector3 operator - (CONST cVector3& v) const;
	cVector3 operator * (float s) const;

	bool cVector3::Equals(float ihs, float rhs) const;

	bool operator == (CONST cVector3& v) const;
	bool operator != (CONST cVector3& v) const;

	// 벡터의 길이(크기)
	float Length();
	// 벡터의 제곱 길이
	float LengthSq();
	// 정규화 함수(노멀라이즈)
	cVector3 Normalize();
	// 내적 (닷 프로덕트)
	static float Dot(cVector3& v1, cVector3& v2);
	// 외적 (크로스 프로덕트)
	static cVector3 Cross(cVector3& v1, cVector3& v2);

	// 용책 32 ~ 60 까지 읽기 (매트릭스)
};

