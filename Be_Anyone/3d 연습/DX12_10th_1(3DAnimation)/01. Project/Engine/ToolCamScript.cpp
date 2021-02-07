#include "stdafx.h"
#include "ToolCamScript.h"
#include "Camera.h"
#include "../Client/Extern.h"



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
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	char dir;

	if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (W_KEY_HOLD)
	{
		if (A_KEY_HOLD)
		{
			// 대각선으로 움직이는 코드 
			netMgr.Send_Move_Packet(MV_LEFT_UP);
		}
		else if (D_KEY_HOLD)
		{
			netMgr.Send_Move_Packet(MV_RIGHT_UP);
		}
		else
		{
			Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vFront * fSpeed * DT;
			netMgr.Send_Move_Packet(MV_UP);
		}
	}

	if (S_KEY_HOLD)
	{
		Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vBack * fSpeed * DT;
		netMgr.Send_Move_Packet(MV_DOWN);
	}

	if (A_KEY_HOLD)
	{
		Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vLeft * fSpeed * DT;
		netMgr.Send_Move_Packet(MV_LEFT);
	}

	if (D_KEY_HOLD)
	{
		Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vRight * fSpeed * DT;
		netMgr.Send_Move_Packet(MV_RIGHT);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_1))
	{
		fScale -= m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_2))
	{
		fScale += m_fScaleSpeed * DT;
		Camera()->SetScale(fScale);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();

		vRot.x -= vDrag.y * DT * 3.f;
		vRot.y += vDrag.x * DT * 1.5f;

		Transform()->SetLocalRot(vRot);
	}

	Transform()->SetLocalPos(vPos);
}
