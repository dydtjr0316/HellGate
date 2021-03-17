#include "pch.h"
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
// #include "Animator2D.h"
// #include "Animation2D.h"
// #include "Light2D.h"
#include "Light.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

//#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"
#include "Core.h"

#include "PlayerScript.h"
#include "ToolCamScript.h"

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

void CSceneMgr::init()
{
	// 필요한 리소스 로딩
	// Texture 로드
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"TestTex", L"Texture\\cookie.png");
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	
	m_pCurScene = new CScene;
	m_pCurScene->SetName(L"Test Scene");

	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer(1)->SetName(L"Player");
	m_pCurScene->GetLayer(2)->SetName(L"Monster");

	CGameObject* pObject = nullptr;

	// ====================
	// Camera Object
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CToolCamScript);

	pObject->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pObject->Camera()->SetFar(100000.f);
	pObject->Camera()->SetLayerAllCheck();

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight);

	pObject->Light()->SetLightPos(Vector3(-100.f, 200.f, 1000.f));
	pObject->Light()->SetLightType(LIGHT_TYPE::POINT);
	pObject->Light()->SetDiffuseColor(Vector3(1.f, 0.2f, 0.2f));
	pObject->Light()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
	pObject->Light()->SetAmbient(Vector3(0.f, 0.f, 0.f));
	pObject->Light()->SetLightDir(Vector3(1.f, -1.f, 1.f));
	pObject->Light()->SetLightRange(500.f);

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// clone이 이상함 parentobj에 안 넣음

	// 2 번째 광원
	//pObject = pObject->Clone();
	//pObject->Light()->SetLightPos(Vector3(100.f, 200.f, 1000.f));
	//pObject->Light()->SetDiffuseColor(Vector3(0.2f, 0.2f, 1.f));
	//m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight);

	pObject->Light()->SetLightPos(Vector3(100.f, 200.f, 1000.f));
	pObject->Light()->SetLightType(LIGHT_TYPE::POINT);
	pObject->Light()->SetDiffuseColor(Vector3(0.2f, 0.2f, 1.f));
	pObject->Light()->SetSpecular(Vector3(0.3f, 0.3f, 0.3f));
	pObject->Light()->SetAmbient(Vector3(0.f, 0.f, 0.f));
	pObject->Light()->SetLightDir(Vector3(1.f, -1.f, 1.f));
	pObject->Light()->SetLightRange(500.f);

	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// ====================
	// Skybox 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false); // 스카이박스는 항상 그려져야 하기 때문에, 절두체 컬링 테스트자체를 시도하지 않는다.
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());

	// AddGameObject
	m_pCurScene->FindLayer(L"Default")->AddGameObject(pObject);

	// ====================
	// Player Object
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"Player Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vector3(0.f, -200.f, 1000.f));
	pObject->Transform()->SetLocalScale(Vector3(100.f, 100.f, 100.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

	// Script 설정
	pObject->AddComponent(new CPlayerScript);
	m_pCurScene->FindLayer(L"Player")->AddGameObject(pObject);


	//pObject = new CGameObject;
	//pObject->SetName(L"Player Object");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//

	//// Transform 설정
	//pObject->Transform()->SetLocalPos(Vector3(0.f, -200.f, 1000.f));
	//pObject->Transform()->SetLocalScale(Vector3(1000.f, 1000.f, 1000.f));
	//pObject->Transform()->SetLocalRot(Vector3(XM_PI / 2.f, 0.f, 0.f));

	//// MeshRender 설정
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

	//// Script 설정
	//pObject->AddComponent(new CPlayerScript);

	//// AddGameObject
	//m_pCurScene->FindLayer(L"Player")->AddGameObject(pObject);


	m_pCurScene->awake();
	m_pCurScene->start();
}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();

	// rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->finalupdate();

	// 충돌 처리
	//CCollisionMgr::GetInst()->update();
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

//void CSceneMgr::FindGameObjectByPoint(POINT _point, vector<CGameObject*>& _vecFindObj, CCamera* _pToolCam)
//{
//	CCamera* pCam = _pToolCam;
//	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
//	{
//		pCam = CRenderMgr::GetInst()->GetCamera(0);
//	}
//
//	tResolution tRes = CRenderMgr::GetInst()->GetResolution();
//	Vec3 vPickPos = Vec3((float)_point.x - (tRes.fWidth / 2.f), (tRes.fHeight / 2.f) - (float)_point.y, 0.f);
//	vPickPos *= pCam->GetScale(); 
//	vPickPos += pCam->Transform()->GetWorldPos();
//
//	for (int i = 0; i < MAX_LAYER; ++i)
//	{
//		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
//		for (size_t j = 0; j < vecObject.size(); ++j)
//		{
//			if (vecObject[j]->Transform()->IsCasting(vPickPos))
//			{
//				_vecFindObj.push_back(vecObject[j]);
//			}
//		}
//	}
//
//	sort(_vecFindObj.begin(), _vecFindObj.end(), Compare);
//}
