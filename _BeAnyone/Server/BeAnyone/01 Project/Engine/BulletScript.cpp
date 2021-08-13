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
	//cout << "총알 위치\t" << "x : " << Transform()->GetLocalPos().x << "\t" << Transform()->GetLocalPos().y << "\t" << Transform()->GetLocalPos().z << endl;


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
			// Particle Object 생성
			// 떄린놈 이름이 FireMonster 일 때 이 파티클을 생성한다.
			// ====================
			CGameObject* pObject = new CGameObject;
			pObject->SetName(L"Particle");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);

			pObject->FrustumCheck(false);
			pObject->Transform()->SetLocalPos(Transform()->GetLocalPos());

			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->AddGameObject(pObject);

			
			/// 여기까지 파티클 생성하는거
			/// ////////////////////////////////////////////////////////////////////////////////

			//DeleteObject(GetObj());
			m_fTime += 4.0f;
		}
		break;
	case 1:
		//	여기다가 픽업 불릿 분기문 설정
		break;
	}
}
