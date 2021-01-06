#pragma once


// �������� Ȯ�� ����ü 
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
struct CURRENT_SECTOR
{
	int x;
	int y;
};
