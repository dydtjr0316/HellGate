#pragma once
class CGameObject;
class CMonster;
class CNPC;
class CClient;
typedef unordered_map<uShort, CGameObject*> OBJLIST;
typedef unordered_set<uShort> RECKONER;
typedef unordered_set<uShort> MONSTER_RECKONER;
typedef unordered_set<uShort> NPC_RECKONER;
//typedef OBJLIST::iterator OBJITER;
//typedef RECKONER::iterator RECKONER_ITER;
class CMediatorMgr
{
private:
	OBJLIST m_ObjectList;
	RECKONER m_ReckonerList;
	MONSTER_RECKONER m_MonsterReckonerList;
	NPC_RECKONER m_NPCReckonerList;
public:
	void InitObject();
public:
	OBJLIST GetObjList() { return m_ObjectList; }
	RECKONER GetReckonerList() { return m_ReckonerList; }
	MONSTER_RECKONER GetMonsterReckonerList() { return m_MonsterReckonerList; }
	NPC_RECKONER GetNPCReckonerList() { return m_NPCReckonerList; }

public:
	bool IsType(const uShort& id, const OBJECT_TYPE& type);
	CClient* Cast_Client(const uShort& id);
	CMonster* Cast_Monster(const uShort& id);
	CNPC* Cast_NPC(const uShort& id);

public:		//  object 吝俺磊 包府何
	void Add(CGameObject* pObj, const uShort& id);
	CGameObject* Find(const uShort& id);
	void Delete_Obj(const uShort& id);
	const size_t Count(const uShort& id);
	const size_t Size();

public:		//  Reckoner 吝俺磊 包府何
	void ReckonerAdd(const uShort& id);
	int ReckonerFind(const uShort& id);
	void Delete_Reckoner(const uShort& id);
	const size_t ReckonerCount(const uShort& id);
	const size_t ReckonerSize();

public:		// Monster Reckoner
	void MonsterReckonerAdd(const uShort& id);
	int MonsterReckonerFind(const uShort& id);
	void Delete_MonsterReckoner(const uShort& id);
	const size_t MonsterReckonerCount(const uShort& id);
	const size_t MonsterReckonerSize();

public:		// npc Reckoner
	void NPCReckonerAdd(const uShort& id);
	int NPCReckonerFind(const uShort& id);
	void Delete_NPCReckoner(const uShort& id);
	const size_t NPCReckonerCount(const uShort& id);
	const size_t NPCReckonerSize();
};

