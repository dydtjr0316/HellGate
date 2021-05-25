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
public:
	void SetChangeSpeed() { m_fSpeed = m_fSpeed == PLAYER_SPEED_IDLE ? PLAYER_SPEED_DASH : PLAYER_SPEED_IDLE; }
	float GetSpeed() { return m_fSpeed; }


	XMFLOAT3 GetVelocity() { return m_xmf3Velocity; }
	void SetVelocity(XMFLOAT3 _fVelocity) { m_xmf3Velocity = _fVelocity; }

	
	void initDeadReckoner(){ m_pDeadReckoner = new CDeadReckoner(g_myid); }
	CDeadReckoner* GetReckoner() { return m_pDeadReckoner; }
	void op_Move();
	void SetOtherMovePacket(sc_packet_move* p) { m_movePacketTemp = new sc_packet_move; m_movePacketTemp = p; }
	void DeleteOherMovePaacket()
	{
		if (m_movePacketTemp != nullptr) {
	
			m_movePacketTemp = nullptr;
		}
	}
	sc_packet_move* GetOtherMovePacket() { return m_movePacketTemp; }

	void OnPlayerUpdateCallback();

	void SetAnimation(const Ani_TYPE& type);
	void SetAnimation(const int& other_id, const Ani_TYPE& type);
	Ptr<CMesh> GetAniData(const Ani_TYPE& type) { return m_pAniData[(int)type]; }

	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
	CTerrain* GetTerrain() { return m_pTerrainObj; }

	bool isInMap(const Vector3& localPos);

	// 애니메이션 전환
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
};

