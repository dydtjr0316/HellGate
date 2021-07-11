#pragma once
#include "Script.h"

class CMonsterScript :
	public CScript
{
private:
	MONSTER_TYPE m_MType;

	// ui
	CGameObject* m_pUi;
	CGameObject* m_pUnderUi;
	CGameObject* m_pChildDummy;
	bool m_bSetChild = false;
	
	CTerrain* m_pTerrainObj;

	// animation
	vector<Ptr<CMesh>>   m_pAniData;

	bool m_bisAttack = false;
	bool m_bisAniReset = false;


	// ¿ë¼®
	unsigned short m_sId;
	float m_fAnimationCnt = 0.f;
	uShort m_sHp;
	uShort m_sMaxHP = 100;
	MONSTER_AUTOMOVE_DIR m_eDir = MONSTER_AUTOMOVE_DIR::IDLE;
public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	void SetID(const uShort& id) { m_sId = id; }
	uShort GetID() { return m_sId; }
	void SetHP(const uShort& hp) { m_sHp = hp; }
	uShort GetHP() { return m_sHp; }
	void SetBisAttack(const bool& isattack) { m_bisAttack = isattack; }
	bool GetBisAttack() { return m_bisAttack; }

	void Setcnt(const float& cnt) { m_fAnimationCnt = cnt; }
	float Getcnt() { return m_fAnimationCnt; }

	//animatioin
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
	void SetAnimation(const MONSTER_ANI_TYPE& type);
	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }

	void AnimClipReset();

	CTerrain* GetTerrain() { return this->m_pTerrainObj; }

	void DecreaseHp();

	// void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
	MONSTER_AUTOMOVE_DIR& GetDir() { return m_eDir; }
	void SetDir(const MONSTER_AUTOMOVE_DIR& dir) { m_eDir = dir; }
	
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

