#pragma once
#include "Script.h"

class CSnowScript :
	public CScript
{
private:
	Vector3				m_vecVelocity;	
	Vector3				m_vecOrigin;
	Vector3				m_vecExtents;

	Ptr<CMesh>			m_pMesh;			// ��� �޽�(Point Mesh)
	Ptr<CMaterial>		m_pMtrl;			// ��ƼŬ ��������
	Ptr<CMaterial>		m_pUpdateMtrl;		// ��ǻƮ ���̴�


public:
	virtual void update();
	virtual void finalupdate();


	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CLONE(CSnowScript);

public:
	CSnowScript();
	virtual ~CSnowScript();
};


