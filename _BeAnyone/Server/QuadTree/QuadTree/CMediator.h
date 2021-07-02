#pragma once
class CPlayer;
typedef unordered_map<uShort, CPlayer*> OBJLIST;
class CMediator
{
private:
	OBJLIST m_obj;
public:
	void Add(CPlayer* pObj, const uShort& id);
	CPlayer* Find(const uShort& id);
	void Delete_Obj(const uShort& id);
	const size_t Count(const uShort& id);
	const size_t Size();
};

