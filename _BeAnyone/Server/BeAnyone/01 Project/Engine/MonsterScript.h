#pragma once
#include "Script.h"

class CMonsterScript :
	public CScript
{
private:
	MONSTER_TYPE m_MType;
	CGameObject* m_pUi;
	CGameObject* m_pUnderUi;

	unsigned short m_sId;

public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	void SetID(const unsigned short& id) { m_sId = id; }
	unsigned short GetID() { return m_sId; }

	// void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

