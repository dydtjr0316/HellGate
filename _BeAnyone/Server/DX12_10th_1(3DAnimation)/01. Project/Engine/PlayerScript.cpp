#include "stdafx.h"
#include "PlayerScript.h"

#include "BulletScript.h"
#include "TestScript.h"

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
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		vPos.y += DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		vPos.y -= DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LEFT))
	{	
		vPos.x -= DT * 200.f;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_RIGHT))
	{
		vPos.x += DT * 200.f;
	}

	// z Ű�� ������ z �� ȸ��
	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		vRot.z += DT * XM_PI;
							
		// ���� ���׸����� MeshRender �� ����
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		// z Ű�� ���� ���� ���׸���� ���ư�
		MeshRender()->SetMaterial(m_pOriginMtrl);		
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);

	if (KEY_TAB(KEY_TYPE::KEY_SPACE))
	{
		// �̻��� ���
		CGameObject* pBullet = new CGameObject;
		pBullet->SetName(L"Bullet Object");

		vPos.y += 50;
		pBullet->AddComponent(new CTransform());
		pBullet->Transform()->SetLocalPos(vPos);
		pBullet->Transform()->SetLocalScale(Vec3(50.f, 50.f, 1.f));

		pBullet->AddComponent(new CMeshRender);
		pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		pBullet->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

		pBullet->AddComponent(new CCollider2D);
		pBullet->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
		
		pBullet->AddComponent(new CBulletScript);
			   
		CreateObject(pBullet, L"Bullet");
	}
}
