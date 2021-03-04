#pragma once

#include "define.h"

struct tResolution
{
	float fWidth;
	float fHeight;
};

// ���� ����ü
struct VTX
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vTangent;
	Vec3 vNormal;	
	Vec3 vBinormal;
};

struct tDBG
{
	DBG_TYPE eType;
	wstring  strDBG;
};


struct tLightColor
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
};

// ������� �޸�
struct tTransform
{	
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

//struct tTransform
//{
//	Matrix matWorld;
//	Matrix matView;
//	Matrix matProj;
//
//	Matrix matWV;
//	Matrix matWVP;
//};


struct tMtrlParam
{
	Vec4			m_vDiff; // ���� ���
	Vec4			m_vSpec; // �ݻ籤 ���
	Vec4			m_vEmv;  // �߱� ���

	int				m_arrInt[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float			m_arrFloat[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vec2			m_arrVec2[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vec4			m_arrVec4[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix			m_arrMat[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int				m_iArrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
};


struct tAnim2D
{
	Vec2 vLT;			// UV �»��
	Vec2 vLen;			// UV ����
	Vec2 vOffset;		// Offset Pos
	int  IsAnim2D;		// Animation2D ������Ʈ ��뿩��
	int  iPadding;
};

struct tLight2D
{
	Vec4 vLightPos;
	Vec4 vLightColor;
	Vec4 vLightDir;
	int   iLightType;
	float fRange;
	float fAngle;
	int   ipadding;
};

struct tLight2DInfo
{
	tLight2D	arrLight2D[100];
	UINT		iCount;
};

struct tLight3D
{
	tLightColor tColor;
	Vec4		vLightPos;
	Vec4		vLightDir;
	int			iLightType;
	float		fRange;
	float		fAngle;
	int			iPadding;
};

struct tLight3DInfo
{
	tLight3D arrLight3D[100];
	UINT	 iCurCount;
	Vec3     vPadding;
};