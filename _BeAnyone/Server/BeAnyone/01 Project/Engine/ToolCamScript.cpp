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
    // ����
    Vector3 vPos = Transform()->GetLocalPos();
    CTransform* vPlayerPos = g_Object.find(g_myid)->second->Transform();

    
    //// �ʿ���°�
    //float fScale = Camera()->GetScale();
    //float fSpeed = m_fSpeed;

    // ����
    float fDistance = 400.f;
    //cout << "toolcam��ũ��ƮƮƮƮƮƮ�Ѻz ������" << endl;
    //cout << vPos.x << endl;
    //cout << vPos.y << endl;
    //cout << vPos.z << endl;

    vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
    vPos.y = vPlayerPos->GetLocalPos().y + 450.f;

    //cout << "toolcam��ũ��ƮƮƮƮƮƮ�Ѻz ����          ��" << endl;
    //cout << vPos.x << endl;
    //cout << vPos.y << endl;
    //cout << vPos.z << endl;
    Transform()->SetLocalPos(vPos);
    Transform()->SetLocalRot(vPlayerPos->GetLocalRot() + Vector3(XM_PI / 6, XM_PI, 0.f));



    

    // ����
    //Transform()->SetPlayerPosition(vPlayerPos->GetLocalPos());
    
    // �̵�


}