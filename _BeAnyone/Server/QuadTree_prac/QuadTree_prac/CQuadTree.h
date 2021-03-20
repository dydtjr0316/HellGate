#pragma once
class CQuad;
class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree() {}

public:
	void update(CQuad* root);
	void insert(int& x, int& y, CQuad* root, bool dead);
	CQuad* search(int& x, int& y, CQuad* root);
private:
	CQuad* m_pRoot;
};

