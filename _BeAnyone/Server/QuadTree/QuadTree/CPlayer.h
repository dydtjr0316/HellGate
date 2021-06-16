#pragma once
class CPlayer
{
public:
	CPlayer();
	CPlayer(const uShort& id, const float& x, const float& z);
public:  // set
	void SetX(const float& x) { this->m_fx = x; }
	void SetZ(const float& z) { this->m_fz = z; }
	void SetID(const uShort& id) { this->m_id = id; }
public:	 // get
	float GetX() { return m_fx; }
	float GetZ() { return m_fz; }
	uShort GetID() { return m_id; }
private:
	float m_fx, m_fz;
	uShort m_id;
};

