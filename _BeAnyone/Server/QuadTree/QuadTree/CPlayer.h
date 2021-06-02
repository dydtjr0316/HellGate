#pragma once
class CNode;
class CPlayer
{
public:
	void SetParent(CNode* parent) { m_pParent = parent; }
	void clean();
public:
	float id;
	bool m_bisDead = false;
	CNode* m_pParent = nullptr; 
	float x, z;
};

