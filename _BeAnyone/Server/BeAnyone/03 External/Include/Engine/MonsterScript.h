#pragma once
#include "Script.h"

enum class MOB_TYPE
{
	YELLOW,
	GREEN,
	END,
};

class CMonsterScript :
	public CScript
{
private:
	MONSTER_TYPE m_eMonsterType;
	float m_fSpeed = 2.0f;

	// ui
	CGameObject* m_pUi;
	CGameObject* m_pUnderUi;
	CGameObject* m_pChildDummy;
	bool m_bSetChild = false;
	
	CTerrain* m_pTerrainObj;
	vector<Ptr<CMesh>>   m_pAniData;

	bool m_bisAttack = false;
	bool m_bisAniReset = false;
	bool m_bisDamaged = false;
	bool m_bisPunch = false;
	bool m_bisMoving = false;
	bool m_bisDirChange = false;
	// player save for quest
	CGameObject* m_pPlayer;

	sc_packet_monster_automove* m_Packet_autoMove = nullptr;

	// ¿ë¼®
	unsigned short m_sId;
	float m_fAnimationCnt[(UINT)MONSTER_ANICNT_TYPE::END] = {};
	uShort m_sHp;
	uShort m_sMaxHP = 100;
	MONSTER_AUTOMOVE_DIR m_eDir = MONSTER_AUTOMOVE_DIR::IDLE;

	// monster move dir
	float		m_fAngleY = 0.f;
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
	void CMonsterScript::SetAnimation(const uShort& other_id, const MONSTER_ANI_TYPE& type);
		void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
	void AnimClipReset();
	void SetAniReset(bool _aniReset) { m_bisAniReset = _aniReset; }

	// damage 
	void SetIsDamage(bool _isDamage) { m_bisDamaged = _isDamage; }
	bool GetIsDamage() { return m_bisDamaged; }
	void SetIsPunch(bool _isPunched) { m_bisPunch = _isPunched; }
	bool GetIsPunch() { return m_bisPunch; }
	
	// monster type
	void SetMonsterType(MONSTER_TYPE _eMonsterType) { m_eMonsterType = _eMonsterType; }
	MONSTER_TYPE GetMonsterType() { return m_eMonsterType; }

	CTerrain* GetTerrain() { return this->m_pTerrainObj; }

	void DecreaseHp();
	void Move();
	void Attack();
	void AttackToPlayer(MOB_TYPE _eType);

	// void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
	MONSTER_AUTOMOVE_DIR& GetDir() { return m_eDir; }
	void SetDir(const MONSTER_AUTOMOVE_DIR dir) { m_eDir = dir; }

	sc_packet_monster_automove* GetPacketMove() {return  m_Packet_autoMove; }
	void SetPacketMove(sc_packet_monster_automove* p);
	

	void SetisMoving(const bool moving) { m_bisMoving = moving; }
	bool GetisMoving() { return m_bisMoving; }

	void SetisDirChange(const bool moving) { m_bisDirChange = moving; }
	bool GetisDirChange() { return m_bisDirChange; }

public:
	void Attack_Default();

public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

