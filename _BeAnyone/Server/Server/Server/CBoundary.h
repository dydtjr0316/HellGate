#pragma once
#include "CGameObject.h"
class CGameObject;
class CBoundary
{
public:
	CBoundary();
	CBoundary(const float& x, const float& z, const float& w, const float& h);
	CBoundary(CGameObject* player, const float& searchsize = SEARCH_SIZE);
	CBoundary(CClient* player, const float& searchsize = SEARCH_SIZE);
public:  // oper
	bool contains(CGameObject* p);
	bool intersects(CBoundary& range);
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

