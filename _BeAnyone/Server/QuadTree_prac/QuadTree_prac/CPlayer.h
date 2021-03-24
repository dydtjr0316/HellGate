#pragma once
class CQuad;
class CRect;
class CPlayer
{
public:
	CPlayer();
	CPlayer(const int& x, const int& y);

public:
	void clean();

public: // Set
	void SetParent(CQuad* parent) { m_pParent = parent; }	//  이거 맞는건가?

public: // Get
	bool GetDead() const { return m_bisDead; }
	int GetID() const { return m_iID; }
	int GetValue() const { return m_iValue; }
	int GetCounter() const { return m_iCounter; }
	CQuad* GetParent() const { return m_pParent; }
	CRect* GetRect()  { return m_pRect; }

	static int m_iCounter;
private:
	bool m_bisDead = false;
	int m_iID;
	static const int m_iValue;
	CQuad* m_pParent = nullptr;
	CRect* m_pRect = nullptr;

};

