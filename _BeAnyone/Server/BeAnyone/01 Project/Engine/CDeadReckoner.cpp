#include "pch.h"
#include "CDeadReckoner.h"
#include "GameObject.h"
#include "PlayerScript.h"

CDeadReckoner::CDeadReckoner(const int& id)
{
	m_v3localPos = g_Object.find(id)->second->Transform()->GetLocalPos();
    m_fRotateY = g_Object.find(id)->second->Transform()->GetLocalRot().y;
    m_fSpeed = g_Object.find(id)->second->GetScript<CPlayerScript>()->GetSpeed();
    m_v3DirVec = g_Object.find(id)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT);
}


void CDeadReckoner::DeadReckoning(CGameObject* obj)
{
    m_v3localPos += m_v3DirVec * m_fSpeed * DT;
}

bool CDeadReckoner::isFollowing()
{
    Vector2 player(g_Object.find(g_myid)->second->Transform()->GetLocalPos().x, g_Object.find(g_myid)->second->Transform()->GetLocalPos().z);
    Vector2 follower(m_v3localPos.x, m_v3localPos.z);

    float distance = (player.x - follower.x) * (player.x - follower.x) + (player.y - follower.y) * (player.y - follower.y);

  


    return distance >= (m_fSpeed * DT/2) * (m_fSpeed * DT/2);
}
//
//void CDeadReckoner::initSpline(float x[3], float y[3], float a[2], float b[2], float c[2], float d[2])
//{
//    double h[2];                            // delta x
//    double b[2];                            // delta y / delta x
//    double ftt[3];                      // f''(xi)
//
//    // calc h & b
//    for (int i = 0; i < 2; i++)
//    {
//        h[i] = (x[i + 1] - x[i]);
//        b[i] = (y[i + 1] - y[i]) / h[i];
//    }
//
//    // calc f''(xi)
//    ftt[0] = 0; // f''(x0) = 0
//    for (int i = 0; i < 2 - 1; i++)
//        ftt[i + 1] = 3 * (b[i + 1] - b[i]) / (h[i + 1] + h[i]);
//    ftt[2] = 0; // f''(xn) = 0
//
//    // calc spline coefficients
//    for (int i = 0; i < 2; i++)
//    {
//        a[i] = (ftt[i + 1] - ftt[i]) / (6 * h[i]);
//        b[i] = ftt[i] / 2;
//        c[i] = b[i] - h[i] * (ftt[i + 1] + 2 * ftt[i]) / 6;
//        d[i] = y[i];
//    }
//}
