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
    Vector3 vPos = Transform()->GetLocalPos();
    CTransform* vPlayerPos = g_Object.find(g_myid)->second->Transform();

    Vector3 vRot = Transform()->GetLocalRot();
    Vector3 vPlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();
    XMMATRIX vPlayerMat = g_Object.find(g_myid)->second->Transform()->GetWorldMat();
    Vector3 vFront = g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vector3 vUp = g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::UP);
    Vector3 vRight = g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

    float fScale = Camera()->GetScale();
    float fSpeed = m_fSpeed;
    float fDistance = 400.f;
    vPos = vPlayerPos->GetLocalPos() + (vPlayerPos->GetWorldDir(DIR_TYPE::FRONT) * fDistance);
    vPos.y = vPlayerPos->GetLocalPos().y + 450.f;

    if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
    {
        Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
        vRot.y += vDrag.x * DT * 0.01f;
       
        cout << "*************카메라POS***************" << endl;
        cout << Transform()->GetLocalPos().x << endl;
        cout << Transform()->GetLocalPos().y << endl;
        cout << Transform()->GetLocalPos().z << endl;
        cout << "*************플레이어POS***************" << endl;
        cout << vPlayerPos->GetLocalPos().x << endl;
        cout << vPlayerPos->GetLocalPos().y << endl;
        cout << vPlayerPos->GetLocalPos().z << endl;
        cout << "*************ROTATE***************" << endl;
        cout << Transform()->GetLocalRot().x << endl;
        cout << Transform()->GetLocalRot().y << endl;
        cout << Transform()->GetLocalRot().z << endl;
        cout << "*******************************" << endl;
    }

    

    Transform()->SetPlayerPosition(vPlayerPos->GetLocalPos());
    Transform()->SetLocalRot(vRot);
    Transform()->SetLocalPos(vPos);
    Transform()->SetPlayerWorldMat(vPlayerMat);


}