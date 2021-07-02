#include "stdafx.h"
#include "CMediator.h"
#include "CPlayer.h"

void CMediator::Add(CPlayer* pObj, const uShort& id)
{
    if (pObj != nullptr)
        m_obj.emplace(id, pObj);
}

CPlayer* CMediator::Find(const uShort& id)
{
    if (m_obj.count(id) != 0)
    {
        return  m_obj.find(id)->second;
    }
    return nullptr;
}

void CMediator::Delete_Obj(const uShort& id)
{
    if (m_obj.count(id) != 0)
    {
        SafeDelete(m_obj.find(id)->second);
        m_obj.erase(id);
    }
}

const size_t CMediator::Count(const uShort& id)
{
    return m_obj.count(id);
}

const size_t CMediator::Size()
{
    return m_obj.size();
}
