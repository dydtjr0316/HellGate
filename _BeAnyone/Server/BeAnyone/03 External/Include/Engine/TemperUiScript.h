#pragma once
#include "Script.h"

class CTemperUiScript :
	public CScript
{
private:
	bool			m_bUiCheck;
	// float			m_fSpeed;
	// float			m_fScaleSpeed;
	CGameObject* m_pObject;
	CGameObject* m_pTempBar;
	CGameObject* m_pUnderTempBar;

public:
	virtual void update();
	void SetObject(CGameObject* _pObject) { m_pObject = _pObject; }
	void SetTempBar(CGameObject* _pTempBar) { m_pTempBar = _pTempBar; }
	void SetUnderTempBar(CGameObject* _pUnderTempBar) { m_pUnderTempBar = _pUnderTempBar; }

public:
	CLONE(CTemperUiScript);

public:
	CTemperUiScript();
	virtual ~CTemperUiScript();
};

