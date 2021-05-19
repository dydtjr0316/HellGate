#pragma once
class CRect;
class CPlayer;
class CNode
{
public:
	CNode(const Point& p, const float& half_len);
	CNode(const float& x, const float& z, const float& half_len);
	~CNode() { m_pParent = nullptr; }

public:
	void SetParent(CNode* parent) { m_pParent = parent; }
	void SetIteration(const int& iter) { m_iIteration = iter; }

public: // Get
	int GetIteration() const { return m_iIteration; }
	CNode* GetParent() const { return m_pParent; }	// 메모리 문제?
	CRect* GetRect() { return m_pRect; }
	vector<CNode> GetChildern() const { return m_vChildern; }	// 이거도 한번 보기
	unordered_map<int, CPlayer*> GetInfo() { return m_mInfo; }
private:
	int m_iIteration = 0;
	CNode* m_pParent;
	CRect* m_pRect;
	vector<CNode> m_vChildern;
	unordered_map<int, CPlayer*> m_mInfo;
};

