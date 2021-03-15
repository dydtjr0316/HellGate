#pragma once
class CQuad;
class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree() {}

public:
	void update(const CQuad* root);
private:
	CQuad* m_pRoot;
};

