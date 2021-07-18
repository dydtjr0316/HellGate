#pragma once
#include "Script.h"

enum class CAMERA_STATE {
	FREE_CAMERA,
	FIXED_CAMERA,
	NPC_CAMERA,
	END,
};

class CToolCamScript :
	public CScript
{
private:
	float			m_fSpeed;
	float			m_fScaleSpeed;
	CGameObject*	m_pPlayer;

	// npc
	Vector3			m_NpcPos; // npc pos;
	CGameObject*	m_pDummy;

	//	임시 카메라 기준 변수		
	CAMERA_STATE	m_eCamState = CAMERA_STATE::FIXED_CAMERA;

public:
	virtual void update();
	void SetPlayer(CGameObject* _pPlayer) { 
		m_pPlayer = _pPlayer;
	}
	float GetSpeed() { return m_fSpeed; }
	void SetFreeCamera();
	void SetPlayerFixedCamera();
	void SetNpcCamera();

	// npc
	void SetNpcPos(const Vector3& _vPos) { m_NpcPos = _vPos; }
	void SetCamState(int _eCamState) { m_eCamState = (CAMERA_STATE)_eCamState; }

public:
	CLONE(CToolCamScript);

public:
	CToolCamScript();
	virtual ~CToolCamScript();
};

