#include "pch.h"
#include "ToolCamScript.h"

#include "Camera.h"

CToolCamScript::CToolCamScript()
    : CScript(0)
    , m_fSpeed(200.f)
    , m_fScaleSpeed(1.f)
{
	m_pDummy = new CGameObject;
	m_pDummy->SetName(L"NpcDummy");
	m_pDummy->FrustumCheck(false);
	m_pDummy->AddComponent(new CTransform);
	
	m_pDummy->Transform()->SetLocalPos(Vector3(0.0f, 0.0f, 0.0f));
	m_pDummy->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
	m_pDummy->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));

	// 확인용
	m_pDummy->AddComponent(new CMeshRender);
	m_pDummy->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pDummy->MeshRender()->SetMaterial(pMtrl->Clone());
	//
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pDummy);

	// test용 ------------------
	m_pTest = new CGameObject;
	m_pTest->SetName(L"NpcTest");
	m_pTest->FrustumCheck(false);
	m_pTest->AddComponent(new CTransform);

	m_pTest->Transform()->SetLocalPos(Vector3(0.0f, 0.0f, 0.0f));
	m_pTest->Transform()->SetLocalScale(Vector3(30.f, 30.f, 30.f));
	m_pTest->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));

	// 확인용
	m_pTest->AddComponent(new CMeshRender);
	m_pTest->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	Ptr<CMaterial> pMtrla = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pTest->MeshRender()->SetMaterial(pMtrla->Clone());
	//
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(m_pTest);

	
}

CToolCamScript::~CToolCamScript()
{
	
}

void CToolCamScript::update()
{
 
	if (KEY_TAB(KEY_TYPE::KEY_LCTRL))
	{
		/*int i = 0;
		bCameraTag = !bCameraTag;*/

		if (m_eCamState == CAMERA_STATE::FREE_CAMERA)
			m_eCamState = CAMERA_STATE::FIXED_CAMERA;
		else 
			m_eCamState = CAMERA_STATE::FREE_CAMERA;
	}

	if (m_eCamState == CAMERA_STATE::FREE_CAMERA)
		SetFreeCamera();
	else if (m_eCamState == CAMERA_STATE::FIXED_CAMERA)
		SetPlayerFixedCamera();
	else if (m_eCamState == CAMERA_STATE::NPC_CAMERA)
		SetNpcCamera();
}

void CToolCamScript::SetFreeCamera()
{

	Vector3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		Vector3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vFront * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		Vector3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vBack * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{
		Vector3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLeft * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		Vector3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRight * fSpeed * DT;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_NUM1))
	{
		fScale -= m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_NUM2))
	{
		fScale += m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vector3 vRot = Transform()->GetLocalRot();

		vRot.x -= vDrag.y * DT * 3.f;
		vRot.y += vDrag.x * DT * 1.5f;

		Transform()->SetLocalRot(vRot);
	}

	Transform()->SetLocalPos(vPos);

}

void CToolCamScript::SetPlayerFixedCamera()
{
    // 공통
    Vector3 vPos = Transform()->GetLocalPos();
    CTransform* vPlayerPos = g_Object.find(g_myid)->second->Transform();

    // 공통
    float fDistance = 500.f; //200.f;

    vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
    vPos.y = vPlayerPos->GetLocalPos().y + 450.f; // 200.f;

    Transform()->SetLocalPos(vPos);
    Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vector3(XM_PI / 8.5, XM_PI, 0.f));
}

void CToolCamScript::SetNpcCamera()
{
	// 공통
	//Vector3 vPos = Transform()->GetLocalPos();
	//CTransform* vPlayerPosa = g_Object.find(g_myid)->second->Transform();

	//// 공통
	//float fDistance = 500.f; //200.f;

	//vPos = vPlayerPosa->GetLocalPos() + (vPlayerPosa->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	//vPos.y = vPlayerPosa->GetLocalPos().y + 450.f; // 200.f;

	//Transform()->SetLocalPos(vPos);
	//Transform()->SetLocalRot(vPlayerPosa->GetLocalRot() + Vector3(XM_PI / 8.5, XM_PI, 0.f));
	//cout << "npcCamera" << endl;
	//--------------------------------------------------

	// real code
	CTransform* vPlayerTrans = g_Object.find(g_myid)->second->Transform();
	Vector3		vPlayerPos = vPlayerTrans->GetLocalPos();
	Vector3 vCenterPoint{};
	Vector3 vDistanceDiff{};
	Vector3 vTestPos = m_pTest->Transform()->GetLocalPos();
	float fDistance = 300.f;
	

	vCenterPoint = Vector3((vPlayerPos.x + m_NpcPos.x) / 2.f, 0.f, (vPlayerPos.z + m_NpcPos.z) / 2.f);
	m_pDummy->Transform()->SetLocalPos(vCenterPoint);

	if(vPlayerPos.x < vCenterPoint.x)
		vDistanceDiff = Vector3(vPlayerPos.x - vCenterPoint.x, 0.f, vPlayerPos.z - vCenterPoint.z) + (vPlayerTrans->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	else 
		vDistanceDiff = Vector3(vPlayerPos.x - vCenterPoint.x, 0.f, vPlayerPos.z - vCenterPoint.z) + (vPlayerTrans->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	
	if (m_bSetChild == false) {
		m_pDummy->AddChild(m_pTest);
		m_pTest->Transform()->SetLocalPos(Vector3(vDistanceDiff.x, 450.f, vDistanceDiff.z));

		m_pTest->AddChild(GetObj());
		GetObj()->Transform()->SetLocalPos(Vector3(0.f, 0.f, 0.f));
		GetObj()->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
		
		m_bSetChild = true;
	}

	if (m_dd < XM_PI / 2) 
		m_dd += DT * 1.0f;
	
	m_pDummy->Transform()->SetLocalRot(Vector3(0.0f, m_dd, 0.0f));
	// muste delete
	//m_pRealTest->Transform()->SetLocalRot(vPlayerTrans->GetLocalRot() + Vector3(XM_PI / 8.5, XM_PI, 0.0f));

	GetObj()->Transform()->SetLocalRot(vPlayerTrans->GetLocalRot() + Vector3(XM_PI / 8.5, XM_PI, 0.0f));

	if (m_bDelCamParent == true)
		GetObj()->ClearParent();
}