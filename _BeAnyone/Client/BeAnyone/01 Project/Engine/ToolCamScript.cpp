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

	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	vPos.x = vPlayerPos.x;
	vPos.y = vPlayerPos.y + 450;
	vPos.z = vPlayerPos.z - 400;

	


	//vRot.x = vPlayerRot.x;
	//vRot.y = XM_PI / (vPlayerRot.y);

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		//Vector3 vRot = Transform()->GetLocalRot();
		Vector3 a;
		
		
		Matrix matRot = XMMatrixRotationAxis(vPlayerPos, vDrag.x * 0.0001f);
		a = XMVector3TransformNormal(vPos, matRot);
		vPos = a;


		//vRot.x -= vDrag.y * DT * 2.f;
		//vRot.y += vDrag.x * DT * 1.0f;

		//Transform()->SetLocalRot(vRot);
	}

	//Transform()->SetLocalRot(vRot);
	Transform()->SetLocalPos(vPos);
}
