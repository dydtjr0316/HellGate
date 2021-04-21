#pragma once
#include "Entity.h"

class CResMgr;
class CPathMgr;

class CResource :
	public CEntity
{
private:
	wstring		m_strPath;	// �����
	int			m_iRefCount;
	RES_TYPE	m_eType;

	UINT		m_iCloneCount; // ���ҽ� ���� ȸ��

public:
	void SetPath( const wstring& _strPath ) { m_strPath = _strPath; }
	const wstring& GetPath() { return m_strPath; }

	RES_TYPE GetType() { return m_eType; }
	UINT GetCloneCount() { return m_iCloneCount; }

public:
	void AddRef() { m_iRefCount++; }
	void SubRef() { m_iRefCount--; }

public:
	virtual void Load( const wstring& _strPath/*���� ���*/ ) {};
	virtual void Save( const wstring& _strPath/*��� ���*/ ) {};

public:
	virtual CResource* Clone() { return nullptr; }
	void SaveToScene( FILE* _pFile );

	template<typename T>
	bool LoadFromScene( FILE* _pFile ); // return false : �̹� ���ҽ��� �ε��� �Ǿ��ִ� ���

public:
	CResource( RES_TYPE _eType );
	CResource( const CResource& _res );
	virtual ~CResource();

	template<typename T>
	friend class Ptr;
};


template<typename T>
inline bool CResource::LoadFromScene( FILE* _pFile ) {
	// key
	wstring strName = LoadWString( _pFile );

	// path
	m_strPath = LoadWString( _pFile );

	// ���ҽ��� �̹� �ε��Ǿ��ִ��� üũ
	Ptr<T> pRes = CResMgr::GetInst()->FindRes<T>( strName );
	if (nullptr != pRes) // �̹� �ε��Ǿ��ִ�.
		return false;

	SetName( strName );

	wstring strPath = CPathMgr::GetResPath();
	strPath += m_strPath;
	Load( strPath );
}