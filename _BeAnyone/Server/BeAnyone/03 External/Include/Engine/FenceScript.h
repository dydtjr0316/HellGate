#pragma once
#include "Script.h"

class CFenceScript
	: public CScript
{
private:
	CGameObject* m_pChildDummy;
	bool		m_bDoRotate = false;
	float		m_fDegree = 0.0f;

public:
	virtual void update();
	void Init();

public:
	CLONE(CFenceScript);

	// collision
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	virtual void OnCollsion(CCollider* _pOther);

	void DoRotate();

public:
	CFenceScript();
	virtual ~CFenceScript();
};

