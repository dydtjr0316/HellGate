#include "pch.h"
#include "Button.h"
#include "ResMgr.h"

void CButton::init()
{
	
}

void CButton::update()
{
	switch (m_eItemId)
	{
	case ITEM_ID::EMPTY:
		break;

	case ITEM_ID::BASIC_SWORD:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD_IMG");
		break;

	case ITEM_ID::BASIC_ARROW:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOW_IMG");
		break;

	case ITEM_ID::END:
		return;
	}
}

CButton::CButton()
	: CComponent(COMPONENT_TYPE::COLLIDER)
	, m_eItemId(ITEM_ID::EMPTY)
{
}


CButton::~CButton()
{
}

void CButton::SaveToScene(FILE* _pFile)
{
}

void CButton::LoadFromScene(FILE* _pFile)
{
}