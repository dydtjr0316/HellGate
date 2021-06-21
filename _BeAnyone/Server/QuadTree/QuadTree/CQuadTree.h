#pragma once
#include "CRectangle.h"
class CPlayer;
class CRectangle;
class CQuadTree
{
public:
	CQuadTree(const CRectangle& _boundary, const int& n);
private:
	void SubDivideToChild();
	bool IsSameObject(const uShort& p1, const uShort& p2);
public:  //oper
	bool insert(CPlayer* p);
	void Delete(CPlayer* p);
	void sub_Divide();
	vector<CPlayer*> search(CRectangle& range);	// ���ڷ� vector<CPlayer> found �ִ� ��� �����غ���
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

