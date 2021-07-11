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

	unsigned short m_sId;
	bool m_bisAttack = false;
	bool m_bisAniReset = false;
	bool m_bisDamaged = false;

	float m_fAnimationCnt[(UINT)MONSTER_ANICNT_TYPE::END] = {};

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
	void SetBisAttack(const bool& isattack) { m_bisAttack = isattack; }
	bool GetBisAttack() { return m_bisAttack; }

	void Setcnt(const float& cnt, MONSTER_ANICNT_TYPE _cntType) { m_fAnimationCnt[(UINT)_cntType] = cnt; }
	float Getcnt(MONSTER_ANICNT_TYPE _cntType) { return  m_fAnimationCnt[(UINT)_cntType]; }

	//animatioin
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
	void SetAnimation(const MONSTER_ANI_TYPE& type);
	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }

	void AnimClipReset() {
		if (m_bisAniReset == false) {
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			m_bisAniReset = true;
		}
	};

	CTerrain* GetTerrain() { return this->m_pTerrainObj; }

	void DecreaseHp();

	// void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

