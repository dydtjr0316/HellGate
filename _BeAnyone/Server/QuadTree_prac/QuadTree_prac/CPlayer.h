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

public: // Get
	bool GetDead() const { return m_bisDead; }
	int GetID() const { return m_iID; }
	int GetValue() const { return m_iValue; }
	int GetCounter() const { return m_iCounter; }
	CQuad& GetParent() const { return *m_pParent; }
	CRect& GetRect() const { return *m_pRect; }

private:
	bool m_bisDead = false;
	int m_iID;
	static const int m_iValue;
	static int m_iCounter;
	CQuad* m_pParent = nullptr;
	CRect* m_pRect = nullptr;

};

