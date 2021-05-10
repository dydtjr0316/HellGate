#pragma once
#include "../Client/ServerHeader.h"

class CScene;
class CGameObject;
class CToolCamScript;

class CNetMgr
{
public:
	CNetMgr();
	~CNetMgr() {delete m_pObj;}
public:		// obj객체 설정부
	void SetObj(CGameObject* obj) { m_pObj = new CGameObject; m_pObj = obj; }
	void SetCamObj(CGameObject* obj) { m_pCamObj = new CGameObject; m_pCamObj = obj; }

	//void SetCamObj(CToolCamScript* obj) { m_pCamObj = new CToolCamScript; m_pCamObj = obj; }
public:		// 네트워크 연결부
	void Connect();
public:		// 패킷 처리부
	void Recevie_Data();
	void ProcessPacket(char* ptr);
	void Process_Data(char* net_buf, size_t& io_byte);
	void Enter_Player(const int& id);
public:		// 패킷 전송부
	void Send_Packet(void* _packet);
	void Send_LogIN_Packet();
	void Send_Move_Packet(unsigned const char& dir, const Vector3& local);
	void Send_Rotate_Packet(unsigned const char& dir, const float& rotateY);
	void Send_Attack_Packet();
public:
	void err_quit(const char* msg);
private:
	OVERLAPPED m_overlapped;
	CGameObject* m_pObj;
	CGameObject* m_pCamObj;
};

