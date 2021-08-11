#pragma once


// 오버렙드 확장 구조체 
struct EXOVER
{
	WSAOVERLAPPED	over;
	ENUMOP		op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
	uShort id;
};

// 타이머
struct event_type
{
	uShort obj_id;
	system_clock::time_point wakeup_time;
	int event_id;
	int target_id;
	constexpr bool operator < (const event_type& _Left) const
	{
		return (wakeup_time > _Left.wakeup_time);
	}
};

///////////////////////////////////////////////////////////////////////////////////////

// 현재 섹터 판별 좌표 .. 추후 수정
struct _tSector
{
	uShort x;
	uShort z;
	_tSector() { x = z = 0; }
	_tSector(const uShort& _x, const uShort& _z)
	{
		x = _x;
		z = _z;
	}
	void SetSector(const uShort& _x, const uShort& _z)
	{
		x = _x;
		z = _z;
	}
};


