#include "pch.h"
#include "PlayerScript.h"
#include "BulletScript.h"
#include "RenderMgr.h"
#include "SwordScript.h"
#include "StaticUI.h"
#include "Button.h"
#include "Quest.h"
#include "Sound.h"
#include <iostream>


int anicnt = 0;
using namespace std;

bool checkOnce = true;
int movePacketSendCnt;
bool ReckonerMove = false;

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
	
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Init()
{
	m_fSpeed = PLAYER_SPEED_IDLE;
	m_eAniType = Ani_TYPE::IDLE;

	// ui create
	Ptr<CTexture> UiTexture[2] = {
		CResMgr::GetInst()->FindRes<CTexture>(L"UiHug"),
		CResMgr::GetInst()->FindRes<CTexture>(L"UiTemper")
	};

	Vector3 vScale(350.f, 10.f, 1.f);
	CGameObject* pObject = new CGameObject;
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	for (int i = 0; i < 4; ++i) {
		pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// ����ü �ø� ������� ����
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		if (i == 0 || i == 1) {
			if (i == 1) {
				vScale = Vector3(350.f, 20.f, 1.f);
			}
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 60.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 2 || i == 3) {
			vScale = Vector3(350.f, 2.f, 1.f);
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 60.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}
		pObject->Transform()->SetLocalScale(vScale);
		// MeshRender ����
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		// AddGameObject
		if (GetObj()->GetID() == g_myid)
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

		if (i == 0 || i == 1)
			m_vUiBar.push_back(pObject);
		else if (i == 2 || i == 3)
			m_vUnderBar.push_back(pObject);
	}

	vScale = Vector3(350.f, 10.f, 1.f);

	for (int i = 0; i < 4; ++i) {

		pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// ����ü �ø� ������� ����
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		if (i == 0 || i == 1) {
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 600.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 2 || i == 3) {
			vScale = Vector3(350.f, 2.f, 1.f);
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 600.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender ����
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->SetUiRenderCheck(false);
		// AddGameObject
		if (GetObj()->GetID() == g_myid)
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

		if (i == 0 || i == 1)
			m_vUiBar.push_back(pObject);
		else if (i == 2 || i == 3)
			m_vUnderBar.push_back(pObject);
	}

	vScale = Vector3(40.f, 40.f, 1.f);

	for (int i = 0; i < 2; ++i) {

		pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// ����ü �ø� ������� ����
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 10.f + (410 * i)
			, (res.fHeight / 2.f) - (vScale.y / 2.f) - 10.f
			, 1.f));
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender ����
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, UiTexture[i].GetPointer());
		// AddGameObject
		if (GetObj()->GetID() == g_myid)
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

		m_vUiButton.push_back(pObject);
	}

	//	-----------------------
	//	item UI, ��ӵǴ� ��ư��
	//	-----------------------

	pObject = new CGameObject;
	vScale = Vector3(600.f, 800.f, 1.f);

	pObject->SetName(L"Item UI Object");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CStaticUI);
	pObject->AddComponent(new CCollider);
	pObject->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
	pObject->StaticUI()->init(UI_TYPE::PRIVATE_ITEM_UI);
	pObject->StaticUI()->CreatePickingObj();
	
	
	// ������� statiUI ������Ʈ�� ��� (ORTHOGRAPHIC ī�޶� ���� �ʿ�)
	//pObject->StaticUI()->SetCameraProj(pUICam->Camera());
	
	
	////	�÷��̾� ��ũ��Ʈ(������Ʈ)�� StaticUI �ͼ�
	//pPlayerObj->GetScript<CPlayerScript>()->SetUIObj(pObject);
	
	
	// Transform ����

	res = CRenderMgr::GetInst()->GetResolution();
	pObject->Transform()->SetLocalPos(Vector3(300.f, 80.f, 1.f));
	pObject->Transform()->SetLocalScale(vScale);
	// MeshRender ����	
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UITexMtrl");
	pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	Ptr<CTexture> itemUI = pObject->StaticUI()->m_pFrame;
	float fUI = 0.5f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);
	// AddGameObject

	if (GetObj()->GetID() == g_myid)
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
	m_pItemUIObj = pObject;

	//	Static Ui�� ��ӵ� ��ư�� Scene�� Obj�� �߰�
	vScale = Vector3(100.f, 100.f, 1.f);
	Vector3 vObjectPos = pObject->Transform()->GetLocalPos();
	Vector3 vObjectScale = pObject->Transform()->GetLocalScale();
	float	fEmptyY = (vObjectScale.y - 100.f - (vScale.y * 4.f)) / 5.f;
	float	fEmptyX = (vObjectScale.x - (vScale.x * 4.f)) / 5.f;
	for (int i = 0; i < pObject->StaticUI()->m_vecButton.size(); ++i)
	{
		Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
		CGameObject* pButtonObj = new CGameObject;
		pButtonObj->SetName(L"Button Object");
		pButtonObj->FrustumCheck(false);// ����ü �ø� ������� ����
		pButtonObj->AddComponent(new CTransform);
		pButtonObj->AddComponent(new CMeshRender);
		pButtonObj->AddComponent(new CCollider);
		pButtonObj->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
		//	��ư Script ����
		pButtonObj->AddComponent(pObject->StaticUI()->m_vecButton[i]);
		pObject->StaticUI()->m_vecButton[i]->SetParent(pObject->StaticUI());
		pObject->StaticUI()->m_vecButton[i]->SetUiType(UI_TYPE::PRIVATE_ITEM_UI);
		// Transform ����
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		Vector3 result = Vector3(vObjectPos.x - (vObjectScale.x / 2.f) + (vScale.x / 2.f + fEmptyX) + ((vScale.x + fEmptyX) * (i % 4))
			, vObjectPos.y + (vObjectScale.y / 2.f) - (vScale.y / 2.f + fEmptyY) - ((vScale.y + fEmptyY) * (i / 4))
			, 1.f);
		pButtonObj->Transform()->SetLocalPos(result);
		pButtonObj->Transform()->SetLocalScale(vScale);
		// MeshRender ����
		pButtonObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pButtonObj->MeshRender()->SetMaterial(pMtrl->Clone());
		pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pObject->StaticUI()->m_vecButton[i]->GetImage().GetPointer());
		// AddGameObject
		if (GetObj()->GetID() == g_myid)
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pButtonObj);

		pObject->StaticUI()->m_vecButton[i]->init();
	}

	for (int i = 0; i < pObject->StaticUI()->m_vecButton.size(); ++i)
		pObject->StaticUI()->m_vecButton[i]->CreateExplainBox();

	// wallet create
	CGameObject* pWallet = new CGameObject;
	pWallet->SetName(L"Wallet");
	pWallet->FrustumCheck(false);
	pWallet->AddComponent(new CTransform);
	pWallet->AddComponent(new CMeshRender);
	pWallet->Transform()->SetLocalPos(Vector3(vObjectPos.x, vObjectPos.y - (vObjectScale.y / 2.f) + 50.f, 1.f));
	pWallet->Transform()->SetLocalScale(Vector3(120.f, 50.f, 1.f));
	//MeshRender ����
	pWallet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pWallet->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"WalletMtrl"));
	pWallet->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"WALLET").GetPointer());
	if (GetObj()->GetID() == g_myid)
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pWallet);
	pObject->StaticUI()->m_pWallet = pWallet;

	CGameObject* pMoney;
	for (int i = 0; i < 5; ++i) {
		pMoney = new CGameObject;
		pWallet->SetName(L"Money");
		pMoney->FrustumCheck(false);
		pMoney->AddComponent(new CTransform);
		pMoney->AddComponent(new CMeshRender);
		pMoney->Transform()->SetLocalPos(Vector3(vObjectPos.x + (pWallet->Transform()->GetLocalScale().x / 2.f) + 30.f + (i * 30.f), vObjectPos.y - (vObjectScale.y / 2.f) + 50.f, 1.f));
		pMoney->Transform()->SetLocalScale(Vector3(30.f, 30.f, 1.f));
		//MeshRender ����
		pMoney->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pMoney->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
		pMoney->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"0").GetPointer());
		
		if (GetObj()->GetID() == g_myid)
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pMoney);
		pObject->StaticUI()->m_pMoneyUi.push_back(pMoney);
	}
}

void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CPlayerScript::update()
{	
	uShort id = GetObj()->GetID();
	if (id >= MAX_USER)return;
	CTerrain* pTerrain = GetTerrain();
	const Vector3& xmf3Scale = Transform()->GetLocalScale();
	Vector3 localPos = Transform()->GetLocalPos();
	CTransform* playerTrans = Transform();
	Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vector3 vRot = Transform()->GetLocalRot();
	Vector3 worldDir;
	KEY_TYPE tempAnimation;
	char dir = MV_IDLE;
	bool moveKeyInput = false;
	//cout << localPos.x << "\t" << localPos.z << endl;
	if (g_myid != GetObj()->GetID())
		op_Move();
	else
	{
		if (KEY_TAB(KEY_TYPE::KEY_N))
		{
			for (auto& obj : g_SoundList)
			{
				obj.second->VolumControl(false);
			}
		}
		if (KEY_TAB(KEY_TYPE::KEY_M))
		{
			for (auto& obj : g_SoundList)
			{
				obj.second->VolumControl(true);
			}
		}


		// ���� �ִϸ��̼�
		if (KEY_TAB(KEY_TYPE::KEY_R))
		{
			PlaySound_(Sound_Type::HIT);
			AnimClipReset();
			SetAttack(true);
			g_netMgr.Send_Player_Animation_Packet(id, GetAttack(), Ani_TYPE::ATTACK);
			anicnt++;
		}
		 
		if (GetAttack() && GetCnt(PlAYER_ANICNT_TYPE::ATTACK_CNT) < Animator3D()->GetAnimClip(0).dTimeLength) {
			SetCnt(GetCnt(PlAYER_ANICNT_TYPE::ATTACK_CNT) + DT, PlAYER_ANICNT_TYPE::ATTACK_CNT);
			SetAnimation(Ani_TYPE::ATTACK);
			moveKeyInput = true;
		}
		else if (GetCnt(PlAYER_ANICNT_TYPE::ATTACK_CNT) > Animator3D()->GetAnimClip(0).dTimeLength)
		{
			Attack_Default();

			SetAttack(false);
			SetCnt(0.f, PlAYER_ANICNT_TYPE::ATTACK_CNT);
			g_netMgr.Send_Player_Animation_Packet(id, GetAttack(), Ani_TYPE::ATTACK);

		}

		// ������ �ݱ� �ִϸ��̼� ȿ�� 0807(���� ���� ��)
		if (KEY_TAB(KEY_TYPE::KEY_E)) {
			PlaySound_(Sound_Type::GET_COIN);
			AnimClipReset();
			//SetAnimation(Ani_TYPE::PICK_UP);
			//CSound::GetInst()->Play(Sound_Type::GET_COIN);
			m_bIsPick = true;
			PickUp_Default();
			g_netMgr.Send_Player_Animation_Packet(id, m_bIsPick, Ani_TYPE::PICK_UP);


		}
		if (m_bIsPick == true && GetCnt(PlAYER_ANICNT_TYPE::PICKUP_CNT) < Animator3D()->GetAnimClip(0).dTimeLength) {
			SetCnt(GetCnt(PlAYER_ANICNT_TYPE::PICKUP_CNT) + DT, PlAYER_ANICNT_TYPE::PICKUP_CNT);
			SetAnimation(Ani_TYPE::PICK_UP);
			moveKeyInput = true;
		}
		else if (GetCnt(PlAYER_ANICNT_TYPE::PICKUP_CNT) > Animator3D()->GetAnimClip(0).dTimeLength)
		{

			m_bIsPick = false;
			SetCnt(0.f, PlAYER_ANICNT_TYPE::PICKUP_CNT);
			g_netMgr.Send_Player_Animation_Packet(id, m_bIsPick, Ani_TYPE::PICK_UP);


		}

		// ������ �ִϸ��̼�
		if (KEY_TAB(KEY_TYPE::KEY_Q)) {
			AnimClipReset();
			SetDamage(true);
			g_netMgr.Send_Player_Animation_Packet(id, GetDamage(), Ani_TYPE::DAMAGE);
		}
		if (GetDamage() && GetCnt(PlAYER_ANICNT_TYPE::DAMAGE_CNT) < Animator3D()->GetAnimClip(0).dTimeLength) {
			SetCnt(GetCnt(PlAYER_ANICNT_TYPE::DAMAGE_CNT) + DT, PlAYER_ANICNT_TYPE::DAMAGE_CNT);
			SetAnimation(Ani_TYPE::DAMAGE);
			moveKeyInput = true;
		}
		else if (GetCnt(PlAYER_ANICNT_TYPE::DAMAGE_CNT) > Animator3D()->GetAnimClip(0).dTimeLength)
		{
			//Attack_Default();

			SetDamage(false);
			SetCnt(0.f, PlAYER_ANICNT_TYPE::DAMAGE_CNT);
			g_netMgr.Send_Player_Animation_Packet(id, GetDamage(), Ani_TYPE::DAMAGE);

		}




		if (KEY_TAB(KEY_TYPE::KEY_SPACE) || KEY_AWAY(KEY_TYPE::KEY_SPACE))
		{
			SetChangeSpeed();
			//
			//cout << localPos.x << "\t" << localPos.z << endl;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			worldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
			localPos += worldDir * GetSpeed() * DT;
			dir = MV_FRONT;
			SetPlayerDir(worldDir * GetSpeed() * DT);
			SetAnimation(Ani_TYPE::RUN);
			moveKeyInput = true;
			//cout  << GetObj()->Transform()->GetLocalPos().x << " || " << GetObj()->Transform()->GetLocalPos().z << endl;
			//cout << worldDir.z << endl;
			//cout << DT << endl;

		}

		else if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			worldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
			localPos += worldDir * GetSpeed() * DT;

			dir = MV_BACK;
			SetAnimation(Ani_TYPE::RUN);
			moveKeyInput = true;
		}
		else if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			worldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
			localPos += worldDir * GetSpeed() * DT;

			dir = MV_LEFT;
			SetAnimation(Ani_TYPE::RUN);
			moveKeyInput = true;
		}

		else if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			worldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
			localPos += worldDir * GetSpeed() * DT;

			dir = MV_RIGHT;
			SetAnimation(Ani_TYPE::RUN);
			moveKeyInput = true;
		}
		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			vRot.y += vDrag.x * DT * ROTATE_SPEED;

			if (vDrag.x > 0)m_eDrag = Drag_Type::PLUS;
			else if (vDrag.x < 0)m_eDrag = Drag_Type::MINUS;
			else m_eDrag = Drag_Type::IDLE;

			//drag.x�� �����ָ�ǳ�
			if (vDrag.x != 0 && !moveKeyInput)
				g_netMgr.Send_Rotate_Packet(GetObj()->GetID(), vRot);


			//cout << "\t\t\t" << vDrag.x << endl;
			Transform()->SetLocalRot(vRot);
		}
		if (KEY_TAB(KEY_TYPE::KEY_Z))
		{
			// Boss Monster�� �̵�
			Vector3 vPos = Vector3(68000.f, 0.f, 65000.f);
			int z = (int)(vPos.z / xmf3Scale.z);

			float fHeight = pTerrain->GetHeight(vPos.x, vPos.z, ((z % 2) != 0)) * 2.f;


			if (vPos.y != fHeight)
				vPos.y = fHeight;

			playerTrans->SetLocalPos(vPos);
		}

		if (moveKeyInput)
		{
			int z = (int)(localPos.z / xmf3Scale.z);

			float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, ((z % 2) != 0)) * 2.f;


			if (localPos.y != fHeight)
				localPos.y = fHeight;
			playerTrans->SetLocalPos(localPos);
			//cout << "\t\t" << localPos.x << " , " << localPos.z << endl;
		}
		else
		{
			SetAnimation(Ani_TYPE::IDLE);
		}
		if (KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D))
			GetReckoner()->DeadReckoning(GetObj());

		if (((GetReckoner()->isFollowing() || !ReckonerMove) &&
			((KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D)))))
		{
			ReckonerMove = true;
			system_clock::time_point start = system_clock::now();
			g_netMgr.Send_Move_Packet(dir, localPos, worldDir, vRot.y, start, DT, ReckonerMove);

			GetReckoner()->SetDirVec(worldDir);
			GetReckoner()->SetRotateY(vRot.y);
			GetReckoner()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
			CountTime();
		}

		if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_D)))
		{
			ReckonerMove = false;
			g_netMgr.Send_Stop_Packet(false, g_myid);
			SetTime_Zero();
		}

		//if (KEY_HOLD(KEY_TYPE::KEY_X))
		//{
		//	Skill_Hide();
		//}

		//if (KEY_AWAY(KEY_TYPE::KEY_X))
		//{
		//	auto vecChild = GetObj()->GetChild();
		//	MeshRender()->SetMaterial(m_vecHideMtrl[0], 0);
		//	MeshRender()->SetMaterial(m_vecHideMtrl[1], 1);
		//	for (int i = 0; i < vecChild.size(); ++i)
		//	{
		//		if (vecChild[i]->GetName() == L"sword")
		//		{
		//			vecChild[i]->MeshRender()->SetMaterial(m_vecHideMtrl[2], 0);
		//			//vecChild[i]->MeshRender()->SetDynamicShadow(true);
		//			break;
		//		}
		//	}
		//	MeshRender()->SetDynamicShadow(true);
		//	m_bIsHide = false;
		//}


		if (m_ftimeCount >= m_fDelayTime)
		{
			ReckonerMove = true;
			system_clock::time_point start = system_clock::now();
			g_netMgr.Send_Move_Packet(dir, localPos, worldDir, vRot.y, start, DT, ReckonerMove);


			GetReckoner()->SetDirVec(worldDir);
			GetReckoner()->SetRotateY(vRot.y);
			GetReckoner()->SetLocalPos(Transform()->GetLocalPos());
			SetTime_Zero();
		}

		// quest::find item
		if (GetObj()->Quest()->GetDoQuest(QUEST_TYPE::GET_ITEM) == false)
			FindQuestItem();

		// stamina ui
		ClickUiButton();
		ReduceUiBar();
		UseItem();

		//hide
		if(m_fHidetime <= 60.f && m_bIsHide)
			Skill_Hide();
		else if (m_fHidetime > 60.f && m_bIsHide) {
			auto vecChild = GetObj()->GetChild();
			MeshRender()->SetMaterial(m_vecHideMtrl[0], 0);
			MeshRender()->SetMaterial(m_vecHideMtrl[1], 1);
			for (int i = 0; i < vecChild.size(); ++i)
			{
				if (vecChild[i]->GetName() == L"sword")
				{
					vecChild[i]->MeshRender()->SetMaterial(m_vecHideMtrl[2], 0);
					//vecChild[i]->MeshRender()->SetDynamicShadow(true);
					break;
				}
			}
			MeshRender()->SetDynamicShadow(true);
			m_bIsHide = false;
			m_fHidetime = 0.0f;
		}
	}
}

void CPlayerScript::op_Move()
{
	//sc_packet_move* p = GetObj()->GetScript<CPlayerScript>()->GetOtherMovePacket();

	//if (p == nullptr)return;
	uShort id = GetObj()->GetID();
	if (g_Object.count(id) == 0)return;
	if (id >= START_MONSTER)return;

	CTransform* playerTrans = Transform();
	CTerrain* pTerrain = GetTerrain();
	const Vector3& xmf3Scale = Transform()->GetLocalScale();
	Vector3 temp;
	if (GetOtherMovePacket() == nullptr)return;

	if (packetMoving)
	{
		/*cout << playerTrans->GetLocalPos().x << "\t" << playerTrans->GetLocalPos().z << endl;
		cout << "PACKETPOS : " << packetLocalPos.x << ", " << packetLocalPos.z << endl;
		cout << "DIRVEC : " << packetDirVec.x << ", " << packetDirVec.z << endl;
		cout << DT << endl;
		cout << "-------------------------" << endl;*/
		if (FirstPacket)
		{
			temp = packetLocalPos + packetDirVec * packetspeed * (DT);
			SetBisFrist(false);
		}
		else
		{
			temp = playerTrans->GetLocalPos() + packetDirVec * packetspeed * (DT);
		}
		playerTrans->SetLocalRot(packetrotateY);

		int z = (int)(temp.z / xmf3Scale.z);
		float fHeight = pTerrain->GetHeight(temp.x, temp.z, ((z % 2) != 0)) * 2.f;

		if (temp.y != fHeight)
			temp.y = fHeight;

		playerTrans->SetLocalPos(temp);
			//	cout << "ID : "<<GetObj()->GetID() << "\t\t" << playerTrans->GetLocalPos().x << " , " << playerTrans->GetLocalPos().z << endl;
			//cout << "-----------------------------" << endl;

	}
	else {
		/*if (m_isBezier&&m_fRTT>=0.f)
		{
			cout << "�����Ѵ� ~~!!!" << endl;

			Search_Origin_Points(id, m_fRTT);
			Compute_Bezier(GetOriginPoint(), GetInterpolationPoint());
			CTransform* ObjTrans = Transform();;
			ObjTrans->SetLocalRot(Vector3(0.f, packetrotateY, 0.f));
			if (GetInterpolationCnt() != 4)
			{
				cout << "cnt : " << GetInterpolationCnt() << endl;
				ObjTrans->SetLocalPos(Vector3(GetInterpolationPoint()[GetInterpolationCnt()].x,
					packetLocalPos.y,
					GetInterpolationPoint()[GetInterpolationCnt()].y));
				cout << ObjTrans->GetLocalPos().x << endl;
				cout << ObjTrans->GetLocalPos().z << endl << endl;
				InterpolationCnt_PP();
			}
			else
			{
				SetisBezeir(false);
			}
		}*/
	}
}

void CPlayerScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	m_movePacketTemp = new sc_packet_move;
	
	packetLocalPos = p->localVec;
	packetDir = p->dir;
	packetDirVec = p->dirVec;
	packetspeed = p->speed;
	packetrotateY = p->rotateY;
	pacektID = p->id;
	packetMoving = p->isMoving;
	//m_movePacketTemp = p;
	m_fRTT = rtt;
	
}

void CPlayerScript::SetOrigin_Point(const int& index, const float& x, const float& y)
{
	m_v2Origin_Point[index].x = x;
	m_v2Origin_Point[index].y = y;
	//cout << "Set Origin Point : " << m_v2Origin_Point[index].x << ", "<< m_v2Origin_Point[index].y <<endl;
}

void CPlayerScript::SetInterpolation_Point(const int& index, const float& x, const float& y)
{
	m_v2Interpolation_Point[index].x = x;
	m_v2Interpolation_Point[index].y = y;
}

void CPlayerScript::Search_Origin_Points(const int& id, const float& rtt)
{
	Vector3 tempLocalPos = Transform()->GetLocalPos();
	if (rtt < 0)return;
	/*cout << "�����Ҷ� Ŭ��   : \t" << tempLocalPos.x << ", " << tempLocalPos.z << endl;
	cout << "�����Ҷ� dirvec   : \t" << packetDirVec.x << ", " << packetDirVec.z << endl;
	cout << "�����Ҷ� speed rtt   : \t" << packetspeed <<", " << rtt << endl;*/
	for (int i = 0; i < 4; i++) {

		if (packetLocalPos.x <= tempLocalPos.x)
			tempLocalPos.x += (packetDirVec.x * packetspeed * rtt);
		else
			tempLocalPos.x -= (packetDirVec.x * packetspeed * rtt);

		if (packetLocalPos.z <= tempLocalPos.z)
			tempLocalPos.z += (packetDirVec.z * packetspeed * rtt);
		else
			tempLocalPos.z -= (packetDirVec.z * packetspeed * rtt);
		SetOrigin_Point(i, tempLocalPos.x, tempLocalPos.z);

	}
}

Vector2 CPlayerScript::Search_Interpolation_Points(Vector2* points, float time)
{
	float ax, bx, cx, ay, by, cy, time_Double, time_Triple;
	Vector2 result;

	cx = 3.0f * (points[1].x - points[0].x);
	bx = 3.0f * (points[2].x - points[1].x) - cx;
	ax = (points[3].x - points[0].x) - cx - bx;

	cy = 3.0f * (points[1].y - points[0].y);
	by = 3.0f * (points[2].y - points[1].y) - cy;
	ay = (points[3].y - points[0].y) - cy - by;

	time_Double = time * time;
	time_Triple = time_Double * time;

	result.x = (ax * time_Triple) + (bx * time_Double) + (cx * time) + points[0].x;
	result.y = (ay * time_Triple) + (bx * time_Double) + (cy * time) + points[0].y;
	//cout << "Set Interpor Point : " << result.x << ", " << result.y << endl;

	return result;
}

void CPlayerScript::Compute_Bezier(Vector2* points, Vector2* dest)
{
	float dt;
	int i;
	dt = m_fRTT / 3.f;

	for (i = 0; i < 4; i++)
	{

		Vector2 inst = Search_Interpolation_Points(points, i * dt);

		SetInterpolation_Point(i, inst.x, inst.y);
	}
}

void CPlayerScript::SetAnimation(const Ani_TYPE& type)
{
	if (m_pAniData.size() == 0)return;
	Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	MeshRender()->SetMesh(m_pAniData[(int)type]);
}

bool CPlayerScript::isInMap(const Vector3& localPos)
{
	if ((localPos.x > 200 && localPos.x < 6200) && (localPos.z > 200 && localPos.z < 6200))return true;
	else return false;
}

void CPlayerScript::OnCollisionEnter(CCollider* _pOther)
{

	auto a = _pOther->GetObj()->GetName();
	if (L"M_Attack Object" == a)
	{
	
		
	}
}

void CPlayerScript::OnCollision(CCollider* _pOther)
{
	if (_pOther->Collider()->GetColliderType() == COLLIDER_TYPE::RANGE)
		return;

	BoundingSphere myBS = Collider()->GetBoundingSphere();
	BoundingSphere otherBS = _pOther->Collider()->GetBoundingSphere();

	
	Vector3 localPos = Transform()->GetLocalPos();
	localPos += -GetPlayerDir() * 2;
	Transform()->SetLocalPos(localPos);

}

void CPlayerScript::OnCollisionExit(CCollider* _pOther)
{
}

void CPlayerScript::Attack_Default()
{
	Vector3 vPos = Transform()->GetLocalPos();

	vector<CGameObject*> vecObj;
	CSceneMgr::GetInst()->FindGameObjectByTag(L"Attack Object", vecObj);

	if (!vecObj.empty())
	{
		cout << "�Ѿ� ��ü ���� �ȉ�" << endl;
		return;
	}
	else
		cout << "����" << endl << endl;

	CGameObject* pBullet = new CGameObject;
	pBullet->SetName(L"Attack Object");

	vPos += -Transform()->GetWorldDir(DIR_TYPE::FRONT) * Collider()->GetBoundingSphere().Radius;
	pBullet->AddComponent(new CTransform());
	pBullet->Transform()->SetLocalPos(vPos);
	pBullet->AddComponent(new CCollider);
	pBullet->Collider()->SetColliderType(COLLIDER_TYPE::BOX);
	pBullet->Collider()->SetBoundingSphere(BoundingSphere(vPos, 100.f));
	pBullet->AddComponent(new CBulletScript);
	CBulletScript* bulletScript = pBullet->GetScript<CBulletScript>();
	bulletScript->SetPlayer(GetObj());

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Bullet")->AddGameObject(pBullet);
	//CreateObject(pBullet, L"Bullet");
}

void CPlayerScript::PickUp_Default()
{
	Vector3 vPos = Transform()->GetLocalPos();
	CGameObject* pBullet = new CGameObject;
	pBullet->SetName(L"PickUP Object");
	
	pBullet->AddComponent(new CTransform());
	pBullet->AddComponent(new CCollider);
	pBullet->Transform()->SetLocalPos(vPos);
	pBullet->Collider()->SetColliderType(COLLIDER_TYPE::BOX);
	//	���� Sphere �� �޾ƿͼ� GetHeight���� 200.f ��ŭ ���Ѱ��� �����༭ �ݸ��� ����.
	BoundingSphere sphere = Collider()->GetBoundingSphere();
	sphere.Center.y += 200.f;
	pBullet->Collider()->SetBoundingSphere(BoundingSphere( sphere.Center, sphere.Radius * 5.f));
	pBullet->AddComponent(new CBulletScript);
	pBullet->GetScript<CBulletScript>()->SetBulletType(BULLET_TYPE::PICKUP);
	pBullet->GetScript<CBulletScript>()->SetUIObj(m_pItemUIObj);


	CreateObject(pBullet, L"Player");
}

void CPlayerScript::Skill_Hide()
{
	m_vecHideMtrl.push_back(MeshRender()->GetCloneMaterial(0));
	m_vecHideMtrl.push_back(MeshRender()->GetCloneMaterial(1));

	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"), 0);
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"), 1);

	const vector<CGameObject*>& vecChild = GetObj()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		auto a = vecChild[i]->MeshRender()->GetMesh()->GetName();
		auto b = vecChild[i]->GetName();

		if (vecChild[i]->GetName() == L"sword") 
		{
			m_vecHideMtrl.push_back(vecChild[i]->MeshRender()->GetCloneMaterial(0));
			vecChild[i]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"), 0);
			vecChild[i]->MeshRender()->SetDynamicShadow(false);
		}
	}	
	MeshRender()->SetDynamicShadow(false);
	m_bIsHide = true;
	m_fHidetime += DT;
}

void CPlayerScript::ClickUiButton()
{
	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

		for (int i = 0; i < m_vUiButton.size(); ++i) {
			Vector3 vUiButtonPos = m_vUiButton[i]->Transform()->GetLocalPos();
			Vector3 vUiButtonScale = m_vUiButton[i]->Transform()->GetLocalScale();
			tResolution res = CRenderMgr::GetInst()->GetResolution();

			vUiButtonPos.x += res.fWidth / 2;
			vUiButtonPos.y += -(res.fHeight / 2);

			if (pMousePos.x >= vUiButtonPos.x - (vUiButtonScale.x / 2) && pMousePos.x <= vUiButtonPos.x + (vUiButtonScale.x / 2)
				&& pMousePos.y <= -vUiButtonPos.y + (vUiButtonScale.x / 2) && pMousePos.y >= -vUiButtonPos.y - (vUiButtonScale.x / 2)) {

				if (m_bUiCheck == true) {
					// m_pObject->Transform()->SetLocalScale(Vector3(90.f, 90.f, 1.f));
					m_vUiBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(true);
					m_vUnderBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(true);
					m_bUiCheck = false;
				}
				else if (m_bUiCheck == false) {
					m_vUiBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(false);
					m_vUnderBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(false);
					m_bUiCheck = true;
				}
			}
		}
	}
}

void CPlayerScript::ReduceUiBar()
{
	float speed{};
	for (int i = 2; i < (UINT)UI_BAR::END; ++i) {
		if (i == (UINT)UI_BAR::HUG)
			speed = 0.4f;
		else
			speed = 0.25f;

		Vector3 scale = m_vUiBar[i]->Transform()->GetLocalScale();
		Vector3 pos = m_vUiBar[i]->Transform()->GetLocalPos();
		
		//float a = (0.05 * DT);
		scale.x -= (speed * DT);
		pos.x -= (speed * DT) / 2.f;

		m_vUiBar[i]->Transform()->SetLocalScale(Vector3(scale.x, scale.y, scale.z));
		m_vUiBar[i]->Transform()->SetLocalPos(Vector3(pos.x, pos.y, pos.z));
	}
}

void CPlayerScript::IncreaseUiBar(float _stamina, float _dash, float _hug, float _temper)
{
	for (int i = 0; i < (UINT)UI_BAR::END; ++i) {
		Vector3 scale = m_vUiBar[i]->Transform()->GetLocalScale();
		Vector3 pos = m_vUiBar[i]->Transform()->GetLocalPos();
		
		switch (i) {
		case 0:
			if ((scale.x + _stamina) >= 350.f)
				_stamina = 350.f - scale.x;
			scale.x += _stamina;
			pos.x += _stamina / 2.f;
			break;
		case 1:
			if ((scale.x + _dash) >= 350.f)
				_dash = 350.f - scale.x;
			scale.x += _dash;
			pos.x += _dash / 2.f;
			break;
		case 2:
			if ((scale.x + _hug) >= 350.f)
				_hug = 350.f - scale.x;
			scale.x += _hug;
			pos.x += _hug / 2.f;
			break;
		case 3:
			if ((scale.x + _temper) >= 350.f)
				_temper = 350.f - scale.x;
			scale.x += _temper;
			pos.x += _temper / 2.f;
			break;
		}
		
		m_vUiBar[i]->Transform()->SetLocalScale(scale);
		m_vUiBar[i]->Transform()->SetLocalPos(pos);
	}
}


void CPlayerScript::ReduceHpBar()
{
	float damage = 20.f;

	Vector3 scale = m_vUiBar[(UINT)UI_BAR::STAMINA]->Transform()->GetLocalScale();
	Vector3 pos = m_vUiBar[(UINT)UI_BAR::STAMINA]->Transform()->GetLocalPos();

	scale.x -= damage;
	pos.x -= damage / 2.f;

	m_vUiBar[(UINT)UI_BAR::STAMINA]->Transform()->SetLocalScale(Vector3(scale.x, scale.y, scale.z));
	m_vUiBar[(UINT)UI_BAR::STAMINA]->Transform()->SetLocalPos(Vector3(pos.x, pos.y, pos.z));
}



void CPlayerScript::UseItem()
{
	if(m_pItemUIObj->StaticUI()->m_bActive == true){
		for (int i = 0; i < (UINT)ITEM_ID::END; ++i) {
			if (m_pItemUIObj->StaticUI()->GetUseItemID(i) == true) {
				FindItemBeUsed(i);
			}
		}
	}
}

void CPlayerScript::FindItemBeUsed(int _itemId)
{
	switch (_itemId) {
	case (UINT)ITEM_ID::STEAK:	// steak
		cout << "steak ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::STEAK, false);
		IncreaseUiBar(0.0f, 0.0f, 10.f, 5.f);
		break;
	case (UINT)ITEM_ID::BOTTLE_STAMINA:
		cout << "stamina ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::BOTTLE_STAMINA, false);
		IncreaseUiBar(30.0f, 0.0f, 0.f, 0.f);
		break;
	case (UINT)ITEM_ID::BOTTLE_DASH:
		cout << "dash ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::BOTTLE_DASH, false);
		IncreaseUiBar(0.0f, 20.0f, 0.f, 0.f);
		break;
	case (UINT)ITEM_ID::MONEYBAG:
		break;
	case (UINT)ITEM_ID::CARROT:
		cout << "carrot ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::CARROT, false);
		IncreaseUiBar(0.0f, 0.0f, 10.f, 5.f);
		break;
	case (UINT)ITEM_ID::APPLE:
		cout << "apple ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::CARROT, false);
		IncreaseUiBar(0.0f, 0.0f, 20.f, 10.f);
		break;
	case (UINT)ITEM_ID::BRANCH:
		break;
	case (UINT)ITEM_ID::NEW_SWORD:
		cout << "NEW_SWORD ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::NEW_SWORD, false);
		ChangeWeapone(WEAPONE_TYPE::SWORD_NEW, ITEM_ID::NEW_SWORD);
		//CSwordScript* swordScript = GetObj()->GetChild()->GetScript<CSwordScript>();
		break;
	case (UINT)ITEM_ID::BASIC_ARROW:
		break;
	case (UINT)ITEM_ID::AX:
		cout << "AX ���" << endl;
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::AX, false);
		ChangeWeapone(WEAPONE_TYPE::AX, ITEM_ID::AX);
		break;
	case (UINT)ITEM_ID::BASIC_SWORD:
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::BASIC_SWORD, false);
		ChangeWeapone(WEAPONE_TYPE::SWORD, ITEM_ID::BASIC_SWORD);
		break;
	case (UINT)ITEM_ID::BOTTLE_SHADOW:
		m_pItemUIObj->StaticUI()->SetUseItemID((UINT)ITEM_ID::BOTTLE_SHADOW, false);
		m_bIsHide = true;
		break;
	default:
		break;
	}
}

void CPlayerScript::ChangeWeapone(WEAPONE_TYPE _eType, ITEM_ID _iTemID)	// �ٲ� ������
{
	const vector<CGameObject*>& vecChild = GetObj()->GetChild();
	wstring a;
	wstring b;

	for (int i = 0; i < vecChild.size(); ++i) {

		a = vecChild[i]->MeshRender()->GetMesh()->GetName();
		b = vecChild[i]->GetName();

		if (vecChild[i]->GetName() == L"sword") {

			switch (_iTemID) {
			case ITEM_ID::NEW_SWORD:
				if (vecChild[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\PlayerMale_Weapon_Sword.mesh") {
					vecChild[i]->GetScript<CSwordScript>()->SetMeshData(_eType);
					vecChild[i]->Transform()->SetLocalRot(Vector3(0.f, XM_PI / 2.f, 0.f));
					vecChild[i]->Transform()->SetLocalPos(Vector3(-30.f, 0.f, -30.f));

					for (int j = 0; j < m_pItemUIObj->StaticUI()->m_vecButton.size(); ++j) {
						if (m_pItemUIObj->StaticUI()->m_vecButton[j]->GetItemID() == _iTemID) {
							m_pItemUIObj->StaticUI()->m_vecButton[j]->SetItemID(ITEM_ID::BASIC_SWORD);
						}
					}
				}
				// ����
				if(vecChild[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\Ax.mesh") {
					vecChild[i]->GetScript<CSwordScript>()->SetMeshData(_eType);
					vecChild[i]->Transform()->SetLocalRot(Vector3(0.f, XM_PI / 2.f, 0.f));
					//	vecChild[i]->Transform()->SetLocalPos(Vector3(-30.f, 0.f, -30.f));

					for (int j = 0; j < m_pItemUIObj->StaticUI()->m_vecButton.size(); ++j) {
						if (m_pItemUIObj->StaticUI()->m_vecButton[j]->GetItemID() == _iTemID) {
							m_pItemUIObj->StaticUI()->m_vecButton[j]->SetItemID(ITEM_ID::AX);
						}
					}
				}
				break;
			case ITEM_ID::BASIC_SWORD:
				if (vecChild[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\sword_2.mesh") {
					vecChild[i]->GetScript<CSwordScript>()->SetMeshData(_eType);
					vecChild[i]->Transform()->SetLocalRot(Vector3(0.f, 0.0f, 0.f));
					vecChild[i]->Transform()->SetLocalPos(Vector3(0.f, 0.f, -0.f));

					for (int j = 0; j < m_pItemUIObj->StaticUI()->m_vecButton.size(); ++j) {
						if (m_pItemUIObj->StaticUI()->m_vecButton[j]->GetItemID() == _iTemID) {
							m_pItemUIObj->StaticUI()->m_vecButton[j]->SetItemID(ITEM_ID::NEW_SWORD);
						}
					}
				}
				// ����
				if (vecChild[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\Ax.mesh") {
					vecChild[i]->GetScript<CSwordScript>()->SetMeshData(_eType);
					vecChild[i]->Transform()->SetLocalRot(Vector3(0.f, 0.0f, 0.f));
					vecChild[i]->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));

					for (int j = 0; j < m_pItemUIObj->StaticUI()->m_vecButton.size(); ++j) {
						if (m_pItemUIObj->StaticUI()->m_vecButton[j]->GetItemID() == _iTemID) {
							m_pItemUIObj->StaticUI()->m_vecButton[j]->SetItemID(ITEM_ID::AX);
						}
					}
				}
				break;
			case ITEM_ID::AX:
				if (vecChild[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\PlayerMale_Weapon_Sword.mesh") {
					vecChild[i]->GetScript<CSwordScript>()->SetMeshData(_eType);
					vecChild[i]->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
					//vecChild[i]->Transform()->SetLocalPos(Vector3(-30.f, 0.f, -30.f));

					for (int j = 0; j < m_pItemUIObj->StaticUI()->m_vecButton.size(); ++j) {
						if (m_pItemUIObj->StaticUI()->m_vecButton[j]->GetItemID() == _iTemID) {
							m_pItemUIObj->StaticUI()->m_vecButton[j]->SetItemID(ITEM_ID::BASIC_SWORD);
						}
					}
				}

				if (vecChild[i]->MeshRender()->GetMesh()->GetName() == L"Mesh\\sword_2.mesh") {
					vecChild[i]->GetScript<CSwordScript>()->SetMeshData(_eType);
					vecChild[i]->Transform()->SetLocalRot(Vector3(0.f, XM_PI / 2.f, 0.f));
					vecChild[i]->Transform()->SetLocalPos(Vector3(-30.f, 0.f, -30.f));

					for (int j = 0; j < m_pItemUIObj->StaticUI()->m_vecButton.size(); ++j) {
						if (m_pItemUIObj->StaticUI()->m_vecButton[j]->GetItemID() == _iTemID) {
							m_pItemUIObj->StaticUI()->m_vecButton[j]->SetItemID(ITEM_ID::NEW_SWORD);
						}
					}
				}
				break;
			}
			
			//
		}
	}
}

void CPlayerScript::FindQuestItem()
{
	GetObj()->Quest()->SetQuestcount(QUEST_TYPE::GET_ITEM, m_pItemUIObj->StaticUI()->GetQuestItemCount());
}

void CPlayerScript::PlaySound_(const Sound_Type& sound)
{
	g_SoundList.find(sound)->second->Play(1);
}
