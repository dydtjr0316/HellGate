#include "pch.h"
#include "StaticUI.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "RenderMgr.h"
#include "Button.h"

void CStaticUI::CreatePickingObj()
{
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"MousePoint");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider);
	pObject->AddComponent(new CMeshRender);
	pObject->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vector3(CKeyMgr::GetInst()->GetMousePos().x, CKeyMgr::GetInst()->GetMousePos().y, 1.f));
	pObject->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"RectMtrl"));
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"PUI")->AddGameObject(pObject);
	m_pMousePoint = pObject;
	//g_netMgr.SetObj(pObject);
}

void CStaticUI::CreateButton(CCamera* _camObj)
{
	//	item UI, 상속되는 버튼들
	CGameObject* pObject = new CGameObject;
	Vector3 vScale = Vector3(400.f, 600.f, 1.f);
	pObject->SetName(L"Item UI Object");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CStaticUI);
	pObject->StaticUI()->init(UI_TYPE::PRIVATE_ITEM_UI);
	pObject->StaticUI()->CreatePickingObj();
	// 투영행렬 statiUI 컴포넌트에 등록 (ORTHOGRAPHIC 카메라 정보 필요)
	pObject->StaticUI()->SetCameraProj(_camObj->Camera());
	// Transform 설정
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (3 * vScale.x)
		, (res.fHeight / 2.f) - (vScale.y / 2.f)
		, 1.f));
	pObject->Transform()->SetLocalScale(vScale);
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	Ptr<CTexture> itemUI = pObject->StaticUI()->m_pFrame;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

	//	Static Ui에 상속된 버튼들 Scene에 Obj로 추가
	for (int i = 0; i < pObject->StaticUI()->m_vecButton.size(); ++i)
	{
		vScale = Vector3(80.f, 120.f, 1.f);
		Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
		CGameObject* pButtonObj = new CGameObject;
		pButtonObj->SetName(L"Button Object");
		pButtonObj->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pButtonObj->AddComponent(new CTransform);
		pButtonObj->AddComponent(new CMeshRender);
		pButtonObj->AddComponent(new CCollider);
		pButtonObj->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
		//	버튼 Script 설정
		pButtonObj->AddComponent(pObject->StaticUI()->m_vecButton[i]);
		pObject->StaticUI()->m_vecButton[i]->SetParent(pObject->StaticUI());
		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		pButtonObj->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x) + 200.f
			, (res.fHeight / 2.f) - (vScale.y / 2.f)
			, 1.f));
		pButtonObj->Transform()->SetLocalScale(vScale);
		// MeshRender 설정
		pButtonObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pButtonObj->MeshRender()->SetMaterial(pMtrl->Clone());
		pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pObject->StaticUI()->m_vecButton[i]->GetImage().GetPointer());
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pButtonObj);
	}
}

void CStaticUI::CreateStoreButton()
{
	CGameObject* storeButton;

	for (int i = 0; i < (UINT)STORE_BUTTON::END; ++i) {
		storeButton = new CGameObject;
		storeButton->SetName(L"Button Object");
		storeButton->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		storeButton->AddComponent(new CTransform);
		storeButton->AddComponent(new CMeshRender);
		storeButton->Transform()->SetLocalPos(Vector3(0.0f, 0.0f, 1.0f));
		storeButton->Transform()->SetLocalScale(Vector3(200.f, 50.f, 1.f));
		//MeshRender 설정
		storeButton->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		storeButton->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"WalletMtrl"));
		storeButton->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"LACK_MONEY").GetPointer());
		storeButton->SetUiRenderCheck(false);
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(storeButton);
		m_StoreButton.push_back(storeButton);
	}
}

void CStaticUI::SetButton(ITEM_ID _id)
{
	for (int i = 0; i < m_vecButton.size(); ++i)
	{
		if (m_vecButton[i]->GetItemID() == _id) {
			m_vecButton[i]->AddItemCount(); return;
		//	m_vecButton[i]->SetChangeCount(true);
		}
		else if (m_vecButton[i]->GetItemID() == ITEM_ID::EMPTY)
		{
			//*(m_vecButton[i]->GetObj()) = *_obj;
			m_vecButton[i]->SetItemID(_id);
			m_vecButton[i]->AddItemCount();
		//	m_vecButton[i]->SetChangeCount(true);
			return;
		}
		else
			cout << "아이템 창 꽉 참" << endl;
	}
}

void CStaticUI::init(UI_TYPE _eType)
{
	m_bActive = false;
	m_eType = _eType;
	m_pFrame = CResMgr::GetInst()->Load<CTexture>(L"ButtonFrame", L"Texture\\Terrain\\Lava1.png");

	for (int i = 0; i < 16; ++i)
	{
		m_vecButton.push_back(new CButton);
	}

	for (int i = 0; i < (UINT)ITEM_ID::END; ++i)
		m_bUseItem.push_back(false);
}

void CStaticUI::update()
{
	GetObj()->SetUiRenderCheck(m_bActive);
	if (m_eType == UI_TYPE::PRIVATE_ITEM_UI) {
		m_pWallet->SetUiRenderCheck(m_bActive);
		for (int i = 0; i < m_pMoneyUi.size(); ++i)
			m_pMoneyUi[i]->SetUiRenderCheck(m_bActive);
	}

	if (KEY_TAB(KEY_TYPE::KEY_I) && (m_eType == UI_TYPE::PRIVATE_ITEM_UI))
	{
		m_bActive = !m_bActive;
		for (int i = 0; i < m_vecButton.size(); ++i)
			m_vecButton[i]->SetActive(m_bActive);
		if (m_eType == UI_TYPE::PRIVATE_ITEM_UI)
			SetWalletMoney();
	}

	if (!m_bActive)
		return;
	else
	{
		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))		//	피킹 후 드래그
		{
			Vector3 vec = CKeyMgr::GetInst()->GetTransformedPoint(m_pCameraProj->GetProjMat());
			m_pMousePoint->Transform()->SetLocalPos(Vector3(vec.x, vec.y, 1.f));
		}
		else if (KEY_TAB(KEY_TYPE::KEY_LBTN))	//	키를 눌렸을 때 충돌검사를 하고 충돌 됐으면 BT_ACTIVE 상태값을 충돌된 버튼만 수정한다/
		{	

		}
		else if (KEY_AWAY(KEY_TYPE::KEY_LBTN))	//	피킹 후 상태 이상이 없으면 다시 제자리로 돌아간다.
		{
			for (int i = 0; i < m_vecButton.size(); ++i)
			{
				if (m_vecButton[i]->GetState() == BT_STATE::CLICKED)
				{
					m_vecButton[i]->OnCollisionExit(m_pMousePoint->Collider());

				}
			}
		}

		if (KEY_TAB(KEY_TYPE::KEY_RBTN) && (m_eType == UI_TYPE::PRIVATE_ITEM_UI) && (m_bStoreTime == false))
		{
			for (int i = 0; i < m_vecButton.size(); ++i) {
				Vector3 Pos = m_vecButton[i]->GetObj()->Transform()->GetLocalPos();
				Vector3 Scale = m_vecButton[i]->GetObj()->Transform()->GetLocalScale();
				
				if (m_vecButton[i]->GetItemID() != ITEM_ID::EMPTY) {
					if (ComputeMousePos(Pos, Scale)) {
						UseItem(m_vecButton[i]->GetItemID());
						if (m_vecButton[i]->GetItemID() != ITEM_ID::AX && m_vecButton[i]->GetItemID() != ITEM_ID::BASIC_SWORD) {
							m_vecButton[i]->SubItemCount();
							if (m_vecButton[i]->GetItemCount() == 0)
								m_vecButton[i]->SetItemID(ITEM_ID::EMPTY);
						}
					}
				}
			}
		}
		/*else if (KEY_HOLD(KEY_TYPE::KEY_RBTN))
		{

		}
		else if (KEY_AWAY(KEY_TYPE::KEY_RBTN))	
		{
			for (int i = 0; i < m_vecButton.size(); ++i)
			{
				if (m_vecButton[i]->GetState() == BT_STATE::CLICKED)
				{
					m_vecButton[i]->OnCollisionExit(m_pMousePoint->Collider());

				}
			}
		}*/


		for (int i = 0; i < m_vecButton.size(); ++i) {
			Vector3 Pos = m_vecButton[i]->GetObj()->Transform()->GetLocalPos();
			Vector3 Scale = m_vecButton[i]->GetObj()->Transform()->GetLocalScale();

			if (m_vecButton[i]->GetItemID() != ITEM_ID::EMPTY) {
				if (ComputeMousePos(Pos, Scale))
					m_vecButton[i]->GetExplainBox()->SetUiRenderCheck(true);
				else
					m_vecButton[i]->GetExplainBox()->SetUiRenderCheck(false);
			}
			/*else
				m_vecButton[i]->GetExplainBox()->SetUiRenderCheck(false);*/
		}
	}
}


bool CStaticUI::ComputeMousePos(Vector3& _pos, Vector3& _scale)
{
	POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	pMousePos.x -= res.fWidth / 2.f;
	pMousePos.y -= res.fHeight / 2.f;
	pMousePos.y = -pMousePos.y;

	if (pMousePos.x >= _pos.x - (_scale.x / 2) && pMousePos.x <= _pos.x + (_scale.x / 2)
		&& pMousePos.y <= _pos.y + (_scale.y / 2) && pMousePos.y >= _pos.y - (_scale.y / 2))
		return true;
	return false;
}

int CStaticUI::GetQuestItemCount()
{
	for (int i = 0; i < m_vecButton.size(); ++i)
	{
		if (m_vecButton[i]->GetItemID() == ITEM_ID::STEAK) {
			return m_vecButton[i]->GetItemCount();
		}
	}

	return 0;
}

void CStaticUI::SetWalletMoney()
{
	int unit = m_iMoney % 10;
	int tens = (m_iMoney / 10) % 10;
	int hundreds = (m_iMoney / 100) % 10;
	int thousands = (m_iMoney / 1000) % 10;
	int tenThousands = m_iMoney / 10000;
	// 1의 자리
	string str = to_string(unit);
	wstring wstr = wstring(str.begin(), str.end());
	m_pMoneyUi[4]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	// 10의 자리
	str = to_string(tens);
	wstr = wstring(str.begin(), str.end());
	m_pMoneyUi[3]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	// 100의 자리
	str = to_string(hundreds);
	wstr = wstring(str.begin(), str.end());
	m_pMoneyUi[2]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	// 1000의 자리
	str = to_string(thousands);
	wstr = wstring(str.begin(), str.end());
	m_pMoneyUi[1]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	// 10000의 자리
	str = to_string(tenThousands);
	wstr = wstring(str.begin(), str.end());
	m_pMoneyUi[0]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
}

void CStaticUI::UseItem(ITEM_ID _eType)
{
	switch (_eType) {
	case ITEM_ID::STEAK:
		m_bUseItem[(UINT)ITEM_ID::STEAK] = true;
		break;
	case ITEM_ID::BOTTLE_STAMINA:
		m_bUseItem[(UINT)ITEM_ID::BOTTLE_STAMINA] = true;
		break;
	case ITEM_ID::BOTTLE_DASH:
		m_bUseItem[(UINT)ITEM_ID::BOTTLE_DASH] = true;
		break;
	case ITEM_ID::CARROT:
		m_bUseItem[(UINT)ITEM_ID::CARROT] = true;
		break;
	case ITEM_ID::APPLE:
		m_bUseItem[(UINT)ITEM_ID::APPLE] = true;
		break;
	case ITEM_ID::BASIC_SWORD:
		m_bUseItem[(UINT)ITEM_ID::BASIC_SWORD] = true;
		break;
	case ITEM_ID::AX:
		m_bUseItem[(UINT)ITEM_ID::AX] = true;
		break;
	}
}

void CStaticUI::finalupdate()
{
	GetObj()->SetUiRenderCheck(m_bActive);
}

CStaticUI::CStaticUI()
	: CComponent(COMPONENT_TYPE::UI)
	, m_pMousePoint{ nullptr }
	, m_pWallet{ nullptr }
{
}

CStaticUI::~CStaticUI()
{
}

void CStaticUI::SaveToScene(FILE* _pFile)
{
}

void CStaticUI::LoadFromScene(FILE* _pFile)
{
}