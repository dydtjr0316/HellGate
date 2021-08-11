#include "pch.h"
#include "StaticUI.h"
#include "Button.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"

void CButton::init()
{
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	int num{};

	if (m_eUiType == UI_TYPE::PRIVATE_ITEM_UI)
		num = (UINT)ITEM_NUM::HUNDREDS;
	else {
		num = (UINT)ITEM_NUM::END;
		SetItemPrice();
	}


	// item number create
	for (int i = 0; i < num; ++i) {
		CGameObject* pItemNum = new CGameObject;
		pItemNum->SetName(L"ItemNum");
		pItemNum->FrustumCheck(false);
		pItemNum->AddComponent(new CTransform);
		pItemNum->AddComponent(new CMeshRender);
		pItemNum->Transform()->SetLocalPos(Vector3(-res.fWidth / 2, -res.fHeight / 2, 1.f));
		pItemNum->Transform()->SetLocalScale(Vector3(30.f, 30.f, 1.f));
		//MeshRender ����
		pItemNum->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pItemNum->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
		pItemNum->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"0").GetPointer());
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pItemNum);

		m_vItemNum.push_back(pItemNum);
	}
}

void CButton::update()
{

	GetObj()->SetUiRenderCheck(m_bActive);
	for (int i = 0; i < m_vItemNum.size(); ++i)
		m_vItemNum[i]->SetUiRenderCheck(m_bActive);

	ChangeButtonTexture();
	
	if(m_bChangeCount)
		ChangeNumTexture();

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
		Vector3 vNumScale = m_vItemNum[i]->Transform()->GetLocalScale();

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

void CButton::ChangeButtonTexture()
{
	switch (m_eItemId)
	{
	case ITEM_ID::EMPTY:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"EMPTY");
		break;

	case ITEM_ID::BASIC_SWORD:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD");
		break;

	case ITEM_ID::AX:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"AX");
		break;

	case ITEM_ID::BASIC_ARROW:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOW_IMG");
		break;

	case ITEM_ID::BRANCH:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BRANCH");
		break;

	case ITEM_ID::APPLE:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"APPLE");
		break;

	case ITEM_ID::BOTTLE_STAMINA:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_STAMINA");
		break;

	case ITEM_ID::BOTTLE_DASH:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_DASH");
		break;

	case ITEM_ID::STEAK:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"STEAK");
		break;

	case ITEM_ID::MONEYBAG:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"MONEYBAG");
		break;

	case ITEM_ID::CARROT:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"CARROT");
		break;

	case ITEM_ID::END:
		return;
	}
	auto temp = m_pComParent->GetObj()->StaticUI()->GetButton();

	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pItemImage.GetPointer());

}

void CButton::ChangeNumTexture()
{
	if (m_iItemCount / 10 < 1) {	// ������ 1�� �ڸ�
		string str = to_string(m_iItemCount);
		wstring wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::UNIT]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	}
	else if((m_iItemCount / 10 >= 1) && (m_iItemCount / 100 < 1)){	// ������ 10�� �ڸ�
		int unit = m_iItemCount % 10;
		int tens = m_iItemCount / 10;
		// 1�� �ڸ�
		string str = to_string(unit);
		wstring wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::UNIT]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
		// 10�� �ڸ�
		str = to_string(tens);
		wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::TENS]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	}
	else if (m_iItemCount / 100 >= 1) {
		int unit = m_iItemCount % 10;
		int tens = (m_iItemCount / 10) % 10;
		int hundreds = (m_iItemCount / 10) / 10;
		// 1�� �ڸ�
		string str = to_string(unit);
		wstring wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::UNIT]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
		// 10�� �ڸ�
		str = to_string(tens);
		wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::TENS]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
		// 100�� �ڸ�
		str = to_string(hundreds);
		wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::HUNDREDS]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	}

	m_bChangeCount = false;
}

void CButton::SetItemPrice()
{
	switch (m_eItemId)
	{
	case ITEM_ID::EMPTY:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"EMPTY");
		break;

	case ITEM_ID::BASIC_SWORD:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD");
		m_iItemCount = 900;
		break;

	case ITEM_ID::AX:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"AX");
		m_iItemCount = 550;
		break;

	case ITEM_ID::BOTTLE_STAMINA:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_STAMINA");
		m_iItemCount = 25;
		break;

	case ITEM_ID::BOTTLE_DASH:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_DASH");
		m_iItemCount = 15;
		break;

	case ITEM_ID::STEAK:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"STEAK");
		m_iItemCount = 15;
		break;

	case ITEM_ID::END:
		return;
	}
}

CButton::CButton()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
	, m_eItemId(ITEM_ID::EMPTY)
	, m_bActive(false)
	, m_bState(BT_STATE::NONE)
	, m_bCheckActive(BT_ACTIVE::PASSIVE)
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