#pragma once
#include "Script.h"

enum class UI_TYPE
{
	ITEM_UI,
	SHOP_UI,
};

class StaticUIScript :
	public CScript
{
private:
	UI_TYPE					m_eType;
	bool					m_bUiCheck;

	vector<CGameObject*>	m_vecItemObj;

public:
	virtual void	update();

	void			ShowUI();

public:
	CLONE(StaticUIScript);

public:
	StaticUIScript(UI_TYPE _eType);
	virtual ~StaticUIScript();
};

