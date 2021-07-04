#pragma once
#include "CBoundary.h"
class CGameObject;
class CBoundary;
class CQuadTree
{
public:
	CQuadTree(const CBoundary& _m_boundary, const int& n);
public:
	void PrintQuadTree();

public:  //oper
	bool Insert(CGameObject* p);
	void Delete(CGameObject* p);
	void Sub_Divide();
	void SubDivideToChild();
	unordered_set<uShort> search(const CBoundary& range);

public: // get
	unordered_set<uShort>& GetPoint() { return m_vpPlayers; }
	CQuadTree* GetParent() { return m_pParent; }
	vector<CQuadTree*>& GetChild() { return m_pChild; }
public: // set
	void SetParent(CQuadTree* obj) { m_pParent = obj; }
private:
	bool IsSameObject(const uShort& p1, const uShort& p2);
private:
	bool m_bisDivide = false;
	int m_icapacity;
	int m_iDepth = 0;
private: // node
	CBoundary m_boundary;
	CQuadTree* m_pParent;
	vector<CQuadTree*> m_pChild;
	unordered_set<uShort> m_vpPlayers;
};

