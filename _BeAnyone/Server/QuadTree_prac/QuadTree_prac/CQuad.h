#pragma once
class CRect;
class CPlayer;
class CQuad
{
public:
	CQuad(const int& x, const int& y, const int& w, const int& h);
	~CQuad() { m_pParent = nullptr; }

public: // Get
	int GetIteration() const { return m_iIteration; }
	CQuad& GetParent() const { return *m_pParent; }	// �޸� ����?
	CRect& GetRect() const { return *m_pRect; }
	vector<CQuad> GetChildern() const { return m_vChildern; }	// �̰ŵ� �ѹ� ����
	unordered_map<int, CPlayer> GetInfo()const { return m_mInfo; }


private:
	int m_iIteration = 0;
	CQuad* m_pParent;
	CRect* m_pRect;
	vector<CQuad> m_vChildern;
	unordered_map<int, CPlayer> m_mInfo;
};

