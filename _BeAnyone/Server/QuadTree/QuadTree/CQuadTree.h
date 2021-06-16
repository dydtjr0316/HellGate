#pragma once
#include "CRectangle.h"
class CPlayer;
class CRectangle;
class CQuadTree
{
public:
	CQuadTree(const CRectangle& _boundary, const int& n);
public:  //oper
	bool insert(CPlayer* p);
	void subm_bisDivide();
	vector<CPlayer*> search(CRectangle& range);	// 인자로 vector<CPlayer> found 넣는 방식 생각해보기
public: // get
	vector<CPlayer*> GetPoint() { return m_vpPlayers; }
	CQuadTree* GetParent() { return m_pParent; }
	vector<CQuadTree*> GetChild() { return m_pChild; }
public: // set
	void setParent(CQuadTree* obj) { m_pParent = obj; }
private:
	bool m_bisDivide = false;
	int m_icapacity;
	CRectangle boundary;
	vector<CPlayer*> m_vpPlayers;
private: // node
	CQuadTree* m_pParent;
	vector<CQuadTree*> m_pChild;
};

