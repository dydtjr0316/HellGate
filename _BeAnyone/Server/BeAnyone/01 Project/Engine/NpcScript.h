#pragma once
#include "Script.h"

enum class REQUEST_STATE
{
	NOT_RECIEVE,
	REQUESTING,
	REQUEST_RESOLUTION,
	END,
};

//enum class REQUEST_STATE
//{
//	NOT_RECIEVE,
//	REQUESTING,
//	REQUEST_RESOLUTION,
//	END,
//};

class CNpcScript :
	public CScript
{
private:
	NPC_TYPE m_eNpcType;

	// Request
	CGameObject*		m_pRequestMark;
	REQUEST_STATE		m_eReqState;
	vector<Ptr<CMesh>>	m_ReqMarkMeshData;
	vector<Ptr<CMaterial>> m_ReqMarkMtrlData;

	// Conversation Box
	CGameObject*		m_pConversationBox;
	bool				m_bIsTalk = false;
	bool				m_bIsCollision = false;
	int					m_iClickNum{};

	// animation
	vector<Ptr<CMesh>>   m_pAniData;
	bool m_bisAniReset = false;

public:
	virtual void update();

	void Move();

	// animation
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
	void SetAnimation(const NPC_ANI_TYPE& type);
	void AnimClipReset();

	// Request Mar
	void SetReqMarkData(Ptr<CMesh> _meshData, Ptr<CMaterial> _mtrlData) { m_ReqMarkMeshData.push_back(_meshData); m_ReqMarkMtrlData.push_back(_mtrlData); }
	//void SetReqMarkMtrlData(vector<Ptr<CMaterial>> _mtrlData) { m_ReqMarkMtrlData.push_back(_mtrlData) }
	void SetReqMarkMesh(const REQUEST_STATE& _eType);
	
	// collision
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

	// camera
	void SetCameraState(CAMERA_STATE _eCamState);

public:
	CLONE(CNpcScript);

public:
	CNpcScript();
	virtual ~CNpcScript();
};

