#pragma once
#include "Script.h"

class CToolCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;
	CGameObject* m_pPlayer;

	//	임시 카메라 기준 변수		//	false 기존, true 캐릭터 고정
	bool bCameraTag = false;

public:
	virtual void update();

	void SetPlayer(CGameObject* _pPlyaer) { m_pPlayer = _pPlyaer; }
	void SetFreeCamera();
	void SetPlayerFixedCamera();

public:
	CLONE( CToolCamScript );

public:
	CToolCamScript();
	virtual ~CToolCamScript();
};

