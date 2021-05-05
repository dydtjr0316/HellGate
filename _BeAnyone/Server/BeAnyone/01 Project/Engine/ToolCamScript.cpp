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

    
    //// 필요없는거
    //float fScale = Camera()->GetScale();
    //float fSpeed = m_fSpeed;

    // 공통
    float fDistance = 400.f;
    //cout << "toolcam스크립트트트트트트ㅡ틑 변경전" << endl;
    //cout << vPos.x << endl;
    //cout << vPos.y << endl;
    //cout << vPos.z << endl;

    vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
    vPos.y = vPlayerPos->GetLocalPos().y + 450.f;

    //cout << "toolcam스크립트트트트트트ㅡ틑 변경          후" << endl;
    //cout << vPos.x << endl;
    //cout << vPos.y << endl;
    //cout << vPos.z << endl;
    Transform()->SetLocalPos(vPos);
    Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vector3(XM_PI / 6, XM_PI, 0.f));



    

    // 공통
    //Transform()->SetPlayerPosition(vPlayerPos->GetLocalPos());
    
    // 이동


}