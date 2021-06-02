#include "pch.h"
#include "BulletScript.h"


CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::update()
{
	Vector3 vPos = Transform()->GetLocalPos();

	Transform()->SetLocalPos(vPos);
}

void CBulletScript::OnCollisionEnter(CCollider* _pOther)
{
	if (L"Monster Object" == _pOther->GetObj()->GetName())
	{
		DeleteObject(GetObj());
	}
}
