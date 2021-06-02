#pragma once
class CNode;
class CRect;
class CPlayer;
class CQuadTree
{
public:
	CQuadTree();
	~CQuadTree() {};
public:
	CNode* GetRoot() { return m_pRoot; }
public:
	void update(CNode* root);
	void insert(const float& x, const float& y, CNode* root, bool dead = false);
	CNode* search(const float& x, const float& y, CNode* root);

private:
	CNode* m_pRoot;
};

