#pragma once
class CQuad;
class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree() {}

public:
	void update(CQuad* root);
	void insert(const int& x, const int& y, CQuad* root, const bool dead);
private:
	CQuad* m_pRoot;
};

