#pragma once

typedef unordered_map<int, CGameObject*> OBJLIST;
typedef OBJLIST::iterator OBJITER;

class CMediatorMgr
{
public:
	CMediatorMgr() {}
	~CMediatorMgr() {}

public:
	OBJLIST* GetObjlist(const OBJID& eID) { return &(m_ObjLst[eID]); }
	void Add(CGameObject* pObj, const OBJID& eID, const int& id);
	CGameObject* Find(const OBJID& eID, const int& id);
	void Delete_lst(const OBJID& eID);
	void Delete_Obj(const OBJID& eID, const int& id);
	int Count(const OBJID& eID, const int& id);
	int Size(const OBJID& eID);


private:
	OBJLIST m_ObjLst[OBJID::END];

};

