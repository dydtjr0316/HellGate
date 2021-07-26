#include "pch.h"
#include "Button.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"

void CButton::init()
{
	
}

void CButton::update()
{
	GetObj()->SetUiRenderCheck(m_bActive);

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

void CButton::finalupdate()
{
	GetObj()->StaticUIMgr();
	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{

		for (int i = 0; i < 16; ++i)
		{
			GetObj()->StaticUIMgr();
		}
		//CCollisionMgr::GetInst()->CollisionRect();
		auto mPos = CKeyMgr::GetInst()->GetMousePos();
	}
}

CButton::CButton()
	: CScript(0)
	, m_eItemId(ITEM_ID::EMPTY)
	, m_bActive(false)
{
	m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD_IMG");
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