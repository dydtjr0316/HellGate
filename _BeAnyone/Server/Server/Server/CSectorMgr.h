#pragma once
class CGameObject;
class CSectorMgr
{
public:
	SINGLE(CSectorMgr);
public:
	unordered_set<uShort> GetSector(const uShort& row, const uShort& Column);
	bool Is_Near(const uShort& id1, const uShort& id2);
public:
	//emplace, erase,
	bool IsExist(const uShort& row, const uShort& Column, const uShort& id);
	void Emplace(const uShort& row, const uShort& Column, const uShort& id);
	void Erase(const uShort& row, const uShort& Column, const uShort& id);
	vector<unordered_set<uShort>> Search_Sector(CGameObject* obj);
private:
	unordered_set<uShort> m_Sector[SECTOR_ROW][SECTOR_COL];
};

