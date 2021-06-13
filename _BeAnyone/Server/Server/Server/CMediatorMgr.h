#pragma once
class CGameObject;
class CMediatorMgr
{
public:		//  object 중개자 관리부
	void Add(CGameObject* pObj, const uShort& id);
	CGameObject* Find(const uShort& id);
	void Delete_Obj(const uShort& id);
	const size_t Count(const uShort& id);
	const size_t Size();

public:
	void ReckonerAdd(const uShort& id);
	int ReckonerFind(const uShort& id);
	void Delete_Reckoner(const uShort& id);
	const size_t ReckonerCount(const uShort& id);
	const size_t ReckonerSize();
};

