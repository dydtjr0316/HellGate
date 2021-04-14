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
	Vector3 vPos = Transform()->GetLocalPos();
	Vector3 vPlayerPos = m_pPlayer->Transform()->GetLocalPos();

	Vector3 vRot = Transform()->GetLocalRot();
	Vector3 vPlayerRot = m_pPlayer->Transform()->GetLocalRot();
	XMMATRIX vPlayerMat = m_pPlayer->Transform()->GetWorldMat();
	Vector3 vFront = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 vUp = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::UP);
	Vector3 vRight = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;
	float fDistance = 400.f;

	//vPos.x = vPlayerPos.x;
	//vPos.y = vPlayerPos.y  +450;
	//vPos.z = vPlayerPos.z - 400;

	vPos = vPlayerPos + (vFront * fDistance);
	vPos.y = 600;

 	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		Vector3 vFronta = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vFronta * DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		Vector3 vBacka = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vBacka * DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{
		Vector3 vLefta = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLefta * DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		Vector3 vRighta = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRighta * DT * 200.f;
	}
	

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		//Vector3 vRot = Transform()->GetLocalRot();
		//Vector3 a;
		
		
		//Matrix matRot = XMMatrixRotationAxis(vPlayerPos, vDrag.x * 0.0001f);
		//a = XMVector3TransformNormal(vPos, matRot);
		//vPos = a;
	
	
		//vRot.x -= vDrag.y * DT * 2.f;
		vRot.y += vDrag.x * DT * 1.5f;
	
		
		
		//Transform()->SetLocalRot(vRot);
		
	
		
	}
	Transform()->SetPlayerPosition(vPlayerPos);
	//Transform()->Set3Camera(true);
	Transform()->SetLocalRot(vRot);
	Transform()->SetLocalPos(vPos);
	Transform()->SetPlayerWorldMat(vPlayerMat);
	//Transform()->SetWorldDir(vFront, DIR_TYPE::FRONT);
	//Transform()->SetWorldDir(vUp, DIR_TYPE::UP);
	//Transform()->SetWorldDir(vRight, DIR_TYPE::RIGHT);
	//Transform()->SetPlayerRotation(vPlayerRot);

}
