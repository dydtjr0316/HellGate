#include "pch.h"
#include "StaticUIScript.h"

StaticUIScript::StaticUIScript(UI_TYPE _eType)
	: CScript(0)
	, m_eType(_eType)
	, m_bUiCheck(false)
{
}

StaticUIScript::~StaticUIScript()
{
}


void StaticUIScript::update()
{
	if (KEY_TAB(KEY_TYPE::KEY_Q))
	{
		m_bUiCheck != m_bUiCheck;

		if (m_bUiCheck)
			ShowUI();

	}
}

void StaticUIScript::ShowUI()
{
	switch (m_eType)
	{

	case UI_TYPE::ITEM_UI:
		break;
	case UI_TYPE::SHOP_UI:
		break;
	}
}

