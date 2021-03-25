#include "stdafx.h"
#include "CNetMgr.h"

void CNetMgr::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CNetMgr::Connect()
{

	wcout.imbue(locale("korean"));
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)err_quit("WSAStartup");
	g_Socket = WSASocket(PF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);

	if (g_Socket == INVALID_SOCKET)err_quit("WSASocket");

	WSAEVENT event;


	SOCKADDR_IN recvAddr;
	memset(&recvAddr, 0, sizeof(recvAddr));

	recvAddr.sin_family = AF_INET;
	inet_pton(AF_INET, (const char*)"127.0.0.1", &(recvAddr.sin_addr.s_addr)); // inet addr sdl검사 끄고 실행 해도 안되서 pton으로 수정
	recvAddr.sin_port = htons(SERVER_PORT);

	if (connect(g_Socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)err_quit("Connect()");

	event = WSACreateEvent();
	memset(&m_overlapped, 0, sizeof(m_overlapped));

	m_overlapped.hEvent = event;

}

void CNetMgr::Send_Packet(void* _packet)
{
	EXOVER dataBuf;
	char* packet = reinterpret_cast<char*>(_packet);
	size_t sent;
	dataBuf.wsabuf.len = packet[0];
	dataBuf.wsabuf.buf = (char*)packet;
	dataBuf.over = m_overlapped;

	if (WSASend(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&sent, 0, &dataBuf.over, NULL) != 0)err_quit("WSASend");
}

void CNetMgr::Send_LogIN_Packet()
{
	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = CS_LOGIN;
	int t_id = GetCurrentProcessId();
	char tempName[MAX_ID_LEN];
	string tempstring;
	cout << "nick name : ";
	cin >> tempName;
	sprintf_s(l_packet.name, tempName);
	strcpy_s(name, l_packet.name);

	cout << sizeof(l_packet) << endl << endl;

	Send_Packet(&l_packet);
}

void CNetMgr::Send_Move_Packet(unsigned const char& dir)
{
	cs_packet_move m_packet;
	m_packet.type = CS_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	Send_Packet(&m_packet);
}

void CNetMgr::Send_Attack_Packet()
{
	cs_packet_attack m_packet;
	m_packet.type = CS_ATTACK;
	m_packet.size = sizeof(m_packet);
	Send_Packet(&m_packet);
}

void CNetMgr::testX(const float& _x)
{
	x = _x;
	cout << "test : " << x << endl;
}



void CNetMgr::Recevie_Data()
{
	EXOVER dataBuf;
	size_t recvByte = 0;
	int flags = 0;
	dataBuf.over = m_overlapped;

	auto recv_result = WSARecv(g_Socket, &dataBuf.wsabuf, 1, (LPDWORD)&recvByte, (LPDWORD)&flags, &(dataBuf.over), NULL);
	if (recv_result == 0)
	{
		cout << "수신 완료" << endl;
		if (recvByte > 0)Process_Data((dataBuf.wsabuf).buf, recvByte);
	}
}

void CNetMgr::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{
	case SC_PACKET_LOGIN_OK:
	{

	}
	break;
	case SC_PACKET_ENTER:
	{

	}
	break;
	case SC_PACKET_MOVE:
	{
		sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = packet->id;

		cout << "packet x : " << packet->x << endl;
		testX(packet->x);

		/*m_Pos.x = packet->x;
		m_Pos.y = packet->y;
		m_Pos.z = packet->z;*/

		/*cout << m_Pos.x << endl;
		cout << m_Pos.y << endl;
		cout << m_Pos.z << endl;*/
	}
	break;
	case SC_PACKET_LEAVE:
	{

	}
	break;
	case SC_PACKET_CHAT:
	{

	}
	break;
	case SC_PACKET_ATTACK:
	{

	}
	break;
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void CNetMgr::Process_Data(char* net_buf, size_t& io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[MAX_BUF_SIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

