#pragma once
#include "Component.h"


class CQuest
	: public CComponent
{
private:
	CGameObject*		m_pQuestBox;

	bool				m_bBoxRender = false;
	// quest »Æ¿Œ
	vector<int>			m_vQuestCheck;
	bool				m_bDoQuest[(UINT)QUEST_TYPE::END];


public:
	virtual void update();
	virtual void lateupdate();
	virtual void finalupdate();

	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

	void AddQuestcount(QUEST_TYPE _questType) { m_vQuestCheck[(UINT)_questType]++; }
	void ResetQuestCheck(QUEST_TYPE _questType) { m_vQuestCheck[(UINT)_questType] = 0; }

	vector<int>& GetQuestCheck() { return m_vQuestCheck; }
	bool GetDoQuest(QUEST_TYPE _questType) { return m_bDoQuest[(UINT)_questType]; }

	void SetDoQuest(QUEST_TYPE _questType, bool _doQuest) { m_bDoQuest[(UINT)_questType] = _doQuest; }
	void SetQuestcount(QUEST_TYPE _questType, int _questCount) { m_vQuestCheck[(UINT)_questType] = _questCount; }
public:
	CLONE(CQuest);

public:
	CQuest();
	virtual ~CQuest();
};

