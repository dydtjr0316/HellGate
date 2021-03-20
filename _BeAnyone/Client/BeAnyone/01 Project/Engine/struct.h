#pragma once

struct tResolution
{
	float fWidth;
	float fHeight;
};

// 정점 구조체
struct VTX
{
	Vector3 vPos;
	Vector4 vColor;
	Vector2 vUV;

	Vector3 vTangent;
	Vector3 vNormal;
	Vector3 vBinormal;
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;

	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;
};

extern tTransform g_transform;

struct tMtrlParam
{
	Vector4			m_vDiff; // 색상 계수
	Vector4			m_vSpec; // 반사광 계수
	Vector4			m_vEmv;  // 발광 계수

	int				m_arrInt[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float			m_arrFloat[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vector2			m_arrVec2[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vector4			m_arrVec4[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix			m_arrMat[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int				m_iArrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
};

struct tLightColor
{
	Vector4 vDiff;
	Vector4 vSpec;
	Vector4 vAmb;
};

struct tLight
{
	tLightColor tColor;
	Vector4		vLightPos;
	Vector4		vLightDir;
	int			iLightType;
	float		fRange;
	float		fAngle;
	int			iPadding;
};

struct tLightInfo
{
	tLight arrLight[100];
	UINT	 iCurCount;
	Vector3     vPadding;
};