#pragma once
#pragma once

// 섹터 디파인 값
constexpr float MOVE_SPEED = 5.f; // 이동패킷 구조변경 성공시 삭제

// 추후  수정
constexpr int NAME_LEN = 50;
constexpr int PHONE_LEN = 60;
//
  
// constexpr
constexpr auto MAX_PACKET_SIZE = 255;
constexpr auto MAX_BUF_SIZE = 1024;

// 서버 포트
constexpr int SERVER_PORT = 3389;
constexpr int MAX_ID_LEN = 10;
constexpr int MAX_STR_LEN = 100;

// 동접, 몬스터 ,  관리
constexpr int MAX_USER = 1000;
constexpr int END_USER = MAX_USER;

constexpr int START_MONSTER = MAX_USER;
constexpr int MAX_MONSTER = 2;
constexpr int END_MONSTER = START_MONSTER + MAX_MONSTER;
constexpr int DIVIDE_MONNSTER = MAX_MONSTER / 4;

constexpr int START_NPC = END_MONSTER;
constexpr int MAX_NPC = 2000;
constexpr int END_NPC = START_NPC + MAX_NPC;

constexpr int WORLD_WIDTH = 12800;
constexpr int WORLD_HEIGHT = 12800;

constexpr int VIEW_LIMIT =  1280;				// 시야 반지름

// 화면 // 추후 수정 // float로 수정해야되는지 확인


constexpr int SECTOR_COL = 5;
constexpr int SECTOR_ROW = 5;

constexpr int SECTOR_COL_Length = WORLD_WIDTH / SECTOR_COL;
constexpr int SECTOR_ROW_Length = WORLD_HEIGHT/ SECTOR_ROW;


// 방향키
constexpr char MV_UP = 0;
constexpr char MV_DOWN = 1;
constexpr char MV_LEFT = 2;
constexpr char MV_RIGHT = 3;
constexpr char MV_FRONT = 4;
constexpr char MV_BACK = 5;
constexpr char MV_IDLE = 6;

constexpr char ATTACK_IDLE = 7;
constexpr char ATTACK_ANI = 8;



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
constexpr char SC_PACKET_ID = 9;
constexpr char SC_PACKET_MOUSE = 10;
constexpr char SC_PACKET_STOP = 11;

//가라
constexpr char SC_PACKET_ATTACKANI = 12;

constexpr char CS_LOGIN = 0;
constexpr char CS_MOVE = 1;
constexpr char CS_ATTACK = 2;
constexpr char CS_CHAT = 3;
constexpr char CS_LOGOUT = 4;
constexpr char CS_TELEORT = 5;				// 부하 테스트용 동접 테스트를 위해 텔러포트로 Hot Spot 해소
constexpr char CS_STOP = 6;
constexpr char CS_MONSTER_DEAD = 7;
constexpr char CS_ATTACK_ANIMATION = 8;


constexpr unsigned char O_PLAYER = 0;
constexpr unsigned char O_NPC = 1;

#pragma pack (push, 1)

// 추후 수정, Z값 포함 
struct sc_packet_id
{
	// 4byte
	char size;
	char type;
	uShort  id;
};
struct sc_packet_login_ok {
	
	char size;
	char type;
	// id제거해도 되는데 일단 두고 나중에 최적화 할때 지우자 // 다시확인
	uShort  id;
	
	//float x, y, z; 아래꺼 성공하면 삭제하는데 여기에 dirVector 필요한지 효림이한테 물어보기
	Vector3 localVec;

	uShort hp;
	uShort level;

	uShort exp;
	uShort iMax_exp;

	uShort Attack_Damage;
	// 패딩비트
};

struct sc_packet_move {
	char size;
	char type;
	uShort id;

	uShort move_time;
	uShort dir = MV_IDLE;		// pragma pack 사용 안할거면 다시 char로 수정

	Vector3 localVec;
	Vector3 dirVec;	//받아올땐 삭제해도되는가?

	float speed;
	float rotateY;
	system_clock::time_point Start;

	bool isMoving;

};

//가라
struct sc_packet_AttackAni {
	char size;
	char type;
	uShort id;
	bool isAttack;
};


struct sc_packet_stop {
	char size;
	char type;
	uShort id;
	bool isMoving;
};


struct sc_packet_enter {
	char size;
	char type;
	uShort  id;

	Vector3 localVec;

	float RotateY;

	uShort hp;
	char name[MAX_ID_LEN];
	char o_type;
	//float x, y, z; // 보고 삭제
};

struct sc_packet_leave {
	char size;
	char type;
	uShort  id;

	bool isAttack;
};

struct sc_packet_attack {
	char size;
	char type;
	uShort  id;

	uShort hp;
};

struct sc_packet_level_up {
	char size;
	char type;
	uShort  id;

	uShort hp;
	uShort max_hp;

	uShort level;
	uShort   exp;

	uShort max_exp;
	uShort attack_damage;
};

struct sc_packet_chat {
	char  size;
	char  type;
	uShort	  id;			// teller

	char  message[MAX_STR_LEN];	// 여기 수정 100너무큼
};

struct sc_packet_login_fail {
	char  size;
	char  type;
	uShort	  id;
	char  message[MAX_STR_LEN];// 여기 수정 100너무큼
};

struct sc_packet_stat_change {
	char size;
	char type;
	uShort  id;
	uShort hp;
	uShort level;
	uShort   exp;
};


struct cs_packet_login {
	char  size;
	char  type;
	char  name[MAX_ID_LEN];	//여기수정 너무큼
};



struct cs_packet_move {
	char  size;
	char  type;		// 정리할때 type이 꼭 필요한지 작성
	char  dir;

	float rotateY;

	Vector3 localVec;
	Vector3 DirVec;
	float speed;
	float deltaTime;

	system_clock::time_point Start;
	
	uShort	  move_time;
	bool isMoving;
};

struct cs_packet_stop {
	char size;
	char type;
	uShort id;
	bool isMoving;
};

struct cs_packet_attack {
	char	size;
	char	type;
	uShort  hp;
	uShort  id;
};

struct cs_packet_MonsterDead {
	char	size;
	char	type;
	uShort  id;
};

struct cs_packet_AttackAni {
	char	size;
	char	type;
	uShort  id;

	bool isAttack;
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

