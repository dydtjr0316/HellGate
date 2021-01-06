#pragma once


// 오버렙드 확장 구조체 
struct EXOVER
{
	WSAOVERLAPPED	over;
	ENUMOPER		op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
	int id;
};

// 타이머
struct event_type
{
	int obj_id;
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
struct CURRENT_SECTOR
{
	int x;
	int y;
};
