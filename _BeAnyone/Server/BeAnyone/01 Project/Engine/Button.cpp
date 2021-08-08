#include "pch.h"
#include "StaticUI.h"
#include "Button.h"
#include "ResMgr.h"
#include "RenderMgr.h"
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
	auto temp = m_pComParent->GetObj()->StaticUI()->GetButton();
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pItemImage.GetPointer());


	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{

		for (int i = 0; i < 16; ++i)
		{
			GetObj()->StaticUI();
		}
	}

	for (int i = 0; i < m_vItemNum.size(); ++i) {
		Vector3 vButtonPos = GetObj()->Transform()->GetLocalPos();
		Vector3 vButtonScale = GetObj()->Transform()->GetLocalScale();
		Vector3 vNumPos = m_vItemNum[i]->Transform()->GetLocalPos();
		Vector3 vNumScale = m_vItemNum[i]->Transform()->GetLocalPos();

		vNumPos = vButtonPos;
		vNumPos += Vector3((vButtonScale.x / 2 - vNumScale.x / 2) - (i * vNumScale.x), -(vButtonScale.y / 2 - vNumScale.y / 2), 0.f);

		m_vItemNum[i]->Transform()->SetLocalPos(vNumPos);
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
		m_eItemId = ITEM_ID::EMPTY;
		break;
	case 1:
		m_eItemId = ITEM_ID::BASIC_SWORD;
		break;
	case 2:
		m_eItemId = ITEM_ID::BASIC_ARROW;
		break;
	case 3:
		m_eItemId = ITEM_ID::BRANCH;
		break;
	case 4:
		m_eItemId = ITEM_ID::APPLE;
		break;
	case 5:
		m_eItemId = ITEM_ID::BOTTLE_STAMINA;
		break;
	case 6:
		m_eItemId = ITEM_ID::BOTTLE_DASH;
		break;
	}

	tResolution res = CRenderMgr::GetInst()->GetResolution();
	
	// item number create
	for (int i = 0; i < (UINT)ITEM_NUM::END; ++i) {
		CGameObject* pItemNum = new CGameObject;
		pItemNum->SetName(L"ItemNum");
		pItemNum->FrustumCheck(false);
		pItemNum->AddComponent(new CTransform);
		pItemNum->AddComponent(new CMeshRender);
		pItemNum->Transform()->SetLocalPos(Vector3(-res.fWidth / 2, -res.fHeight / 2, 1.f));
		pItemNum->Transform()->SetLocalScale(Vector3(30.f, 30.f, 1.f));
		//MeshRender ¼³Á¤
		pItemNum->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pItemNum->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
		pItemNum->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"0").GetPointer());
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pItemNum);

		m_vItemNum.push_back(pItemNum);
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