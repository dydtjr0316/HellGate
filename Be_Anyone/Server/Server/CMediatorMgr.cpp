#include "stdafx.h"
#include "CMediatorMgr.h"

void CMediatorMgr::Add(CGameObject* pObj, const OBJID& eID, const int& id)
{
	if (pObj != nullptr)
		m_ObjLst[eID].emplace(id, pObj);
}

CGameObject* CMediatorMgr::Find(const OBJID& eID, const int& id)
{
    if (!m_ObjLst[eID].empty())
    {
        OBJITER iter;
        iter = m_ObjLst[eID].find(id);
        return (iter->second);
    }

    return nullptr;

}

void CMediatorMgr::Delete_lst(const OBJID& eID)
{
    for (auto& objlist : m_ObjLst[eID])
    {
        delete (objlist).second;
        (objlist).second = nullptr;
    }

    m_ObjLst[eID].clear();

}

void CMediatorMgr::Delete_Obj(const OBJID& eID, const int& id)
{
    if (!m_ObjLst[eID].empty())
    {
        SafeDelete(m_ObjLst[eID].find(id)->second);
        m_ObjLst[eID].erase(id);
    }
}

int CMediatorMgr::Count(const OBJID& eID, const int& id)
{
    if (!m_ObjLst[eID].empty())
    {
        return m_ObjLst[eID].count(id);
    }

    return 0;
}

int CMediatorMgr::Size(const OBJID& eID)
{
    return m_ObjLst[eID].size();
}
