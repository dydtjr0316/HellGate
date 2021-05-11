#pragma once
#pragma once

// ���� ������ ��
constexpr float MOVE_SPEED = 5.f; // �̵���Ŷ �������� ������ ����

// ����  ����
constexpr int NAME_LEN = 50;
constexpr int PHONE_LEN = 60;
//
  
// constexpr
constexpr auto MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;

// ���� ��Ʈ
constexpr int SERVER_PORT = 3389;
constexpr int MAX_ID_LEN = 10;
constexpr int MAX_STR_LEN = 100;

// ����, ���� ,  ����
constexpr int MAX_USER = 1000;
constexpr int END_USER = MAX_USER;

constexpr int START_MONSTER = MAX_USER;
constexpr int MAX_MONSTER = 3000;
constexpr int END_MONSTER = START_MONSTER + MAX_MONSTER;
constexpr int DIVIDE_MONNSTER = MAX_MONSTER / 4;

constexpr int START_NPC = END_MONSTER;
constexpr int MAX_NPC = 2000;
constexpr int END_NPC = START_NPC + MAX_NPC;

constexpr int VIEW_LIMIT = 200;				// �þ� ������

// ȭ�� // ���� ���� // float�� �����ؾߵǴ��� Ȯ��
constexpr int WORLD_WIDTH = 6400;		
constexpr int WORLD_HEIGHT = 6400;

constexpr int SECTOR_COL = 20;
constexpr int SECTOR_ROW = 20;

constexpr int SECTOR_COL_Length = WORLD_WIDTH / SECTOR_COL;
constexpr int SECTOR_ROW_Length = WORLD_HEIGHT/ SECTOR_ROW;


// ����Ű
constexpr char MV_UP = 0;
constexpr char MV_DOWN = 1;
constexpr char MV_LEFT = 2;
constexpr char MV_RIGHT = 3;
constexpr char MV_FRONT = 4;
constexpr char MV_BACK = 5;
constexpr char MV_IDLE = 6;

constexpr char Rotate_LBTN = 0;

// ��Ŷ �̺�Ʈ // ���� ����
constexpr char SC_PACKET_LOGIN_OK = 0;
constexpr char SC_PACKET_MOVE = 1;
constexpr char SC_PACKET_ENTER = 2;
constexpr char SC_PACKET_LEAVE = 3;
constexpr char SC_PACKET_CHAT = 4;
constexpr char SC_PACKET_LOGIN_FAIL = 5;
constexpr char SC_PACKET_STAT_CHANGE = 6;
constexpr char SC_PACKET_ATTACK = 7;
constexpr char SC_PACKET_LEVEL_UP = 8;
constexpr char SC_PACKET_ID = 9;
constexpr char SC_PACKET_MOUSE = 10;

constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;
constexpr char CS_ATTACK = 2;
constexpr char CS_CHAT = 3;
constexpr char CS_LOGOUT = 4;
constexpr char CS_TELEORT = 5;				// ���� �׽�Ʈ�� ���� �׽�Ʈ�� ���� �ڷ���Ʈ�� Hot Spot �ؼ�
constexpr char CS_ROTATE = 6;
constexpr unsigned char O_PLAYER = 0;
constexpr unsigned char O_NPC = 1;

#pragma pack (push, 1)

// ���� ����, Z�� ���� 
struct sc_packet_id
{
	// 4byte
	char size;
	char type;
	unsigned short  id;
};
struct sc_packet_login_ok {

	char size;
	char type;
	// id�����ص� �Ǵµ� �ϴ� �ΰ� ���߿� ����ȭ �Ҷ� ������ // �ٽ�Ȯ��
	unsigned short  id;
	
	//float x, y, z; �Ʒ��� �����ϸ� �����ϴµ� ���⿡ dirVector �ʿ����� ȿ�������� �����
	Vector3 localVec;

	unsigned short hp;
	unsigned short level;

	unsigned short exp;
	unsigned short iMax_exp;

	unsigned short Attack_Damage;
	// �е���Ʈ
};

struct sc_packet_move {
	char size;
	char type;
	unsigned short id;

	unsigned short move_time;
	unsigned short dir = MV_IDLE;		// pragma pack ��� ���ҰŸ� �ٽ� char�� ����

	Vector3 localVec;
	Vector3 dirVec;	//�޾ƿö� �����ص��Ǵ°�?
};

struct sc_packet_rotate {
	char size;
	char type;
	char dir;

	unsigned short id;
	unsigned short move_time;// ��¥ �ʿ��Ѱ�?

	float rotateY;
};

struct sc_packet_enter {
	char size;
	char type;
	unsigned short  id;

	Vector3 localVec;

	char name[MAX_ID_LEN];
	char o_type;
	//float x, y, z; // ���� ����
};

struct sc_packet_leave {
	char size;
	char type;
	unsigned short  id;
};

struct sc_packet_attack {
	char size;
	char type;
	unsigned short  id;

	unsigned short hp;
};

struct sc_packet_level_up {
	char size;
	char type;
	unsigned short  id;

	unsigned short hp;
	unsigned short max_hp;

	unsigned short level;
	unsigned short   exp;

	unsigned short max_exp;
	unsigned short attack_damage;
};

struct sc_packet_chat {
	char  size;
	char  type;
	unsigned short	  id;			// teller

	char  message[MAX_STR_LEN];	// ���� ���� 100�ʹ�ŭ
};

struct sc_packet_login_fail {
	char  size;
	char  type;
	unsigned short	  id;
	char  message[MAX_STR_LEN];// ���� ���� 100�ʹ�ŭ
};

struct sc_packet_stat_change {
	char size;
	char type;
	unsigned short  id;
	unsigned short hp;
	unsigned short level;
	unsigned short   exp;
};


struct cs_packet_login {
	char  size;
	char  type;
	char  name[MAX_ID_LEN];	//������� �ʹ�ŭ
};



struct cs_packet_move {
	char  size;
	char  type;		// �����Ҷ� type�� �� �ʿ����� �ۼ�
	unsigned short  direction;

	Vector3 localVec;
	
	unsigned short	  move_time;
};

struct cs_packet_rotate {
	char  size;
	char  type;
	unsigned short	  move_time;

	float rotateY;
	
	char  dir;
};

struct cs_packet_attack {
	char	size;
	char	type;
};

struct cs_packet_chat {
	char	size;
	char	type;
	char	message[MAX_STR_LEN];
};

struct cs_packet_logout {
	char	size;
	char	type;
};

struct cs_packet_teleport {
	char size;
	char type;
};

#pragma pack (pop)

