#pragma once
#include "Script.h"

class CToolCamScript :
	public CScript
{
private:
	float			m_fSpeed;
	float			m_fScaleSpeed;
	CGameObject*	m_pPlayer;

	//	임시 카메라 기준 변수		//	false 기존, true 캐릭터 고정
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

