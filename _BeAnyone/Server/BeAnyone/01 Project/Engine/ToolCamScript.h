#pragma once
#include "Script.h"



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
	CGameObject*	m_pTest;
	float			m_bSetChild = false;
	float			m_dd = 0.0f;

	CGameObject* m_pRealTest;

	//	�ӽ� ī�޶� ���� ����		
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
	void SetCamState(CAMERA_STATE _eCamState) { m_eCamState = _eCamState; }
	CGameObject* GetTestObj() { return m_pTest; }	// camera parent ��������
	void SetIsChild(bool _bSetChild) { m_bSetChild = _bSetChild; }
	void ResetNpcCamAngle() { m_dd = 0.0f; }

public:
	CLONE(CToolCamScript);

public:
	CToolCamScript();
	virtual ~CToolCamScript();
};

