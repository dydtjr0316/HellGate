#pragma once
#include "Script.h"

enum class REQUEST_STATE
{
	NOT_RECIEVE,
	REQUESTING,
	REQUEST_RESOLUTION,
	END,
};

class CNpcScript :
	public CScript
{
private:
	NPC_TYPE m_eNpcType;

	// Request
	CGameObject*		m_pRequestMark;
	REQUEST_STATE		m_eReqState;
	vector<Ptr<CMesh>>	m_ReqMarkMeshData;
	vector<vector<Ptr<CMaterial>>> m_ReqMarkMtrlData;

	// Conversation Box
	CGameObject*		m_pConversationBox;
	bool				m_bIsTalk = false;
	bool				m_bIsCollision = false;

	// animation
	vector<Ptr<CMesh>>   m_pAniData;
	bool m_bisAniReset = false;

public:
	virtual void update();

	// animation
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }
	void SetAnimation(const NPC_ANI_TYPE& type);
	void AnimClipReset();

	// Request Mar
	void SetReqMarkData(Ptr<CMesh> _meshData) { m_ReqMarkMeshData.push_back(_meshData); }
	void SetReqMarkMesh(const REQUEST_STATE& _eType);

	// collision
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

	// camera
	void RotateCamera();

public:
	CLONE(CNpcScript);

public:
	CNpcScript();
	virtual ~CNpcScript();
};

