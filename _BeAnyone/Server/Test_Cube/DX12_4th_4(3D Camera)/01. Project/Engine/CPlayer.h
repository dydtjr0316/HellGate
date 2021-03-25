#pragma once
#include "GameObject.h"
class CPlayer :
    public CGameObject
{
public:
    void SetID(const int& id) { m_id = id; }
    int GetID() { return m_id; }
private:
    int m_id;
};

