#pragma once
#include "Script.h"

class CMonsterScript :
	public CScript
{
private:
	MONSTER_TYPE m_MType;
	float m_fSpeed = 2.0f;

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
	bool m_bisDamaged = false;
	bool m_bisPunch = false;

	sc_packet_monster_automove* m_Packet_autoMove = nullptr;

	// �뼮
	unsigned short m_sId;
	float m_fAnimationCnt[(UINT)MONSTER_ANICNT_TYPE::END] = {};
	uShort m_sHp;
	uShort m_sMaxHP = 100;
	MONSTER_AUTOMOVE_DIR m_eDir = MONSTER_AUTOMOVE_DIR::IDLE;
public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	float GetSpeed() { return m_fSpeed; }
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
	void AnimClipReset();

	CTerrain* GetTerrain() { return this->m_pTerrainObj; }

	void DecreaseHp();

	// void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
	MONSTER_AUTOMOVE_DIR& GetDir() { return m_eDir; }
	void SetDir(const MONSTER_AUTOMOVE_DIR& dir) { m_eDir = dir; }

	sc_packet_monster_automove* GetPacketMove() { m_Packet_autoMove; }
	void SetPacketMove(sc_packet_monster_automove* p) { m_Packet_autoMove = p; }
	
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

