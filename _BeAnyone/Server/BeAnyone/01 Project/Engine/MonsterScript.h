#pragma once
#include "Script.h"

class CMonsterScript :
	public CScript
{
private:
	MONSTER_TYPE m_MType;

public:
	virtual void update();

	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

