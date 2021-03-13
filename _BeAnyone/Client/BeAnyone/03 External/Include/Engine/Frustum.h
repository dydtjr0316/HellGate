#pragma once
#include "Entity.h"

class CCamera;

class CFrustum
	: public CEntity
{
private:
	CCamera*	m_pCam;
	Vector3		m_arrProj[8];
	Vector4		m_arrFace[(UINT)FACE_TYPE::FT_END];

public:
	void init();
	void finalupdate();

	bool CheckFrustum(const Vector3& _vPos);
	bool CheckFrustumSphere(const Vector3& _vPos, float _fRadius);

public:
	CFrustum(CCamera* _pCam);
	virtual	~CFrustum();
};

