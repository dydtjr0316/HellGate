#include "pch.h"
#include "Frustum.h"

#include "Camera.h"

CFrustum::CFrustum(CCamera* _pCam)
	: m_pCam(_pCam)
{
	init();
}

CFrustum::~CFrustum()
{
}

void CFrustum::init()
{
	// ������ǥ�� ���� �� 8�� ������

	// Near		Far
	// 		/	4 ----- 5
	// 	   /	|		|
	// 			|		|
	//0 -- 1	7 ----- 6
	//|    |   /
	//3 -- 2  /
	m_arrProj[0] = Vector3(-1.f, 1.f, 0.f);
	m_arrProj[1] = Vector3(1.f, 1.f, 0.f);
	m_arrProj[2] = Vector3(1.f, -1.f, 0.f);
	m_arrProj[3] = Vector3(-1.f, -1.f, 0.f);

	m_arrProj[4] = Vector3(-1.f, 1.f, 1.f);
	m_arrProj[5] = Vector3(1.f, 1.f, 1.f);
	m_arrProj[6] = Vector3(1.f, -1.f, 1.f);
	m_arrProj[7] = Vector3(-1.f, -1.f, 1.f);
}

void CFrustum::finalupdate()
{
	Matrix matViewInv = m_pCam->GetViewMatInv();
	Matrix matProjInv = m_pCam->GetProjMatInv();
	Matrix matInv = matProjInv * matViewInv;

	Vector3 vWorldPos[8] = {};

	// ���� �����̽� ���� ��ǥ�� ���Ѵ�
	for (int i = 0; i < 8; ++i)
	{
		vWorldPos[i] = XMVector3TransformCoord(m_arrProj[i], matInv);
	}

	// ���� �����̽� ��� 6���� ���Ѵ�.	
	m_arrFace[(UINT)FACE_TYPE::FT_NEAR] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[(UINT)FACE_TYPE::FT_FAR] = XMPlaneFromPoints(vWorldPos[5], vWorldPos[4], vWorldPos[7]);
	m_arrFace[(UINT)FACE_TYPE::FT_UP] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[5], vWorldPos[1]);
	m_arrFace[(UINT)FACE_TYPE::FT_DOWN] = XMPlaneFromPoints(vWorldPos[6], vWorldPos[7], vWorldPos[2]);
	m_arrFace[(UINT)FACE_TYPE::FT_LEFT] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[0], vWorldPos[7]);
	m_arrFace[(UINT)FACE_TYPE::FT_RIGHT] = XMPlaneFromPoints(vWorldPos[5], vWorldPos[6], vWorldPos[1]);
}

bool CFrustum::CheckFrustum(const Vector3& _vPos)
{
	for (UINT i = 0; i < (UINT)FACE_TYPE::FT_END; ++i)
	{
		Vector3 vNormal = m_arrFace[i];

		// vNormal == (a, b, c)
		// _vPos == (x, y, z)
		// m_arrFace[i].w == (d)
		// vNormal.Dot(_vPos) + m_arrFace[i].w = 0  ==>  ax + by + cz + d = 0
		if (vNormal.Dot(_vPos) + m_arrFace[i].w > 0)
			return false;
	}

	return true;
}

bool CFrustum::CheckFrustumSphere(const Vector3& _vPos, float _fRadius)
{
	for (UINT i = 0; i < (UINT)FACE_TYPE::FT_END; ++i)
	{
		Vector3 vNormal = m_arrFace[i];

		if (vNormal.Dot(_vPos) + m_arrFace[i].w > _fRadius)
			return false;
	}

	return true;
}