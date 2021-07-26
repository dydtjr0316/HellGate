#include "pch.h"
#include "StaticUIMgr.h"
#include "Button.h"

CStaticUIMgr::CStaticUIMgr()
	: m_bActive(false)
	, m_eType(UI_TYPE::PRIVATE_ITEM_UI)
{
	for (int i = 0; i < 16; ++i)
		m_vecButton.push_back(new CButton);
}

CStaticUIMgr::~CStaticUIMgr()
{
}
