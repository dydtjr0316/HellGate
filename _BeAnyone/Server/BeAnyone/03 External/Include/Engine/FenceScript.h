#pragma once
#include "Script.h"

enum class DOOR_STATE {
	IDLE,
	OPEN,
	END,
};

class CFenceScript
	: public CScript
{
private:
	CGameObject* m_pChildDummy;
	bool		m_bDoRotate = false;
	float		m_fDegree = 0.0f;
	DOOR_STATE  m_eDoorState;

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

