#pragma once

// 확정
enum class OBJECT_TYPE { CLIENT, MONSTER, NPC };

enum class MONSTER_TYPE { MONSTER1, MONSTER2, MONSTER3, MONSTER4, BOSS_MONSTER, DEFAULT };

enum class MONSTER_AUTOMOVE_DIR { FRONT, BACK, LEFT, RIGHT, AUTO, IDLE };

enum class KEYINPUT { UP, DOWN, LEFT, RIGHT, JUMP, /* + 조작키 */ };

enum class PLAYER_TYPE { WARRIOR, SNIPER };	// 캐릭터 타입 근접 : 원거리

enum class SECTOR_SEARCH{ DEFAULT, UP, DOWN, LEFT, RIGHT, JUMP };

// 수정 후 삭제
enum OBJSTATUS { ST_FREE, ST_ALLOC, ST_ACTIVE, ST_SLEEP };

// 미확정
enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT, OP_RAMDON_MOVE_MONSTER, OP_RAMDON_MOVE_NPC, OP_NOTIFY, OP_NPC };