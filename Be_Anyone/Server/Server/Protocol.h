#pragma once
#pragma once

// 섹터 디파인 값


// 추후  수정
constexpr int NAME_LEN = 50;
constexpr int PHONE_LEN = 60;
//
  
// constexpr
constexpr auto MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;

// 서버 포트
constexpr int SERVER_PORT = 3500;
constexpr int MAX_ID_LEN = 10;
constexpr int MAX_STR_LEN = 100;

// 동접, 몬스터 ,  관리
constexpr int MAX_USER = 1000;
constexpr int END_USER = MAX_USER;

constexpr int START_MONSTER = MAX_USER;
constexpr int MAX_MONSTER = 3000;
constexpr int END_MONSTER = START_MONSTER + MAX_MONSTER;
constexpr int DIVIDE_MONNSTER = MAX_MONSTER / 4;


constexpr int START_NPC = END_MONSTER;
constexpr int MAX_NPC = 100;
constexpr int END_NPC = START_NPC + MAX_NPC;

constexpr int VIEW_LIMIT = 7;				// 시야 반지름

// 화면 // 추후 수정
constexpr int WORLD_WIDTH = 800;
constexpr int WORLD_HEIGHT = 800;

constexpr int SECTOR_COL = 20;
constexpr int SECTOR_ROW = 20;

constexpr int SECTOR_COL_Length = WORLD_WIDTH / SECTOR_COL;
constexpr int SECTOR_ROW_Length = WORLD_HEIGHT/ SECTOR_ROW;

#pragma pack (push, 1)

// 방향키
constexpr char MV_UP = 0;
constexpr char MV_DOWN = 1;
constexpr char MV_LEFT = 2;
constexpr char MV_RIGHT = 3;

// 패킷 이벤트 // 추후 수정
constexpr char SC_PACKET_LOGIN_OK = 0;
constexpr char SC_PACKET_MOVE = 1;
constexpr char SC_PACKET_ENTER = 2;
constexpr char SC_PACKET_LEAVE = 3;
constexpr char SC_PACKET_CHAT = 4;
constexpr char SC_PACKET_LOGIN_FAIL = 5;
constexpr char SC_PACKET_STAT_CHANGE = 6;
constexpr char SC_PACKET_ATTACK = 7;
constexpr char SC_PACKET_LEVEL_UP = 8;

constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;
constexpr char CS_ATTACK = 2;
constexpr char CS_CHAT = 3;
constexpr char CS_LOGOUT = 4;
constexpr char CS_TELEORT = 5;				// 부하 테스트용 동접 테스트를 위해 텔러포트로 Hot Spot 해소

constexpr unsigned char O_PLAYER = 0;
constexpr unsigned char O_NPC = 1;


// 추후 수정, Z값 포함 
struct sc_packet_login_ok {
	char size;
	char type;

	int  id;
	int x, y, z;
	int hp;
	int level;
	int   exp;

	int	  iMax_exp;
	int Attack_Damage;
};

struct sc_packet_move {
	char size;
	char type;
	int id;
	int x, y, z;
	int move_time;
};

struct sc_packet_enter {
	char size;
	char type;
	int  id;
	char name[MAX_ID_LEN];
	char o_type;
	int x, y;
};

struct sc_packet_leave {
	char size;
	char type;
	int  id;
};

struct sc_packet_attack {
	char size;
	char type;
	int  id;

	int hp;
};

struct sc_packet_level_up {
	char size;
	char type;
	int  id;

	int hp;
	int max_hp;
	int level;
	int   exp;
	int max_exp;
	int attack_damage;
};

struct sc_packet_chat {
	char  size;
	char  type;
	int	  id;			// teller
	char  message[MAX_STR_LEN];
};

struct sc_packet_login_fail {
	char  size;
	char  type;
	int	  id;
	char  message[MAX_STR_LEN];
};

struct sc_packet_stat_change {
	char size;
	char type;
	int  id;
	int hp;
	int level;
	int   exp;
};


struct cs_packet_login {
	char  size;
	char  type;
	char  name[MAX_ID_LEN];
};



struct cs_packet_move {
	char  size;
	char  type;
	char  direction;
	int	  move_time;
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
	int x, y;
};

#pragma pack (pop)

