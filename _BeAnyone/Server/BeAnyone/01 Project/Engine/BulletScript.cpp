#include "pch.h"
#include "BulletScript.h"
#include "ParticleSystem.h"


CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
	, m_bType(BULLET_TYPE::DEFAULT)
	, m_fTime(0.0f)
	, m_bDead(false)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::update()
{
	Vector3 vPos = Transform()->GetLocalPos();

	Transform()->SetLocalPos(vPos);
	cout << "�Ѿ� ��ġ\t" << "x : " << Transform()->GetLocalPos().x << "\t" << Transform()->GetLocalPos().y << "\t" << Transform()->GetLocalPos().z << endl;


	m_fTime += DT;

	if (m_fTime > 3.0f)
	{
		m_bDead = true;
		DeleteObject(GetObj());
	}
	


}

void CBulletScript::OnCollisionEnter(CCollider* _pOther)
{
	auto a = _pOther->GetObj()->GetName();
	auto ty = m_bType;

	switch ((UINT)m_bType)
	{
	case 0:
		if (L"FireMonster" == _pOther->GetObj()->GetName())
		{
			// ====================
			// Particle Object ����
			// ====================
			CGameObject* pObject = new CGameObject;
			pObject->SetName(L"Particle");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);

			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Transform()->GetLocalPos());

			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

			DeleteObject(GetObj());
		}
		break;
	case 1:
		//	����ٰ� �Ⱦ� �Ҹ� �б⹮ ����
		break;
	}
}
