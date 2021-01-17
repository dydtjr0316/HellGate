#pragma once


// �������� Ȯ�� ����ü 
struct EXOVER
{
	WSAOVERLAPPED	over;
	ENUMOP		op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF			wsabuf;
		SOCKET			c_socket;
	};
	int id;
};

// Ÿ�̸�
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

// ���� ���� �Ǻ� ��ǥ .. ���� ����
struct _tSector
{
	int x;
	int y;
	int z;
	_tSector() {}
	_tSector(const int& _x, const int& _y)
	{
		x = _x;
		y = _y;
	}
	void SetSector(const int& _x, const int& _y)
	{
		x = _x;
		y = _y;
	}
};
