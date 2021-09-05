#include "pch.h"

#include <iostream>
#include <random>

#include "Sound.h"

#include "SceneMgr.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Light.h"
#include "Button.h"
#include "Quest.h"
#include "ParticleSystem.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Terrain.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "ItemMgr.h"
#include "Device.h"
#include "Core.h"

#include "PlayerScript.h"
#include "ToolCamScript.h"
#include "GridScript.h"
#include "SwordScript.h"
#include "MonsterScript.h"
#include "NpcScript.h"
#include "TreeScript.h"
#include "FenceScript.h"

// UI
#include "StaticUI.h"

unordered_map<Sound_Type, SoundMgr*> g_SoundList;
using namespace std;

default_random_engine dre;
uniform_real_distribution<float> uid(9000, 76000);

CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;
}

void CSceneMgr::ChangeScene(CScene* _pNextScene)
{
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pNextScene;
}

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::CreateTargetUI(CGameObject* _camObj)
{
#ifdef _DEBUG
	Vector3 vScale(150.f, 150.f, 1.f);
	
	Ptr<CTexture> arrTex[5] = { CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseLightTargetTex")
		, CResMgr::GetInst()->FindRes<CTexture>(L"SpecularLightTargetTex") };
	
	for (UINT i = 0; i < 5; ++i)
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
	
		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();
	
		pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + (i * vScale.x)
			, (res.fHeight / 2.f) - (vScale.y / 2.f)
			, 1.f));
	
		pObject->Transform()->SetLocalScale(vScale);
	
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, arrTex[i].GetPointer());
	
		// AddGameObject
		m_pCurScene->FindLayer(L"UI")->AddGameObject(pObject);
	}

#else

	Vector3 vScale;

#endif
}

void CSceneMgr::LoadRes()
{
	LoadSound();

	Ptr<CTexture> piBow = CResMgr::GetInst()->Load<CTexture>(L"BOW_IMG", L"Texture\\UI\\Items\\Weapons\\01_BOW.png");
	Ptr<CTexture> piSword = CResMgr::GetInst()->Load<CTexture>(L"SWORD_IMG", L"Texture\\UI\\Items\\Weapons\\02_Sword.png");
	Ptr<CTexture> piHealPotion = CResMgr::GetInst()->Load<CTexture>(L"HP_POTION_IMG", L"Texture\\UI\\Items\\Resources\\15_Heal_potion.png");

	//==========================
	// Conversation Box
	//==========================
	Ptr<CTexture> pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"UiBoard", L"Texture\\UIboard.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest1(1)", L"Texture\\Quest\\npc1_quest1(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest1(2)", L"Texture\\Quest\\npc1_quest1(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest1(3)", L"Texture\\Quest\\npc1_quest1(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest2(1)", L"Texture\\Quest\\npc1_quest2(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest2(2)", L"Texture\\Quest\\npc1_quest2(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_quest2(3)", L"Texture\\Quest\\npc1_quest2(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_done", L"Texture\\Quest\\npc1_done.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc1_why", L"Texture\\Quest\\npc1_why.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest1(1)", L"Texture\\Quest\\npc2_quest1(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest1(2)", L"Texture\\Quest\\npc2_quest1(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest1(3)", L"Texture\\Quest\\npc2_quest1(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest2(1)", L"Texture\\Quest\\npc2_quest2(1).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest2(2)", L"Texture\\Quest\\npc2_quest2(2).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_quest2(3)", L"Texture\\Quest\\npc2_quest2(3).png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_done", L"Texture\\Quest\\npc2_done.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc2_why", L"Texture\\Quest\\npc2_why.png");
	pUiBoard = CResMgr::GetInst()->Load<CTexture>(L"npc3_start", L"Texture\\Quest\\npc3_start.png");

	//===========================
	// number texture
	//==========================
	Ptr<CTexture> pNumber = CResMgr::GetInst()->Load<CTexture>(L"0", L"Texture\\Number\\0.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"1", L"Texture\\Number\\1.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"2", L"Texture\\Number\\2.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"3", L"Texture\\Number\\3.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"4", L"Texture\\Number\\4.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"5", L"Texture\\Number\\5.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"6", L"Texture\\Number\\6.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"7", L"Texture\\Number\\7.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"8", L"Texture\\Number\\8.png");
	pNumber = CResMgr::GetInst()->Load<CTexture>(L"9", L"Texture\\Number\\9.png");

	//===========================
	// Button texture
	//==========================
	Ptr<CTexture> pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BRANCH", L"Texture\\ItemButton\\BranchTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"APPLE", L"Texture\\ItemButton\\AppleTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_STAMINA", L"Texture\\ItemButton\\BottleStaminaTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_DASH", L"Texture\\ItemButton\\BottleDashTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"STEAK", L"Texture\\ItemButton\\MeatTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"MONEYBAG", L"Texture\\ItemButton\\MoneyTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"CARROT", L"Texture\\ItemButton\\CarrotTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"EMPTY", L"Texture\\ItemButton\\EmptyTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"SWORD", L"Texture\\ItemButton\\SwordTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BASIC_SWORD", L"Texture\\ItemButton\\BasicSwordTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"AX", L"Texture\\ItemButton\\AxTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"WALLET", L"Texture\\ItemButton\\WalletTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"EXIT", L"Texture\\ItemButton\\ExitStoreTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"LACK_MONEY", L"Texture\\ItemButton\\LackMoneyTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_EMPTY", L"Texture\\ItemButton\\BottleEmptyTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_CARROT", L"Texture\\ItemButton\\BottleCarrotTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"BOTTLE_STRAWBERRY", L"Texture\\ItemButton\\BottleStrawberryTex.png");
	pButtonTex = CResMgr::GetInst()->Load<CTexture>(L"TRASH", L"Texture\\ItemButton\\TrashTex.png");

	//===========================
	// QuestBox texture
	//==========================
	Ptr<CTexture> pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"QuestBase", L"Texture\\Quest\\QuestBase.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"QuestBase", L"Texture\\Quest\\QuestBase_2.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"MonsterKill", L"Texture\\Quest\\MonsterKill.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"MonsterKill_Complete", L"Texture\\Quest\\MonsterKill_Complete.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"GetItem", L"Texture\\Quest\\GetItem.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"GetItem_Complete", L"Texture\\Quest\\GetItem_Complete.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"BuyPotion", L"Texture\\Quest\\BuyPotion.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"BuyPotion_Complete", L"Texture\\Quest\\BuyPotion_Complete.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"BuyWeapone", L"Texture\\Quest\\BuyWeapone.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"BuyWeapone_Complete", L"Texture\\Quest\\BuyWeapone_Complete.png");
	
	//===========================
	// ExplainBox texture
	//==========================
	Ptr<CTexture> pExplain = CResMgr::GetInst()->Load<CTexture>(L"ExplainSword", L"Texture\\ItemButton\\ExplainSwordTex.png");
	pExplain = CResMgr::GetInst()->Load<CTexture>(L"ExplainBasicSword", L"Texture\\ItemButton\\ExplainBasicSwordTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainAx", L"Texture\\ItemButton\\ExplainAxTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainApple", L"Texture\\ItemButton\\ExplainAppleTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainBDash", L"Texture\\ItemButton\\ExplainBDashTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainBStamina", L"Texture\\ItemButton\\ExplainBStaminaTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainCarrot", L"Texture\\ItemButton\\ExplainCarrotTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainBranch", L"Texture\\ItemButton\\ExplainBranchTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainMeat", L"Texture\\ItemButton\\ExplainMeatTex.png");
	pQuestBox = CResMgr::GetInst()->Load<CTexture>(L"ExplainMoneyBag", L"Texture\\ItemButton\\ExplainMoneyBagTex.png");

}

void CSceneMgr::CreateNpc(CTerrain* _terrain)
{

	// Npc_1
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_4@Idle.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);
	CGameObject* pNpcObject = nullptr;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_1");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(5500.f, _terrain->GetHeight(5500.f, 3200.f, true) * 2 /*240.f*/, 3200.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_1");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pNpcObject->MeshRender()->SetDynamicShadow(true);
 	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false);

	// Idle
	CNpcScript* pNpcScript = pNpcObject->GetScript<CNpcScript>();
	pNpcScript->SetAnimationData(pMeshData->GetMesh());

	// Talk
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_4@Talking.fbx", FBX_TYPE::NPC);
	pNpcScript->SetAnimationData(pMeshData->GetMesh());
	
	//--------------------------
	// Npc_2
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_male_1@Dance.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);

	pNpcObject = new CGameObject;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_2");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(1600.f, _terrain->GetHeight(1600.f, 1500.f, true) * 2 /*240.f*/, 1500.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_2");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pNpcObject->MeshRender()->SetDynamicShadow(true);
	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false);

	// Idle
	pNpcScript = pNpcObject->GetScript<CNpcScript>();
	pNpcScript->SetAnimationData(pMeshData->GetMesh());

	// Talk
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_male_1@Talking.fbx", FBX_TYPE::NPC);
	pNpcScript->SetAnimationData(pMeshData->GetMesh());

	//--------------------------
	// Npc_3(store)
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_male_3@SittingIdle.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);

	pNpcObject = new CGameObject;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_3");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(1600.f, _terrain->GetHeight(1600.f, 4200.f, true) * 2 /*240.f*/, 4200.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_3");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pNpcObject->MeshRender()->SetDynamicShadow(true);
	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false); 

	// Idle
	pNpcScript = pNpcObject->GetScript<CNpcScript>();
	pNpcScript->SetAnimationData(pMeshData->GetMesh());

	// Talk
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_male_3@Talking.fbx", FBX_TYPE::NPC);
	pNpcScript->SetAnimationData(pMeshData->GetMesh());
	pNpcScript->init(UI_TYPE::PUBLIC_SHOP_UI);

	//--------------------------
	// Npc_4(alchemy)
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_2@Idle.fbx", FBX_TYPE::NPC);
	//Ptr<CMeshData> pAniData = CResMgr::GetInst()->LoadFBX(L"FBX\\Animation\\itempack@spraying.fbx", FBX_TYPE::ANI);

	pNpcObject = new CGameObject;
	pNpcObject = pMeshData->Instantiate();
	pNpcObject->SetName(L"Npc_4");
	pNpcObject->FrustumCheck(false);
	pNpcObject->Transform()->SetLocalPos(Vector3(2300.f, _terrain->GetHeight(2300.f, 4200.f, true) * 2 /*240.f*/, 4200.f));
	pNpcObject->Transform()->SetLocalScale(Vector3(1.5f, 1.5f, 1.5f));//(1.0f, 1.0f, 1.0f));
	pNpcObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
	pNpcObject->AddComponent(new CCollider);
	pNpcObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Npc_4");
	pNpcObject->Collider()->SetBoundingBox(BoundingBox(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pNpcObject->Collider()->SetBoundingSphere(BoundingSphere(pNpcObject->Transform()->GetLocalPos(), pNpcObject->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pNpcObject->MeshRender()->SetDynamicShadow(true);
	pNpcObject->AddComponent(new CNpcScript);
	m_pCurScene->AddGameObject(L"Npc", pNpcObject, false);

	// Idle
	pNpcScript = pNpcObject->GetScript<CNpcScript>();
	pNpcScript->SetAnimationData(pMeshData->GetMesh());

	// Talk
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\NPC\\common_people_female_2@Talking.fbx", FBX_TYPE::NPC);
	pNpcScript->SetAnimationData(pMeshData->GetMesh());
	pNpcScript->init(UI_TYPE::ALCHEMY_SHOP_UI);
}

void CSceneMgr::LoadSound()
{
	for (int i = 0; i < 4; i++)
	{
		SoundMgr* temp = new SoundMgr;
		wstring strFullPath = CPathMgr::GetResPath();
		switch ((Sound_Type)i)
		{
		case Sound_Type::BGM:
			strFullPath += L"Sound\\background.wav";
			break;
		case Sound_Type::HIT:
			strFullPath += L"Sound\\hit.wav";
			break;
		case Sound_Type::GET_COIN:
			strFullPath += L"Sound\\getcoin.wav";
			break;
		case Sound_Type::SET_COIN:
			strFullPath += L"Sound\\spendcoin.wav";
			break;
		default:
			break;
		}
		temp->Load(strFullPath, (Sound_Type)i);
		g_SoundList.emplace((Sound_Type)i, temp);
	}
}

void CSceneMgr::init()
{
	// =================
	// 필요한 리소스 로딩
	// =================
	// Texture 로드
	


	LoadRes();
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\Health.png");
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>(L"Explosion", L"Texture\\Explosion\\Explosion80.png");
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>(L"Black", L"Texture\\asd.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>(L"Sky02", L"Texture\\Skybox\\Sky02.jpg");
	// UI
	Ptr<CTexture> pUiHug = CResMgr::GetInst()->Load<CTexture>(L"UiHug", L"Texture\\hug.png");
	Ptr<CTexture> pUiTemper = CResMgr::GetInst()->Load<CTexture>(L"UiTemper", L"Texture\\temper.png");
	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_03.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga");

	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex");
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");



	//==========================
	// UAV 용 Texture 생성
	//==========================
	Ptr<CTexture> pTestUAVTexture = CResMgr::GetInst()->CreateTexture(L"UAVTexture", 1024, 1024
		, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
		, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());
	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());

	// ===============
	// Test Scene 생성
	// ===============
	m_pCurScene = new CScene;
	m_pCurScene->SetName(L"Test Scene");

	// ===============
	// Layer 이름 지정
	// ===============
	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	m_pCurScene->GetLayer(2)->SetName(L"Monster");
	m_pCurScene->GetLayer(3)->SetName(L"Map");
	m_pCurScene->GetLayer(4)->SetName(L"Npc");
	m_pCurScene->GetLayer(5)->SetName(L"Bullet");
	m_pCurScene->GetLayer(6)->SetName(L"Item");
	m_pCurScene->GetLayer(7)->SetName(L"Terrain");	// 카메라 순서 때문에 오류나서 일단 터레인 레이어 추가함
	m_pCurScene->GetLayer(30)->SetName(L"UI");
	m_pCurScene->GetLayer(31)->SetName(L"PUI");

	CGameObject* pObject = nullptr;
	// =============
    // FBX 파일 로드
    // =============
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nIdle1.fbx", FBX_TYPE::PLAYER);
	pMeshData->Save(pMeshData->GetPath());

	CGameObject* pPlayerObj = nullptr;
	pPlayerObj = pMeshData->Instantiate();
	pPlayerObj->SetName(L"PlayerMale");
	pPlayerObj->FrustumCheck(false);
	pPlayerObj->Transform()->SetLocalPos(Vector3(0.f, 140.f, 0.f));
	pPlayerObj->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
	pPlayerObj->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
	pPlayerObj->AddComponent(new CCollider);
	pPlayerObj->AddComponent(new CQuest);
	pPlayerObj->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"PlayerMale@nWalk_F");
	pPlayerObj->Collider()->SetBoundingBox(BoundingBox(pPlayerObj->Transform()->GetLocalPos(), pPlayerObj->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	pPlayerObj->Collider()->SetBoundingSphere(BoundingSphere(pPlayerObj->Transform()->GetLocalPos(), pPlayerObj->MeshRender()->GetMesh()->GetBoundingSphereRadius() / 2.f));
	pPlayerObj->MeshRender()->SetDynamicShadow(true);
	

	// Script 설정
	pPlayerObj->AddComponent(new CPlayerScript);
	CPlayerScript* playerScript = pPlayerObj->GetScript<CPlayerScript>();
	//playerScript->Init();
	
	// Animaition Data 넘겨주기
	// Idle
	playerScript->SetAnimationData(pMeshData->GetMesh());
	g_netMgr.SetAniData(pMeshData->GetMesh());
	// walk_F
	Ptr<CMeshData> pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nWalk_F.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());
	// walk_d
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nWalk_B.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());
	// run
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@nRun_F.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());
	// ATTACK
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@Attack1.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());
	// Damage
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@Damage_Strong.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());
	// PIck_UP
	pMeshDataKey = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale@PickUp.fbx", FBX_TYPE::PLAYER);
	playerScript->SetAnimationData(pMeshDataKey->GetMesh());
	g_netMgr.SetAniData(pMeshDataKey->GetMesh());
	m_pCurScene->AddGameObject(L"Player", pPlayerObj, false);


	// =============
	// 무기 
	// =============
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\PlayerMale_Weapon_Sword.fbx", FBX_TYPE::PLAYER);
	//pMeshData->Save(pMeshData->GetPath());

	CGameObject* pSword = nullptr;
	pSword = pMeshData->Instantiate();
	pSword->SetName(L"sword");
	pSword->FrustumCheck(false);
	pSword->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));//(1.0f, 1.0f, 1.0f));
	//pSword->Transform()->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
	pSword->AddComponent(new CCollider);
	pSword->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"PlayerMale_Weapon_Sword");
	//pSword->Collider()->SetBoundingBox(BoundingBox(pSword->Transform()->GetLocalPos(), pSword->MeshRender()->GetMesh()->GetBoundingBoxExtents()));
	//pSword->Collider()->SetBoundingSphere(BoundingSphere(pSword->Transform()->GetLocalPos(), pSword->MeshRender()->GetMesh()->GetBoundingSphereRadius()));
	pSword->MeshRender()->SetDynamicShadow(true);
	// Script 설정
	pSword->AddComponent(new CSwordScript);
	CSwordScript* SwordScript = pSword->GetScript<CSwordScript>();
	SwordScript->SetBoneFinalMat(pPlayerObj->Animator3D()->GetSwordFinalBoneMat());

	// 무기 meshdata 추가
	SwordScript->SetWeaponeData(pMeshData);
	// 2번째 무기
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\sword_2.fbx", FBX_TYPE::PLAYER);
	SwordScript->SetWeaponeData(pMeshData);
	// 도끼
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Player\\Ax.fbx", FBX_TYPE::PLAYER);
	SwordScript->SetWeaponeData(pMeshData);

	m_pCurScene->AddGameObject(L"Player", pSword, false);
	pPlayerObj->AddChild(pSword);

	
	// ==================
	//	item 객체 생성
	// ==================
	//CGameObject* pItem = new CGameObject;
	//pItem->SetName("")

	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Bottle_03.fbx", FBX_TYPE::ITEM)); // 물약1
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Bottle_04.fbx", FBX_TYPE::ITEM));	// 물약2
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Steak_02.fbx", FBX_TYPE::ITEM));	// 고기
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\MoneyBag.fbx", FBX_TYPE::ITEM));	// 돈
	CItemMgr::GetInst()->SetItemData(CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Carrot.fbx", FBX_TYPE::ITEM));	// 당근
	
	// ==================
	// Camera Object 생성
	// ==================
	// Main Camera
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(30, false);
	//pMainCam->Transform()->SetLocalPos(Vector3(0.f, 600.f, -500.f));
	//pMainCam->Transform()->SetLocalRot(Vector3(XM_PI / 6 /*0.f*/, 0.0f, 0.f));
	//vector<CToolCamScript*> camScript = (CToolCamScript*)(pMainCam->GetScripts())
	////camScript[0]->
	//pMainCam->GetScripts()[0]->SetPlayer();
	CToolCamScript* camScript = pMainCam->GetScript<CToolCamScript>();
	camScript->SetPlayer(pPlayerObj);
	// pObject->SetCam(pMainCam);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pMainCam);


	// UI Camera
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UiCam");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetFar(100.f);
	pUICam->Camera()->SetLayerCheck(30, true);
	pUICam->Camera()->SetLayerCheck(31, true);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pUICam);
	pUICam->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pUICam->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);
	CreateTargetUI(pUICam);
	//pPlayerObj->GetScript<CPlayerScript>()->GetUIObj()->StaticUI()->SetCameraProj(pUICam->Camera());


	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight);
	pObject->Light()->SetLightPos(Vector3(0.f, 1000.f, 1000.f));
	pObject->Light()->SetLightType(LIGHT_TYPE::DIR);
	pObject->Light()->SetDiffuseColor(Vector3(1.f, 1.f, 1.f));
	pObject->Light()->SetSpecular(Vector3(0.1f, 0.1f, 0.1f));
	pObject->Light()->SetAmbient(Vector3(0.3f, 0.3f, 0.3f));
	pObject->Light()->SetLightDir(Vector3(-1.f, -1.f, -1.f));
	pObject->Light()->SetLightRange(1000.f);
	pObject->Transform()->SetLocalPos(Vector3(5000.f, 1000.f, 1000.f));
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// monster
	//idle
	Ptr<CMeshData> pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@idle.fbx", FBX_TYPE::MONSTER);
	//walk
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@walking.fbx", FBX_TYPE::MONSTER);
	//dead
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@die.fbx", FBX_TYPE::MONSTER);
	//attack
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@attack2.fbx", FBX_TYPE::MONSTER);
	//damage
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\monster3@damage.fbx", FBX_TYPE::MONSTER);

	//idle
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@idle.fbx", FBX_TYPE::MONSTER);
	//walk
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@Walk Forward With Root Motion.fbx", FBX_TYPE::MONSTER);
	//dead
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@die.fbx", FBX_TYPE::MONSTER);
	//attack
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@meleeattack01.fbx", FBX_TYPE::MONSTER);
	//damage
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\TreantGuard@Damage.fbx", FBX_TYPE::MONSTER);
	
	// boss
	//idle
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Idle.fbx", FBX_TYPE::MONSTER);
	//walk
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Slither Forward Fast WO Root.fbx", FBX_TYPE::MONSTER);
	//dead
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Die.fbx", FBX_TYPE::MONSTER);
	//attack
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Bite Attack.fbx", FBX_TYPE::MONSTER);
	//damage
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Defend.fbx", FBX_TYPE::MONSTER);
	//roar
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Roar.fbx", FBX_TYPE::MONSTER);
	//left
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Claw Attack Left.fbx", FBX_TYPE::MONSTER);
	//right
	pMonsterMadt = CResMgr::GetInst()->LoadFBX(L"FBX\\Monster\\Polygonal Alien Serpent@Claw Attack Right.fbx", FBX_TYPE::MONSTER);

	// ====================
	// Skybox 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());
	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	// ====================
	// Grid 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"Grid");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CGridScript);
	// Transform 설정
	pObject->Transform()->SetLocalScale(Vector3(100000.f, 100000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vector3(XM_PI / 2.f, 0.f, 0.f));
	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GridMtrl"));
	// Script 설정	
	pObject->GetScript<CGridScript>()->SetToolCamera(pMainCam);
	pObject->GetScript<CGridScript>()->SetGridColor(Vector3(0.8f, 0.2f, 0.2f));
	// AddGameObject
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);


	// Terrain
	CGameObject* pTerrainObject = new CGameObject;
	pTerrainObject->SetName(L"Terrain");
	pTerrainObject->AddComponent(new CTransform);
	pTerrainObject->AddComponent(new CMeshRender);
	pTerrainObject->AddComponent(new CTerrain);
	pTerrainObject->FrustumCheck(false);
	pTerrainObject->Transform()->SetLocalPos(Vector3(0.f, 470.f, 0.f));
	pTerrainObject->Transform()->SetLocalScale(Vector3(300.f , 6000.f, 300.f)); // 2배함
	pTerrainObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
	pTerrainObject->Terrain()->init();
	m_pCurScene->FindLayer(L"Terrain")->AddGameObject(pTerrainObject);
	pPlayerObj->GetScript<CPlayerScript>()->SetTerrain(pTerrainObject->Terrain());
	g_netMgr.SetObj(pPlayerObj);
	
	CreateNpc(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());
	CreateNewMap(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());
	CItemMgr::GetInst()->SetTerrain(pPlayerObj->GetScript<CPlayerScript>()->GetTerrain());

	

//	// ====================
//// Particle Object 생성
//// ====================
//	pObject = new CGameObject;
//	pObject->SetName(L"Particle");
//	pObject->AddComponent(new CTransform);
//	pObject->AddComponent(new CParticleSystem);
//
//	pObject->FrustumCheck(false);
//	pObject->Transform()->SetLocalPos(Vec3(500.f, 0.f, 0.f));
//
//	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// ===============
	// particle
	// ===============
	pObject = new CGameObject;
	pObject->SetName(L"Rain");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleSystem);

	pObject->FrustumCheck(false);
	//pObject->Particlesystem()->SetFrequency(2.f);
	pObject->Particlesystem()->SetType(true);
	//pObject->Particlesystem()->SetMaxParticle(512);
	//pObject->GetScript<CParticleScript>()->SetLifeTime(pObject->Particlesystem()->GetMaxLifeTime());
	
	Vector3 particlePos = Vector3(3000.f, 0.f, 200.f);
	{
		int z = (int)(particlePos.z / pObject->Transform()->GetLocalScale().z);

		float fHeight = pPlayerObj->GetScript<CPlayerScript>()->GetTerrain()->GetHeight(particlePos.x, particlePos.z, ((z % 2) != 0)) * 2.f;

		if (particlePos.y != fHeight)
			particlePos.y = fHeight + 2000.f;
	}
	pObject->Transform()->SetLocalPos(particlePos);
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	//=============
	// Line Mesh
	//=============

	//VTX v;
	//vector<VTX> vecVTX;
	//vector<UINT> vecIdx;
	//Ptr<CMesh> pMesh = nullptr;
	//pMesh = new CMesh;

	//v.vPos = Vector3(-0.5f, 0.5f, 0.f);
	//v.vColor = Vector4(0.8f, 0.7f, 0.6f, 1.f);
	//v.vUV = Vector2(0.f, 0.f);
	//v.vNormal = Vector3(0.f, 0.f, -1.f);
	//v.vTangent = Vector3(1.f, 0.f, 0.f);
	//v.vBinormal = Vector3(0.f, 1.f, 0.f);
	//vecVTX.push_back(v);

	//v.vPos = Vector3(0.5f, 0.5f, 0.f);
	//vecVTX.push_back(v);

	//v.vPos = Vector3(0.5f, -0.5f, 0.f);
	//vecVTX.push_back(v);

	//v.vPos = Vector3(-0.5f, -0.5f, 0.f);
	//vecVTX.push_back(v);

	//vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	//vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	//pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
	//	, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	//CGameObject* pLineMesh = new CGameObject;
	//pLineMesh->SetName(L"Line");
	//pLineMesh->AddComponent(new CTransform);
	//pLineMesh->AddComponent(new CMeshRender);
	//pLineMesh->FrustumCheck(false);
	//pLineMesh->Transform()->SetLocalPos(particlePos);
	//pLineMesh->Transform()->SetLocalScale(Vector3(100.f, 100.f, 100.f)); // 2배함

	//// MeshRender 설정
	//pLineMesh->MeshRender()->SetMesh(pMesh);
	//pLineMesh->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl"));
	////pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTestUAVTexture.GetPointer());
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pLineMesh);
	//// ====================
	//// Compute Shader Test
	//// ====================
	//int i = 1;
	//Ptr<CMaterial> pCSMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"CSTestMtrl");
	//pCSMtrl->SetData(SHADER_PARAM::INT_0, &i);
	//CDevice::GetInst()->SetUAVToRegister_CS(pTestUAVTexture.GetPointer(), UAV_REGISTER::u0);
	//pCSMtrl->Dispatch(1, 1024, 1); // --> 컴퓨트 쉐이더 수행	


	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Map");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Player", L"Npc");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Monster");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Map");
	//CCollisionMgr::GetInst()->CheckCollisionLayer(L"Bullet", L"Item");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"Item", L"Player");
	CCollisionMgr::GetInst()->CheckCollisionLayer(L"UI", L"PUI");



	m_pCurScene->awake();
	m_pCurScene->start();


}

void CSceneMgr::update() 
{

	m_pCurScene->update();
	g_SoundList.find(Sound_Type::BGM)->second->Play(0);
	m_pCurScene->lateupdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->finalupdate();

	// 충돌 처리
	CItemMgr::GetInst()->update();
	CCollisionMgr::GetInst()->update();
}

void CSceneMgr::update_tool()
{
	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->finalupdate();
}

void CSceneMgr::FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				_vecFindObj.push_back(vecObject[j]);
			}
		}
	}
}

bool Compare(CGameObject* _pLeft, CGameObject* _pRight)
{
	return (_pLeft->Transform()->GetWorldPos().z < _pRight->Transform()->GetWorldPos().z);
}


void CSceneMgr::CreateNewMap(CTerrain* _terrain)
{
	// Tree
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
	//pMeshData->Save(pMeshData->GetPath());
	CGameObject* pMapObject = nullptr;
	pMapObject = pMeshData->Instantiate();
	pMapObject->SetName(L"Tree");
	pMapObject->FrustumCheck(false);
	int z = (int)(3000.f / 60.f);
	bool bReverseQuad = ((z % 2) != 0);
	float mapY = _terrain->GetHeight(1000.f, 3000, bReverseQuad);
	pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 3000.f));
	pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
	pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
	pMapObject->AddComponent(new CCollider);
	pMapObject->MeshRender()->SetDynamicShadow(true);
	pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
	pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
	pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
	m_pCurScene->AddGameObject(L"Map", pMapObject, false);

	// 나무
	{
		// 동그란 나무
		for (int i = 0; i < 150; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//	pMeshData->Save(pMeshData->GetPath());

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree" + i);
			pMapObject->FrustumCheck(true);
			pMapObject->AddComponent(new CTreeScript);

			float randomX = uid(dre);
			float randomZ = uid(dre);

			z = (int)(randomZ / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2, randomZ));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * (120.f / 3.f)));
			pMapObject->MeshRender()->SetDynamicShadow(true);

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			// 나무 충돌 스크립트
			CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
			//-------item load---------
			// 나뭇가지
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			// 사과
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			//-----------
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pTreeScript->SetTerrain(_terrain);	// Terrain
			//-----------------------------------		

		}

		// 죽은 나무
		for (int i = 0; i < 100; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_05.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//	pMeshData->Save(pMeshData->GetPath());

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree" + i);
			pMapObject->FrustumCheck(true);
			pMapObject->AddComponent(new CTreeScript);

			float randomX = uid(dre);
			float randomZ = uid(dre);

			z = (int)(randomZ / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2, randomZ));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * (120.f / 3.f)));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
			//-------item load---------
			// 나뭇가지
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			// 사과
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			//-----------
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pTreeScript->SetTerrain(_terrain);	// Terrain
		}

		// 뾰족한 나무
		for (int i = 0; i < 100; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//	pMeshData->Save(pMeshData->GetPath());

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree" + i);
			pMapObject->FrustumCheck(true);
			pMapObject->AddComponent(new CTreeScript);

			float randomX = uid(dre);
			float randomZ = uid(dre);

			z = (int)(randomZ / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(randomX, randomZ, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(randomX, mapY * 2, randomZ));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"JoshuaTree1");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * (120.f / 3.f)));
			pMapObject->MeshRender()->SetDynamicShadow(true);

			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
			//-------item load---------
			// 나뭇가지
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			// 사과
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
			pTreeScript->SetItemData(pMeshData);
			//-----------
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			pTreeScript->SetStumpData(pMeshData);
			pTreeScript->SetTerrain(_terrain);	// Terrain
		}
	}
	{}

	// 집1
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_03.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House1");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1000.f, 3000, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 3000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House1");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집1 tile
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tile8.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tile");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1600.f, 3000.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1600.f, mapY * 2, 3000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 200.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 4, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tile5");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 3300.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 3400.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 3300.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 3400.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1720.f, 2300.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1720.f, mapY * 2, 2300.f + (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}{}
	// 집2
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House2");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1000.f, 1630.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 1630.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House2");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 tile
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tile8.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tile");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1600.f, 1700.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1600.f, mapY * 2, 1700.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 250.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 4, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tile5");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 fence1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 2100.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 2100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집2 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 1150.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 1150.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 1050.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 1050.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 2100.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 2100.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 2; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1720.f, 1260.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1720.f, mapY * 2, 1260.f + (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}{}
	// 왼쪽 상점
	{
		// 나무
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());

		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1300.f, 3600.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1300.f, mapY * 2, 3600.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 100.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 천막
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Roof_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Roof");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1300.f, 4100.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1300.f, mapY * 2, 4100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Roof");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 테이블
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Table_03.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Table");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1300.f, 4100.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1300.f, mapY * 2, 4100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Table");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 수레
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Cart_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Cart");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1600.f, 4400.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1600.f, mapY * 2, 4400.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 150.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Cart");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}
	// 집3
	{
		// 집3
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House_01");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1000.f, 5200.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 5200.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House_01");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 4720.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 4720.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(1100.f, 5680.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 5680.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 3; ++i) {
			// 집2 fence3
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 4610.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 4610.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence4
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1300.f, 5790.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1300.f + (210.f * i), mapY * 2, 5790.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 2; ++i) {
			// 집3 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1720.f, 4820.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1720.f, mapY * 2, 4820.f + (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}

	// --------------------- 오른쪽 --------------------- 
	{}
	// 집4
	{
		// 집3
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\House_06.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"House2");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(6000.f, 2000.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(6000.f, mapY * 2, 2000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"House2");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 tile
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tile8.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tile");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5500.f, 1820.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5500.f, mapY * 2, 1820.f));
		pMapObject->Transform()->SetLocalScale(Vector3(150.f, 250.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 4, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tile5");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5700.f, 1530.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5700.f, mapY * 2, 1530.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 집3 fence2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5700.f, 2850.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5700.f, mapY * 2, 2850.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 2; ++i) {
			// 집3 fence3
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(5490.f, 1430.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(5490.f - (210.f * i), mapY * 2, 1430.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 2; ++i) {
			// 집3 fence3
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(5490.f, 2950.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(5490.f - (210.f * i), mapY * 2, 2950.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		for (int i = 0; i < 3; ++i) {
			// 집2 fence2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Fence_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Fence");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(5280.f, 2750.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(5280.f, mapY * 2, 2750.f - (210 * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Shelving_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Fence");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5800.f, 2850.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5800.f, mapY * 2, 3100.f));
		pMapObject->Transform()->SetLocalScale(Vector3(400.f, 300.f, 300.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Fence");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}{}
	// 집5
	{

	}
	{}
	// 오른쪽 상점
	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Roof_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Roof");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5800.f, 3750.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5800.f, mapY * 2, 3750.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Roof");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 테이블
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Table_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Table");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(5800.f, 3750.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(5800.f, mapY * 2, 3750.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Table");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}
	// 가운데 나무, 돌
	{
		// 가운데 나무
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);
		pMapObject->AddComponent(new CTreeScript);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3000.f, 3000.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3000.f, mapY * 2, 3000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(200.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		// 나무 충돌 스크립트
		// 나무 충돌 스크립트
		CTreeScript* pTreeScript = pMapObject->GetScript<CTreeScript>();
		//-------item load---------
		// 나뭇가지
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Branch.fbx", FBX_TYPE::ITEM);
		pTreeScript->SetItemData(pMeshData);
		// 사과
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Item\\Apple.fbx", FBX_TYPE::ITEM);
		pTreeScript->SetItemData(pMeshData);
		//-----------
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
		pTreeScript->SetStumpData(pMeshData);
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Stump_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
		pTreeScript->SetStumpData(pMeshData);
		pTreeScript->SetTerrain(_terrain);	// Terrain
		//-----------------------------------

		// 가운데 나무2
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3100.f, 2700.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3100.f, mapY * 2, 2700.f));
		pMapObject->Transform()->SetLocalScale(Vector3(210.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 가운데 나무3
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_05.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Tree");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3000.f, 2400.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3000.f, mapY * 2, 2400.f));
		pMapObject->Transform()->SetLocalScale(Vector3(500.f, 350.f, 400.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 가운데 돌1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Rock");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3300.f, 2600.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3300.f, mapY * 2, 2600.f));
		pMapObject->Transform()->SetLocalScale(Vector3(400.f, 550.f, 550.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 가운데 돌1
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_07.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Rock");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3200.f, 2900.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3200.f, mapY * 2, 2900.f));
		pMapObject->Transform()->SetLocalScale(Vector3(450., 450.f, 450.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
	}{}
	// 가운데 우물
	{
		// 우물
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Well.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Well");
		pMapObject->FrustumCheck(false);

		z = (int)(3000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3200.f, 3700.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3200.f, mapY * 2, 3700.f));
		pMapObject->Transform()->SetLocalScale(Vector3(300.f, 300.f, 300.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Well");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		// 테이블
		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Table_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Table_01");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(3900.f, 2800.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(3900.f, mapY * 2, 2800.f - (900.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 150.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Table_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 벤치1
		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Bench_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Bench_02");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(3900.f, 2950.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(3900.f, mapY * 2, 2950.f - (300.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 150.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Bench_02");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 벤치2
		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Bench_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Bench_02");
			pMapObject->FrustumCheck(false);

			z = (int)(3000.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(3900.f, 2050.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(3900.f, mapY * 2, 2050.f - (300.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 150.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Bench_02");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}
	
	//위
	// 교회
	{
		// church
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Church.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Market_Roof");
		pMapObject->FrustumCheck(false);

		z = (int)(7000.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(3000.f, 7000.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(3000.f, mapY * 2, 7000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"church");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		
		// 교회 옆 작은 정원
		{
			// 나무1
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree");
			pMapObject->FrustumCheck(false);
			int z = (int)(7000.f / 60.f);
			bool bReverseQuad = ((z % 2) != 0);
			float mapY = _terrain->GetHeight(1000.f, 7000.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(1000.f, mapY * 2, 7000.f));
			pMapObject->Transform()->SetLocalScale(Vector3(60.f, 60.f, 60.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			// 나무2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree");
			pMapObject->FrustumCheck(false);
			z = (int)(6500.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(600.f, 6500.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(600.f, mapY * 2, 6500.f));
			pMapObject->Transform()->SetLocalScale(Vector3(70.f, 70.f, 70.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			// 나무2
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Tree");
			pMapObject->FrustumCheck(false);
			z = (int)(6800.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(700.f, 6800.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(700.f, mapY * 2, 6800.f));
			pMapObject->Transform()->SetLocalScale(Vector3(70.f, 70.f, 70.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			// 큰돌
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\BigRock_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"BigRock_01");
			pMapObject->FrustumCheck(false);
			z = (int)(7100.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1400.f, 7100.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(1400.f, mapY * 2, 7100.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 100.f, 100.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"BigRock_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			// 마켓
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Market_Table_03.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Market_Table_03");
			pMapObject->FrustumCheck(false);
			z = (int)(7100.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(2100.f, 7100.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(2100.f, mapY * 2, 7100.f));
			pMapObject->Transform()->SetLocalScale(Vector3(300.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Market_Table_03");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			// 나무
			for (int i = 0; i < 2; ++i) {
				pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Tree_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
				//pMeshData->Save(pMeshData->GetPath());
				pMapObject = nullptr;
				pMapObject = pMeshData->Instantiate();
				pMapObject->SetName(L"Tree");
				pMapObject->FrustumCheck(false);
				z = (int)(7300.f / 60.f);
				bReverseQuad = ((z % 2) != 0);
				mapY = _terrain->GetHeight(1600.f + (700.f * i), 7300.f, bReverseQuad);
				pMapObject->Transform()->SetLocalPos(Vector3(1600.f + (700.f * i), mapY * 2, 7300.f));
				pMapObject->Transform()->SetLocalScale(Vector3(160.f, 130.f, 160.f));//(1.0f, 1.0f, 1.0f));
				pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2,  0.0f + ((XM_PI / 2) * i) , 0.f));
				pMapObject->AddComponent(new CCollider);
				pMapObject->MeshRender()->SetDynamicShadow(true);
				pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Tree");
				pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
				pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
				m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			}

			// 물
			Ptr<CTexture> piBow = CResMgr::GetInst()->Load<CTexture>(L"Water", L"Texture\\DesertMap\\Water.png");

			pMapObject = nullptr;
			pMapObject = new CGameObject;
			pMapObject->SetName(L"Water");
			pMapObject->FrustumCheck(false);
			pMapObject->AddComponent(new CTransform);
			pMapObject->AddComponent(new CMeshRender);
			z = (int)(6500.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1100.f, 6500.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(1100.f, mapY * 2, 6500.f));
			pMapObject->Transform()->SetLocalScale(Vector3(400.f, 400.f, 150.f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
			pMapObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"WaterMtrl"));
			pMapObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, piBow.GetPointer());
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);


			for (int i = 0; i < 2; ++i) {
				// 돌
				pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_09.fbx", FBX_TYPE::NEW_DESERT_MAP);
				//pMeshData->Save(pMeshData->GetPath());
				pMapObject = new CGameObject;
				pMapObject = pMeshData->Instantiate();
				pMapObject->SetName(L"Rock");
				pMapObject->FrustumCheck(false);

				z = (int)(6300.f + (380.f * i) / 60.f);
				bReverseQuad = ((z % 2) != 0);
				mapY = _terrain->GetHeight(1000.f + (100.f * i), 6300.f + (380.f * i), bReverseQuad);

				pMapObject->Transform()->SetLocalPos(Vector3(1000.f + (100.f * i), mapY * 2, 6300.f + (380.f * i)));
				pMapObject->Transform()->SetLocalScale(Vector3(200.f, 350.f, 350.f));//(1.0f, 1.0f, 1.0f));
				pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, (XM_PI / 2) + (XM_PI * i), 0.f));
				pMapObject->AddComponent(new CCollider);
				pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
				pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
				pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
				pMapObject->MeshRender()->SetDynamicShadow(true);
				m_pCurScene->AddGameObject(L"Map", pMapObject, false);

				// 돌
				pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
				//pMeshData->Save(pMeshData->GetPath());
				pMapObject = new CGameObject;
				pMapObject = pMeshData->Instantiate();
				pMapObject->SetName(L"Rock");
				pMapObject->FrustumCheck(false);

				z = (int)(6500.f / 60.f);
				bReverseQuad = ((z % 2) != 0);
				mapY = _terrain->GetHeight(850.f + (500.f * i), 6500.f, bReverseQuad);

				pMapObject->Transform()->SetLocalPos(Vector3(850.f + (500.f * i), mapY * 2, 6500.f));
				pMapObject->Transform()->SetLocalScale(Vector3(400.f, 550.f, 550.f));//(1.0f, 1.0f, 1.0f));
				pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
				pMapObject->AddComponent(new CCollider);
				pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
				pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
				pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
				pMapObject->MeshRender()->SetDynamicShadow(true);
				m_pCurScene->AddGameObject(L"Map", pMapObject, false);

				// 돌
				pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
				//pMeshData->Save(pMeshData->GetPath());
				pMapObject = new CGameObject;
				pMapObject = pMeshData->Instantiate();
				pMapObject->SetName(L"Rock");
				pMapObject->FrustumCheck(false);

				z = (int)(6300.f + (300.f * i) / 60.f);
				bReverseQuad = ((z % 2) != 0);
				mapY = _terrain->GetHeight(900.f + (420.f * i), 6300.f + (300.f * i), bReverseQuad);

				pMapObject->Transform()->SetLocalPos(Vector3(900.f + (420.f * i), mapY * 2, 6300.f + (300.f * i)));
				pMapObject->Transform()->SetLocalScale(Vector3(400.f, 550.f, 550.f));//(1.0f, 1.0f, 1.0f));
				pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, (-XM_PI / 3) + (XM_PI * i), 0.f));
				pMapObject->AddComponent(new CCollider);
				pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
				pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
				pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
				pMapObject->MeshRender()->SetDynamicShadow(true);
				m_pCurScene->AddGameObject(L"Map", pMapObject, false);

				// 돌
				pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Rock_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
				//pMeshData->Save(pMeshData->GetPath());
				pMapObject = new CGameObject;
				pMapObject = pMeshData->Instantiate();
				pMapObject->SetName(L"Rock");
				pMapObject->FrustumCheck(false);

				z = (int)(6250.f + (400.f * i) / 60.f);
				bReverseQuad = ((z % 2) != 0);
				mapY = _terrain->GetHeight(1300.f - (450.f * i), 6250.f + (400.f * i), bReverseQuad);

				pMapObject->Transform()->SetLocalPos(Vector3(1300.f - (450.f * i), mapY * 2, 6250.f + (400.f * i)));
				pMapObject->Transform()->SetLocalScale(Vector3(400.f, 550.f, 550.f));//(1.0f, 1.0f, 1.0f));
				pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, (XM_PI / 3) + (XM_PI * i), 0.f));
				pMapObject->AddComponent(new CCollider);
				pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Rock");
				pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
				pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
				pMapObject->MeshRender()->SetDynamicShadow(true);
				m_pCurScene->AddGameObject(L"Map", pMapObject, false);
			}
			// 꽃
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Reeds_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Reeds_01");
			pMapObject->FrustumCheck(false);

			z = (int)(6350.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(1320.f, 6350.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(1320.f, mapY * 2, 6350.f));
			pMapObject->Transform()->SetLocalScale(Vector3(100.f, 150.f, 200.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Reeds_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		}
	}

	{
	}

	// 마을 펜스
	{	
		for (int i = 0; i < 21; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\FenceBig_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"FenceBig_02");
			pMapObject->FrustumCheck(true);

			z = (int)(8500.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(450.f + (290.f * i), 8500.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(450.f + (290.f * i), mapY * 2, 8500.f));
			pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"FenceBig_02");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			if (i == 20) {
				pMapObject->AddComponent(new CFenceScript);
				CFenceScript* pFenceScript = pMapObject->GetScript<CFenceScript>();
				pFenceScript->Init();
			}
		}

		for (int i = 0; i < 29; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\FenceBig_02.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"FenceBig_02");
			pMapObject->FrustumCheck(true);

			z = (int)(450.f + (290.f * i) / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(6400.f, 450.f + (290.f * i), bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(6400.f, mapY * 2, 450.f + (290.f * i)));
			pMapObject->Transform()->SetLocalScale(Vector3(130.f, 130.f, 130.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"FenceBig_02");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}

	{

	}
	//Wall
	{
		//왼쪽
		float wallX = 1400.f;
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

pMapObject = pMeshData->Instantiate();
pMapObject->SetName(L"Wall");
pMapObject->FrustumCheck(false);


z = (int)(30.f / 60.f);
bReverseQuad = ((z % 2) != 0);
mapY = _terrain->GetHeight(30.f, (wallX * i) + wallX / 2, true);

pMapObject->Transform()->SetLocalPos(Vector3(250.f, mapY * 2 - 50.f, (wallX* i) + wallX / 2));
pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 2, 0.f));
/*pMapObject->AddComponent(new CCollider);
pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/
pMapObject->MeshRender()->SetDynamicShadow(true);
m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		//오른쪽
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(false);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(30.f, (wallX * i) + wallX / 2, true);

			pMapObject->Transform()->SetLocalPos(Vector3(76500.f, mapY * 2 - 1000.f, (wallX * i) + wallX / 2));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI / 2, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 위
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(false);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight((wallX * i) + wallX / 2, 76300.f, true);

			pMapObject->Transform()->SetLocalPos(Vector3((wallX * i) + wallX / 2, mapY * 2 - 100.f, 76300.f));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}

		// 아래
		for (int i = 0; i < 55; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Desert\\Wall.fbx", FBX_TYPE::DESERT_MAP);

			pMapObject = new CGameObject;

			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Wall");
			pMapObject->FrustumCheck(false);


			z = (int)(30.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight((wallX * i) + wallX / 2, 300.f, true);

			pMapObject->Transform()->SetLocalPos(Vector3((wallX * i) + wallX / 2, mapY * 2 - 50.f, 300.f));
			pMapObject->Transform()->SetLocalScale(Vector3(310.f, 200.f, 1000.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, XM_PI, 0.f));
			/*pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Wall");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents() * pMapObject->Transform()->GetLocalScale()));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));*/
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}

	// Boss Monster
	
	{
	}

	{
		pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Mountain_03.fbx", FBX_TYPE::NEW_DESERT_MAP);
		//pMeshData->Save(pMeshData->GetPath());
		pMapObject = new CGameObject;
		pMapObject = pMeshData->Instantiate();
		pMapObject->SetName(L"Mountain");
		pMapObject->FrustumCheck(false);

		z = (int)(74500.f / 60.f);
		bReverseQuad = ((z % 2) != 0);
		mapY = _terrain->GetHeight(69000.f, 74500.f, bReverseQuad);

		pMapObject->Transform()->SetLocalPos(Vector3(69000.f, mapY * 2, 73000.f));
		pMapObject->Transform()->SetLocalScale(Vector3(100.f, 100.f, 100.f));//(1.0f, 1.0f, 1.0f));
		pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
		pMapObject->AddComponent(new CCollider);
		pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Mountain");
		pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
		pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
		pMapObject->MeshRender()->SetDynamicShadow(true);
		m_pCurScene->AddGameObject(L"Map", pMapObject, false);

		for (int i = 0; i < 2; ++i) {
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\Mountain_04.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = new CGameObject;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"Mountain");
			pMapObject->FrustumCheck(false);

			z = (int)(72800.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(67900.f + (2500.f * i), 72800.f, bReverseQuad);

			pMapObject->Transform()->SetLocalPos(Vector3(67900.f + (2500.f * i), mapY * 2, 72800.f));
			pMapObject->Transform()->SetLocalScale(Vector3(50.f, 50.f, 50.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, 0.f, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"Mountain");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			pMapObject->MeshRender()->SetDynamicShadow(true);
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			// 큰 돌
			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\BigRock_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"BigRock_01");
			pMapObject->FrustumCheck(false);
			z = (int)(72500.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(67500.f, 72500.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(67500.f, mapY * 2, 72500.f));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 3, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"BigRock_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\BigRock_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"BigRock_01");
			pMapObject->FrustumCheck(false);
			z = (int)(71500.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(67000.f, 71500.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(67000.f, mapY * 2, 71500.f));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 4, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"BigRock_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);

			pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\DesertMap\\BigRock_01.fbx", FBX_TYPE::NEW_DESERT_MAP);
			//pMeshData->Save(pMeshData->GetPath());
			pMapObject = nullptr;
			pMapObject = pMeshData->Instantiate();
			pMapObject->SetName(L"BigRock_01");
			pMapObject->FrustumCheck(false);
			z = (int)(70500.f / 60.f);
			bReverseQuad = ((z % 2) != 0);
			mapY = _terrain->GetHeight(66800.f, 70500.f, bReverseQuad);
			pMapObject->Transform()->SetLocalPos(Vector3(66800.f, mapY * 2, 70500.f));
			pMapObject->Transform()->SetLocalScale(Vector3(200.f, 200.f, 200.f));//(1.0f, 1.0f, 1.0f));
			pMapObject->Transform()->SetLocalRot(Vector3(-XM_PI / 2, -XM_PI / 5, 0.f));
			pMapObject->AddComponent(new CCollider);
			pMapObject->MeshRender()->SetDynamicShadow(true);
			pMapObject->Collider()->SetColliderType(COLLIDER_TYPE::MESH, L"BigRock_01");
			pMapObject->Collider()->SetBoundingBox(BoundingBox(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingBoxExtents(pMapObject->Transform()->GetLocalScale())));
			pMapObject->Collider()->SetBoundingSphere(BoundingSphere(pMapObject->Transform()->GetLocalPos(), pMapObject->MeshRender()->GetMesh()->GetBoundingSphereRadius() * 60.f));
			m_pCurScene->AddGameObject(L"Map", pMapObject, false);
		}
	}
}
