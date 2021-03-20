#pragma once
struct tRect	// 진짜 필요한 구조체인지 다시 생각해보기 
{
	int m_ix, m_iy, m_iw, m_ih;

	tRect(){
		m_ix = 0;
		m_iy = 0;
		m_iw = 0;
		m_ih = 0;
	}

	void set(const int& x, const int& y, const int& w, const int& h)
	{
		m_ix = x;
		m_iy = y;
		m_iw = w;
		m_ih = h;
	}
};