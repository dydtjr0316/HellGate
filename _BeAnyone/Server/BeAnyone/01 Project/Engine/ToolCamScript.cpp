#include "pch.h"
#include "ToolCamScript.h"

#include "Camera.h"

CToolCamScript::CToolCamScript()
    : CScript(0)
    , m_fSpeed(200.f)
    , m_fScaleSpeed(1.f)
{
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
	Vector3 vPos = Transform()->GetLocalPos();
	CTransform* vPlayerPos = g_Object.find(g_myid)->second->Transform();

	// 공통
	float fDistance = 500.f; //200.f;

	vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
	vPos.y = vPlayerPos->GetLocalPos().y + 450.f; // 200.f;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vector3(XM_PI / 8.5, XM_PI, 0.f));
	cout << "npcCamera" << endl;
}