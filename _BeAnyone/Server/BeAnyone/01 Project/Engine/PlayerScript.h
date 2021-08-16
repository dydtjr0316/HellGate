#pragma once
#include "Script.h"
#include "Terrain.h"
#include "CDeadReckoner.h"
#include "Mesh.h"

enum class UI_BAR {
	STAMINA,
	DASH,
	HUG,
	TEMPER,
	END,
};

enum class UI_BUTTON {
	HUG,
	TEMPER,
	END,
};


class CDeadReckoner;
class CPlayerScript
	: public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();


private:
	Vector3				m_vecPlayerDir;
	CGameObject*		m_pItemUIObj;
	Drag_Type			m_eDrag;
	float				m_fBeforeDragX = 0.f;

private:
	float			m_ftimeCount = 0.f;
	float			m_fDelayTime = 0.5f;
	bool			FirstPacket = false;
	bool			m_bisAttack = false;
	bool			m_bisAniReset = false;
	bool			m_bisDamage = false;
	float			m_fAnimationCnt[(UINT)PlAYER_ANICNT_TYPE::END]{};

	CTerrain*		m_pTerrainObj;
	XMFLOAT3		m_xmf3Velocity;
	float			m_fSpeed;
	Ani_TYPE		m_eAniType;
	CDeadReckoner*	m_pDeadReckoner;
	sc_packet_move* m_movePacketTemp = nullptr;
	float			m_fRTT;
	Vector2			m_v2Interpolation_Point[4];
	Vector2			m_v2Origin_Point[4];
	int				m_iInterpolationCnt = 0;

	// pickup
	bool			m_bIsPick = false;

	// ui
	vector<CGameObject*>		m_vUnderBar;
	vector<CGameObject*>		m_vUiBar;
	vector<CGameObject*>		m_vUiButton;
	bool						m_bUiCheck = true;

	float						m_fTime = 0.0f;

public:
	bool GetAniReset() { return m_bisAniReset; }
	void SetAniReset(const bool& reset) { m_bisAniReset = reset; }
	void SetAttack(bool isattack) { m_bisAttack = isattack; }
	bool GetAttack() { return this->m_bisAttack; }

	void SetDamage(bool _isDamage) { m_bisDamage = _isDamage; }
	bool GetDamage() { return m_bisDamage; }

	// animclip
	void SetCnt(const float& cnt, PlAYER_ANICNT_TYPE _AniCntType) { m_fAnimationCnt[(UINT)_AniCntType] = cnt; }
	float GetCnt(PlAYER_ANICNT_TYPE _AniCntType) { return m_fAnimationCnt[(UINT)_AniCntType]; }
	
	
public:
	XMFLOAT3 GetVelocity() { return m_xmf3Velocity; }
	void SetVelocity(XMFLOAT3 _fVelocity) { m_xmf3Velocity = _fVelocity; }
	void OnPlayerUpdateCallback();
	bool GetBisFrist() { return FirstPacket; }
	void SetBisFrist(const bool& bis) { FirstPacket = bis; }
	void CountTime() { m_ftimeCount += DT; }
	void SetTime_Zero() { m_ftimeCount = 0.f; }

	Vector3 GetPlayerDir() { return m_vecPlayerDir; }
	void SetPlayerDir(Vector3 _a) { m_vecPlayerDir = _a; }
	void Attack_Default();

	// pick up item
	CGameObject* GetUIObj() { return m_pItemUIObj; }
	void SetUIObj(CGameObject* _obj) { m_pItemUIObj = _obj; }
	void PickUp_Default();

	// ui
	void ClickUiButton();
	void FindQuestItem();
	void ReduceUiBar();

	// use item
	void UseItem();
	void FindItemBeUsed(int _itemId);
public:
	// sound
	void PlaySound_(const Sound_Type& sound);
public: 
	//¿ë¼®
	//animation
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
	void SetAnimation(const Ani_TYPE& type);
	void SetAnimation(const uShort& other_id, const Ani_TYPE& type);
	Ptr<CMesh> GetAniData(const Ani_TYPE& type) { return m_pAniData[(int)type]; }
	void SetAnimationType(const Ani_TYPE& type) { m_eAniType = type; }
	void AnimClipReset() {
		if (m_bisAniReset == false) {
			GetObj()->Animator3D()->SetClipTime(0, 0.f);
			m_bisAniReset = true;
		}
	};

	void initDeadReckoner() { m_pDeadReckoner = new CDeadReckoner(g_myid); }
	CDeadReckoner* GetReckoner() { return m_pDeadReckoner; }
	void op_Move();
	void SetOtherMovePacket(sc_packet_move* p, const float& rtt);
	void SetOtherMovePacket__IsMoving(const bool& isMoving) { 
		if(m_movePacketTemp!=nullptr)m_movePacketTemp->isMoving = isMoving;
	}
	void DeleteOherMovePaacket() { if (m_movePacketTemp != nullptr) { m_movePacketTemp = nullptr; } }
	sc_packet_move* GetOtherMovePacket() { return m_movePacketTemp; }

	float GetRTT() { return m_fRTT; }
	void SetRTT(const float& rtt) { m_fRTT = rtt; }
	Vector2* GetOriginPoint() { return m_v2Origin_Point; }
	Vector2* GetInterpolationPoint() { return m_v2Interpolation_Point; }

	int GetInterpolationCnt() { return m_iInterpolationCnt; }
	void InterpolationCnt_PP() { m_iInterpolationCnt++; }
	void Set_InterpolationCnt_Zero() { m_iInterpolationCnt = 0; }

	void SetOrigin_Point(const int& index, const float& x, const float& y);
	void SetInterpolation_Point(const int& index, const float& x, const float& y);

	void Search_Origin_Points(const int& id, const float& rtt);
	Vector2 Search_Interpolation_Points(Vector2* points, float time);
	void Compute_Bezier(Vector2* points, Vector2* dest);


	void SetChangeSpeed() { m_fSpeed = m_fSpeed == PLAYER_SPEED_IDLE ? PLAYER_SPEED_DASH : PLAYER_SPEED_IDLE; }
	float GetSpeed() { return m_fSpeed; }

	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
	CTerrain* GetTerrain() { return this->m_pTerrainObj; }

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	bool isInMap(const Vector3& localPos);
};

