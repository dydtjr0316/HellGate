#include "pch.h"
#include "FenceScript.h"

CFenceScript::CFenceScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
    , m_eDoorState(DOOR_STATE::IDLE)
{
    CGameObject* childDummy = new CGameObject;
    childDummy->SetName(L"ChildDummy");
    childDummy->FrustumCheck(false);   // 절두체 컬링 사용하지 않음
    childDummy->AddComponent(new CTransform);
    childDummy->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));
    CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Map")->AddGameObject(childDummy);
    m_pChildDummy = childDummy;
}

CFenceScript::~CFenceScript()
{
}

void CFenceScript::Init()
{
    Vector3 vPos = Transform()->GetLocalPos();
    m_pChildDummy->Transform()->SetLocalPos(Vector3(vPos.x - 150.f, vPos.y, vPos.z));
    m_pChildDummy->AddChild(GetObj());
    Transform()->SetLocalPos(Vector3(150.0f, 0.0f, 0.0f));
   
}

void CFenceScript::update()
{
    if (m_bDoRotate)
        DoRotate();
}

void CFenceScript::OnCollisionEnter(CCollider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"PlayerMale") {
        m_bDoRotate = true;
    }
}

void CFenceScript::OnCollisionExit(CCollider* _pOther)
{
}

void CFenceScript::OnCollsion(CCollider* _pOther)
{
}

void CFenceScript::DoRotate()
{
    switch (m_eDoorState) {
    case DOOR_STATE::IDLE:
        if (m_fDegree < XM_PI / 2)
            m_fDegree += DT * 1.0f;
        else {
            m_bDoRotate = false;
            m_eDoorState = DOOR_STATE::OPEN;
            return;
        }
        m_pChildDummy->Transform()->SetLocalRot(Vector3(0.0f, -m_fDegree, 0.0f));
        break;
    case DOOR_STATE::OPEN:
        if (m_fDegree > 0.0f)
            m_fDegree -= DT * 1.0f;
        else {
            m_bDoRotate = false;
            m_eDoorState = DOOR_STATE::IDLE;
            return;
        }
        m_pChildDummy->Transform()->SetLocalRot(Vector3(0.0f, -m_fDegree, 0.0f));
        break;
    default:
        break;
    }
}


