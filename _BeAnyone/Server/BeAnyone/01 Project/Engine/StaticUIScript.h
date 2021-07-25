#pragma once
#include "Script.h"

class StaticUIScript :
	public CScript
{
private:
	bool			m_bUiCheck;

	CGameObject*	m_pObject;
	CGameObject*	m_pTempBar;
	CGameObject*	m_pUnderTempBar;

public:
	virtual void	update();

	void			ShowUI();

public:
	CLONE(StaticUIScript);

public:
	StaticUIScript();
	virtual ~StaticUIScript();
};

