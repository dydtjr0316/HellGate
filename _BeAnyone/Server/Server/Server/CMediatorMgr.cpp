#include "stdafx.h"
#include "CMediatorMgr.h"
#include "CGameObject.h"

void CMediatorMgr::InitObject()
{
    // Client init
    CGameObject* pObj = nullptr;
    for (int i = 0; i < MAX_USER; ++i) {
        pObj = new CClient;
        pObj->SetID(i);
        pObj->SetStatus(OBJSTATUS::ST_FREE);
        pObj->SetType(OBJECT_TYPE::CLIENT);
        Add(pObj, i);
    }

    // Monster init
    srand((unsigned int)time(NULL));
    for (int i = START_MONSTER; i < END_MONSTER; ++i) {
        pObj = new CMonster;
        pObj->SetPosV((float)(1500), 300.f, (float)(5000 + (i - 1000) * 400));
        pObj->SetID(i);
        dynamic_cast<CMonster*>(pObj)->SetHP(100);
        pObj->SetType(OBJECT_TYPE::MONSTER);
        pObj->SetStatus(OBJSTATUS::ST_SLEEP);
        Add(pObj, i);
        pObj->Insert_Sector();
        cout << "ID : " << i << "  HP : " << dynamic_cast<CMonster*>(pObj)->GetHP() << endl;
    }

    // NPC init
    /*for (int i = START_NPC; i < END_NPC; ++i)
    {
        pObj = new CNPC;
        pObj->SetPosV((float)(rand() % WORLD_WIDTH), (float)(rand() % WORLD_WIDTH), (float)(rand() % WORLD_WIDTH));
        pObj->SetID(i);
        char npc_name[50];
        sprintf_s(npc_name, "N%d", i);
        pObj->SetName(npc_name);
        pObj->SetStatus(OBJSTATUS::ST_SLEEP);
        pObj->SetType(OBJECT_TYPE::NPC);
        pObj->Insert_Sector();
        m_pMediator->Add(pObj, i);

    }*/
}

bool CMediatorMgr::IsType(const uShort& id, const OBJECT_TYPE& type)
{
    if (Count(id) == 0)return false;
    if (Find(id)->GetType() == type)return true;
    else return false;
}


CClient* CMediatorMgr::Cast_Client(const uShort& id)
{
    CGameObject* obj = Find(id);
    if (obj != nullptr)return dynamic_cast<CClient*>(obj);
    else return nullptr;
}

CMonster* CMediatorMgr::Cast_Monster(const uShort& id)
{
    CGameObject* obj = Find(id);
    if (obj != nullptr)return dynamic_cast<CMonster*>(obj);
    else return nullptr;
}
CNPC* CMediatorMgr::Cast_NPC(const uShort& id)
{
    CGameObject* obj = Find(id);
    if (obj != nullptr)return dynamic_cast<CNPC*>(obj);
    else return nullptr;
}
void CMediatorMgr::Add(CGameObject* pObj, const uShort& id)
{
    if (pObj != nullptr)
        m_ObjectList.emplace(id, pObj);
}

CGameObject* CMediatorMgr::Find(const uShort& id)
{
    if (m_ObjectList.count(id) != 0)
    {
        return  m_ObjectList.find(id)->second;
    }
    return nullptr;
}

void CMediatorMgr::Delete_Obj(const uShort& id)
{
    if (m_ObjectList.count(id) != 0)
    {
        SafeDelete(m_ObjectList.find(id)->second);
        m_ObjectList.erase(id);
    }
}

const size_t CMediatorMgr::Count(const uShort& id)
{
    return m_ObjectList.count(id);
}

const size_t CMediatorMgr::Size()
{
    return m_ObjectList.size();
}

void CMediatorMgr::ReckonerAdd(const uShort& id)
{
    if (m_ReckonerList.count(id) == 0)
        m_ReckonerList.emplace(id);
}

int CMediatorMgr::ReckonerFind(const uShort& id)
{
    if (m_ReckonerList.count(id) != 0)
    {
        return  id;
    }
}

void CMediatorMgr::Delete_Reckoner(const uShort& id)
{
    if (m_ReckonerList.count(id) != 0)
    {
        m_ReckonerList.erase(id);
    }
}

const size_t CMediatorMgr::ReckonerCount(const uShort& id)
{
    return m_ReckonerList.count(id);
}

const size_t CMediatorMgr::ReckonerSize()
{
    return m_ReckonerList.size();
}