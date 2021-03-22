#pragma once
class CQuad;
class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree() {}

public:
	void update(CQuad* root);
	void insert(const int& x,const int& y, CQuad* root, bool dead = false);
	CQuad* search(const int& x,const int& y, CQuad* root);
private:
	CQuad* m_pRoot;
};

