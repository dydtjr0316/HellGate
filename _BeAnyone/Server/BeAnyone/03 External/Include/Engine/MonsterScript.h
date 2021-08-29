#pragma once
#include "Script.h"

enum class MONSTER_STATE {
    MOVE,
    FIND,
    FOLLOW,
    ATTACK,
    DAMAGE,
    DIE,
    END,
};

enum class BOSS_ATTACK {
    BITE_ATTACK,
    LEFT_ATTACK,
    RIGHT_ATTACK,
    END,
};

class CMonsterScript :
    public CScript
{
private:
    MONSTER_TYPE m_eMonsterType;    // server에서 어떻게 쓰이고 있는지 모르겠음 없애도 되나
    MOB_TYPE     m_eMobType;
    float m_fSpeed = 2.0f;

    // ui
    CGameObject* m_pUi;
    CGameObject* m_pUnderUi;
    CGameObject* m_pChildDummy;
    bool m_bSetChild = false;

    CTerrain* m_pTerrainObj;
    vector<Ptr<CMesh>>   m_pAniData;

    bool m_packetDead = false;
    bool m_bisAttack = false;
    bool m_bisAniReset = false;
    bool m_bisDamaged = false;
    bool m_bisPunch = false;
    bool m_bisMoving = false;
    bool m_bisDirChange = false;
    // player save for quest
    CGameObject* m_pPlayer = nullptr;

    sc_packet_monster_automove* m_Packet_autoMove = nullptr;

    // 용석
    unsigned short m_sId;
    float m_fAnimationCnt[(UINT)MONSTER_ANICNT_TYPE::END] = {};
    uShort m_sHp;
    uShort m_sMaxHP = 100;
    MONSTER_AUTOMOVE_DIR m_eDir = MONSTER_AUTOMOVE_DIR::IDLE;
    bool m_isfirst = true;

    // monster move dir
    float      m_fAngleY = 0.f;

    // boss monster
    MONSTER_STATE       m_eMonsterState;
    BOSS_ATTACK         m_eAttackPattern;
    CGameObject*        m_pFindCollider;
    bool                m_bIsFindPlayer = false;
    bool                m_bIsNearPlayer = false;
    float               m_fFollowTime = 0.f;

    bool                m_bIsRoar = false;
    bool                m_bIsAttakLeft = false;
    bool                m_bIsAttakRight = false;
  
public:
    void Init();
    virtual void update();
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    float GetSpeed() { return m_fSpeed; }
    void SetID(const uShort& id) { m_sId = id; }
    uShort GetID() { return m_sId; }
    void SetHP(const uShort& hp) { m_sHp = hp; }
    uShort GetHP() { return m_sHp; }
    void SetBisAttack(const bool& isattack) { m_bisAttack = isattack; }
    bool GetBisAttack() { return m_bisAttack; }

    void Setcnt(const float& cnt, MONSTER_ANICNT_TYPE _cntType) { m_fAnimationCnt[(UINT)_cntType] = cnt; }
    float Getcnt(MONSTER_ANICNT_TYPE _cntType) { return  m_fAnimationCnt[(UINT)_cntType]; }

    //animatioin
    void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
    void SetAnimation(const MONSTER_ANI_TYPE& type);
    void CMonsterScript::SetAnimation(const uShort& other_id, const MONSTER_ANI_TYPE& type);
    void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
    void AnimClipReset();
    void SetAniReset(bool _aniReset) { m_bisAniReset = _aniReset; }

    // damage 
    void SetIsDamage(bool _isDamage) { m_bisDamaged = _isDamage; }
    bool GetIsDamage() { return m_bisDamaged; }
    void SetIsPunch(bool _isPunched) { m_bisPunch = _isPunched; }
    bool GetIsPunch() { return m_bisPunch; }

    // monster type
    void SetMonsterType(MONSTER_TYPE _eMonsterType) { m_eMonsterType = _eMonsterType; }// 둘 중에 하나 없애야 함 
    MONSTER_TYPE GetMonsterType() { return m_eMonsterType; }                           // 둘 중에 하나 없애야 함 
    void SetMobTYpe(MOB_TYPE _eMobType) { m_eMobType = _eMobType; }
    MOB_TYPE GetMobTYpe() { return m_eMobType; }

    CTerrain* GetTerrain() { return this->m_pTerrainObj; }

    void DecreaseHp();
    void Move();
    void BossTurn();
    void Attack();
    void TurnToPlayer(MOB_TYPE _eType);

    // void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }
    MONSTER_AUTOMOVE_DIR& GetDir() { return m_eDir; }
    void SetDir(const MONSTER_AUTOMOVE_DIR dir) { m_eDir = dir; }

    sc_packet_monster_automove* GetPacketMove() { return  m_Packet_autoMove; }
    void SetPacketMove(sc_packet_monster_automove* p);


    void SetisMoving(const bool moving) { m_bisMoving = moving; }
    bool GetisMoving() { return m_bisMoving; }

    void SetisDirChange(const bool moving) { m_bisDirChange = moving; }
    bool GetisDirChange() { return m_bisDirChange; }

    void  SetPlayer(CGameObject* p) {
        m_pPlayer = p;
    };
    void SetPacketDead(const bool& dead) { m_packetDead = dead; }

    // boss monster
    void SetIsFindPlayer(bool _bool) { m_bIsFindPlayer = _bool; }
    void SetIsNearPlayer(bool _bool) { m_bIsNearPlayer = _bool; }
    void FollowToPlayer();
    void ChecktoAttack();   // 사정거리 안에 들어오는지 확인
    void ChooseAttackPattern();    // 공격 패턴 고르기

    MONSTER_STATE GetMonsterState() { return m_eMonsterState; }
public:
    void Attack_Default();

public:
    CLONE(CMonsterScript);

public:
    CMonsterScript();
    virtual ~CMonsterScript();
};
