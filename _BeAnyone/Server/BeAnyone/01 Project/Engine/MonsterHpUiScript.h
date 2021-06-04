#pragma once
#include "Script.h"

class CMonsterHpUiScript :
	public CScript
{
private:
	bool			m_bUiCheck;

	CGameObject* m_pObject;
	CGameObject* m_pMonster;

public:
	virtual void update();
	void SetObject(CGameObject* _pObject) { m_pObject = _pObject; }
	void SetMonsterObject(CGameObject* _pMonster) { m_pMonster = _pMonster; }


public:
	CLONE(CMonsterHpUiScript);

public:
	CMonsterHpUiScript();
	virtual ~CMonsterHpUiScript();
};

