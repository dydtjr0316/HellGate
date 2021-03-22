#pragma once
class CQuad;
class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree() {}

public:
	CQuad& GetRoot() { return *m_pRoot; }

public:
	void update(CQuad* root);
	void insert(const int& x,const int& y, CQuad* root, bool dead = false);
	CQuad* search(const int& x,const int& y, CQuad* root);
public:
	CQuad* m_pRoot;
};

