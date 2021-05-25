#pragma once
#include "Script.h"

class CToolCamScript :
	public CScript
{
private:
	float			m_fSpeed;
	float			m_fScaleSpeed;
	CGameObject*	m_pPlayer;

	//	�ӽ� ī�޶� ���� ����		//	false ����, true ĳ���� ����
	bool bCameraTag = true;

public:
	virtual void update();
	void SetPlayer(CGameObject* _pPlayer) { 
		m_pPlayer = _pPlayer;
	}
	float GetSpeed() { return m_fSpeed; }
	void SetFreeCamera();
	void SetPlayerFixedCamera();


public:
	CLONE(CToolCamScript);

public:
	CToolCamScript();
	virtual ~CToolCamScript();
};

