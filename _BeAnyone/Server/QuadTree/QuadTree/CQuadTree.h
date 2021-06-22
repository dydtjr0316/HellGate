#pragma once
#include "CRectangle.h"
class CPlayer;
class CRectangle;
class CMediator;
class CQuadTree
{
public: 
	void PrintQuadTree();
public:
	CQuadTree(const CRectangle& _boundary, const int& n);
private:
	void SubDivideToChild();
	bool IsSameObject(const uShort& p1, const uShort& p2);
public:  //oper
	bool insert(CPlayer* p);
	void Delete(CPlayer* p);
	void sub_Divide();
	unordered_set<uShort> search(CRectangle& range);	// 인자로 vector<CPlayer> found 넣는 방식 생각해보기
public: // get
	unordered_set<uShort> GetPoint() { return m_vpPlayers; }
	CQuadTree* GetParent() { return m_pParent; }
	vector<CQuadTree*> GetChild() { return m_pChild; }
public: // set
	void setParent(CQuadTree* obj) { m_pParent = obj; }
private:
	bool m_bisDivide = false;
	int m_icapacity;
	CRectangle boundary;
	
private: // node
	CQuadTree* m_pParent;
	vector<CQuadTree*> m_pChild;
	unordered_set<uShort> m_vpPlayers;
};

