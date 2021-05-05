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
    // 공통
    Vector3 vPos = Transform()->GetLocalPos();
    CTransform* vPlayerPos = g_Object.find(g_myid)->second->Transform();
   

    // 공통
    float fDistance = 400.f;

    vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
    vPos.y = vPlayerPos->GetLocalPos().y + 450.f;


    Transform()->SetLocalRot(vPlayerPos->Transform()->GetLocalRot() + Vector3(XM_PI / 6, XM_PI, 0.f));
    Transform()->SetLocalPos(vPos);


}