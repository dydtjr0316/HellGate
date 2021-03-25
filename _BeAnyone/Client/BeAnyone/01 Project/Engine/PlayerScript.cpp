#include "pch.h"
#include "PlayerScript.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
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
	Vector3 vPos = Transform()->GetLocalPos();
	Vector3 vRot = Transform()->GetLocalRot();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		vPos.z += DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		vPos.z -= DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		vPos.x -= DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		vPos.x += DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		vRot.z += DT * XM_PI;

		// 복사 메테리얼을 MeshRender 에 세팅
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		// z 키를 떼면 원레 메테리얼로 돌아감
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vector3 vRot = Transform()->GetLocalRot();

		//vRot.x -= vDrag.y * DT * 3.f;
		vRot.y += vDrag.x * DT * 1.5f;

		Transform()->SetLocalRot(vRot);
	}

	Transform()->SetLocalPos(vPos);
	//Transform()->SetLocalRot(vRot);
}