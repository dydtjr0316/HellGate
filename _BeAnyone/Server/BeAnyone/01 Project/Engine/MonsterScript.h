#pragma once
#include "Script.h"

class CMonsterScript :
	public CScript
{
private:
	MONSTER_TYPE m_MType;
	CGameObject* m_pUi;
	CGameObject* m_pUnderUi;
	CTerrain* m_pTerrainObj;

	unsigned short m_sId;

	uShort m_sHp;
	uShort m_sMaxHP = 100;
public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	void SetID(const uShort& id) { m_sId = id; }
	uShort GetID() { return m_sId; }
	void SetHP(const uShort& hp) { m_sHp = hp; }
	uShort GetHP() { return m_sHp; }

	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
	CTerrain* GetTerrain() { return this->m_pTerrainObj; }

	void DecreaseHp();

	// void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

