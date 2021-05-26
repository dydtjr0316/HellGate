#pragma once
#include "Script.h"
#include "Terrain.h"
#include "CDeadReckoner.h"
//
#include "Mesh.h"

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
	CTerrain* m_pTerrainObj;
	XMFLOAT3 m_xmf3Velocity;
	float m_fSpeed;
	Ani_TYPE m_eAniType;
	CDeadReckoner* m_pDeadReckoner;
	sc_packet_move* m_movePacketTemp = nullptr;
	float m_fRTT;
	Vector2 m_v2Interpolation_Point[4];
	Vector2 m_v2Origin_Point[4];
	int m_iInterpolationCnt = 0;
public:
	XMFLOAT3 GetVelocity() { return m_xmf3Velocity; }
	void SetVelocity(XMFLOAT3 _fVelocity) { m_xmf3Velocity = _fVelocity; }
	void OnPlayerUpdateCallback();

public: //¿ë¼®
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
	void SetAnimation(const Ani_TYPE& type);
	void SetAnimation(const int& other_id, const Ani_TYPE& type);
	Ptr<CMesh> GetAniData(const Ani_TYPE& type) { return m_pAniData[(int)type]; }

	void initDeadReckoner() { m_pDeadReckoner = new CDeadReckoner(g_myid); }
	CDeadReckoner* GetReckoner() { return m_pDeadReckoner; }
	void op_Move();
	void SetOtherMovePacket(sc_packet_move* p, const float& rtt);
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
	CTerrain* GetTerrain() { return m_pTerrainObj; }
	bool isInMap(const Vector3& localPos);

};

