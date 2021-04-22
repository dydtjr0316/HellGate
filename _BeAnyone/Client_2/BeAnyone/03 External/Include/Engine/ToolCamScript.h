#pragma once
#include "Script.h"

class CToolCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;
	CGameObject* m_pPlayer;

	//	�ӽ� ī�޶� ���� ����		//	false ����, true ĳ���� ����
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

