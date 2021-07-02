#pragma once
#include "CPlayer.h"
class CPlayer;
class CRectangle
{
public:
	CRectangle();
	CRectangle(const float& x, const float& z, const float& w, const float& h);
	CRectangle(CPlayer* player, const float& w = 1000.f, const float& h = 1000.f);
public:  // oper
	bool contains(CPlayer* p);
	bool intersects(CRectangle& range);
public: //set
	void SetX(const float& x) { m_fx = x; }
	void SetZ(const float& z) { m_fz = z; }
	void SetW(const float& w) { m_fw = w; }
	void SetH(const float& h) { m_fh = h; }
public:	//get
	float GetX() { return m_fx; }
	float GetZ() { return m_fz; }
	float GetW() { return m_fw; }
	float GetH() { return m_fh; }
private:
	float m_fx, m_fz, m_fw, m_fh;
};

