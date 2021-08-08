#include "pch.h"
#include "StaticUI.h"
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
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"HP_POTION_IMG");
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

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{

		for (int i = 0; i < 16; ++i)
		{
			GetObj()->StaticUI();
		}
	}
	else
	{
	}
}

void CButton::finalupdate()
{

}

void CButton::OnCollisionEnter(CCollider* _pOther)
{
	vector<CButton*> vecButton = m_pComParent->GetObj()->StaticUI()->GetButton();
	for (int i = 0; i < vecButton.size(); ++i)
	{
		if (vecButton[i]->GetActive() == BT_ACTIVE::ACTIVE)
			return;
	}
	m_bCheckActive = BT_ACTIVE::ACTIVE;

	m_vecOldPos = Transform()->GetLocalPos();

}

void CButton::OnCollision(CCollider* _pOther)
{
	if (m_bCheckActive == BT_ACTIVE::PASSIVE)
		return;

	m_bState = BT_STATE::CLICKED;

	Vector3 localPos = _pOther->Transform()->GetLocalPos();
	GetObj()->Transform()->SetLocalPos(localPos);

	/*if (_pOther->GetObj()->StaticUI()->GetType() == UI_TYPE::PUBLIC_SHOP_UI)
		m_bState = BT_STATE::SELL;

	else*/
		m_bState = BT_STATE::CLICKED;

}

void CButton::OnCollisionExit(CCollider* _pOther)
{
	switch (m_bState)
	{
	case BT_STATE::NONE:
		break;

	case BT_STATE::CLICKED:
		Transform()->SetLocalPos(m_vecOldPos);
		break;

	case BT_STATE::SELL:
		Transform()->SetLocalPos(m_vecOldPos);
		m_eItemId == ITEM_ID::EMPTY;
		break;
	}

	m_bState = BT_STATE::NONE;
	m_bCheckActive = BT_ACTIVE::PASSIVE;
}


CButton::CButton()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
	, m_eItemId(ITEM_ID::EMPTY)
	, m_bActive(false)
	, m_bState(BT_STATE::NONE)
	, m_bCheckActive(BT_ACTIVE::PASSIVE)
{
	//	test
	switch (UINT(rand() % 3))
	{
	case 0:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"HP_POTION_IMG");
		break;
	case 1:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD_IMG");
		break;
	case 2:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOW_IMG");
		break;
	}
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