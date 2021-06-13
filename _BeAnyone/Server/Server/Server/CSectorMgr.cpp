#include "stdafx.h"
#include "CSectorMgr.h"
#include "CGameObject.h"
CSectorMgr::CSectorMgr()
{
    
}
CSectorMgr::~CSectorMgr()
{

}
unordered_set<uShort> CSectorMgr::GetSector(const uShort& row, const uShort& Column)
{
	//if (row > SECTOR_ROW || Column > SECTOR_COL || row < 0 || Column < 0);
	return m_Sector[row][Column];
}

bool CSectorMgr::Is_Near(const uShort& id1, const uShort& id2)
{
    if (id1 == id2)return false;
    float dist =
        (Netmgr.GetMediatorMgr()->Find(id1)->GetLocalPosVector().x - Netmgr.GetMediatorMgr()->Find(id2)->GetLocalPosVector().x)
        * (Netmgr.GetMediatorMgr()->Find(id1)->GetLocalPosVector().x - Netmgr.GetMediatorMgr()->Find(id2)->GetLocalPosVector().x);

    dist += (Netmgr.GetMediatorMgr()->Find(id1)->GetLocalPosVector().z - Netmgr.GetMediatorMgr()->Find(id2)->GetLocalPosVector().z)
        * (Netmgr.GetMediatorMgr()->Find(id1)->GetLocalPosVector().z - Netmgr.GetMediatorMgr()->Find(id2)->GetLocalPosVector().z);

    return dist <= (float)(VIEW_LIMIT * VIEW_LIMIT);
}

bool CSectorMgr::IsExist(const uShort& row, const uShort& Column, const uShort& id)
{
	if (m_Sector[row][Column].count(id) != 0)return true;
	else return false;
}

void CSectorMgr::Emplace(const uShort& row, const uShort& Column, const uShort& id)
{
	/*Netmgr.GetMediatorMgr()->Count(id) != 0  �̰� �˻��ؾߵǴ°�?*/
	// ���ʿ� �ߺ��� ������� ������ exist�Լ���ü�� �ʿ���°�? ������ �����غ���
	// erase�� ��������
	if (!IsExist(row, Column, id))
		m_Sector[row][Column].emplace(id);
}

void CSectorMgr::Erase(const uShort& row, const uShort& Column, const uShort& id)
{
	if (IsExist(row, Column, id))
		m_Sector[row][Column].erase(id);
}

vector<unordered_set<uShort>> CSectorMgr::Search_Sector(CGameObject* obj)
{
    vector<unordered_set<uShort>> vSectors;

    // sector�� ���� ���� �Ұ�?
    uShort x1 = ((uShort)obj->GetLocalPosVector().x - VIEW_LIMIT) / SECTOR_COL_Length;
    uShort z1 = ((uShort)obj->GetLocalPosVector().z - VIEW_LIMIT) / SECTOR_ROW_Length;

    x1 = x1 < 0 ? 0 : x1;
    z1 = z1 < 0 ? 0 : z1;

    uShort x2 = ((uShort)obj->GetLocalPosVector().x + VIEW_LIMIT) / SECTOR_COL_Length;
    uShort z2 = ((uShort)obj->GetLocalPosVector().z + VIEW_LIMIT) / SECTOR_ROW_Length;

    x2 = x2 > SECTOR_ROW - 1 ? SECTOR_ROW - 1 : x2;
    z2 = z2 > SECTOR_COL - 1 ? SECTOR_COL - 1 : z2;

    if (x1 == x2 && z1 == z2) // ���� �ϳ� �˻�
    {
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x1, z1));
    }
    else if (x1 == x2) // ��*�� �˻�
    {
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x1, z1));
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x1, z2));
    }
    else if (z1 == z2) // ��*�� �˻�
    {
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x1, z1));
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x2, z1));
    }
    else // ����� �˻�
    {
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x1, z2));
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x2, z2));
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x1, z1));
        vSectors.push_back(CSectorMgr::GetInst()->GetSector(x2, z1));
    }
    return vSectors;
}
