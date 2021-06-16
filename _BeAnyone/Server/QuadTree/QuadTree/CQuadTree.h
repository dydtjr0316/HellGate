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
	void subdivide();
	vector<CPlayer*> search(CRectangle& range);	// 인자로 vector<CPlayer> found 넣는 방식 생각해보기

public: // get
	CQuadTree* Getnw() { return nw; }
	CQuadTree* Getne() { return ne; }
	CQuadTree* Getsw() { return sw; }
	CQuadTree* Getse() { return se; }
	vector<CPlayer*> GetPoint() { return points; }
	CQuadTree* GetParent() { return parent; }
public: // set
	void setParent(CQuadTree* obj) { parent = obj; }
private:
	bool divide = false;
	int capacity;
	CRectangle boundary;
	vector<CPlayer*> points;
private: // node
	CQuadTree* parent;
	CQuadTree* nw;
	CQuadTree* ne;
	CQuadTree* sw;
	CQuadTree* se;
};

