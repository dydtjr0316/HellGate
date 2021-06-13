#include "stdafx.h"
#include "CMediatorMgr.h"


void CMediatorMgr::Add(CGameObject* pObj, const uShort& id)
{
    if (pObj != nullptr)
        g_Object.emplace(id, pObj);
}

CGameObject* CMediatorMgr::Find(const uShort& id)
{
    if (g_Object.count(id) != 0)
    {
        return  g_Object.find(id)->second;
    }
    return nullptr;
}

void CMediatorMgr::Delete_Obj(const uShort& id)
{
    if (g_Object.count(id) != 0)
    {
        SafeDelete(g_Object.find(id)->second);
        g_Object.erase(id);
    }
}

const size_t CMediatorMgr::Count(const uShort& id)
{
    return g_Object.count(id);
}

const size_t CMediatorMgr::Size()
{
    return g_Object.size();
}

void CMediatorMgr::ReckonerAdd(const uShort& id)
{
    if (g_Reckoner.count(id) == 0)
        g_Reckoner.emplace(id);
}

int CMediatorMgr::ReckonerFind(const uShort& id)
{
    if (g_Reckoner.count(id) != 0)
    {
        return  id;
    }
}

void CMediatorMgr::Delete_Reckoner(const uShort& id)
{
    if (g_Reckoner.count(id) != 0)
    {
        g_Reckoner.erase(id);
    }
}

const size_t CMediatorMgr::ReckonerCount(const uShort& id)
{
    return g_Reckoner.count(id);
}

const size_t CMediatorMgr::ReckonerSize()
{
    return g_Reckoner.size();
}