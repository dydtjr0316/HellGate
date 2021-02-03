#pragma once
#include "Entity.h"

class CConstantBuffer :
	public CEntity
{
private:
	ComPtr<ID3D12Resource>			m_pBuffer;			// 실제 상수 버퍼(리소스)
	ComPtr<ID3D12DescriptorHeap>	m_pCbvHeap;			// 상수 버퍼 설명자 힙(View 역할) 
	D3D12_CPU_DESCRIPTOR_HANDLE		m_hCbvHeapHandle;	// 서술자 힙 핸들
	UINT							m_iBufferSize;		// 상수 버퍼 크기
	UINT							m_iMaxCount;		// 상수 버퍼 최대 개수
	CONST_REGISTER					m_eRegisterNum;		// 상수 버퍼 대응 레지스터 번호
	BYTE*							m_pData;			// 상수 버퍼와 맵핑된 주소

	D3D12_DESCRIPTOR_HEAP_DESC		m_tHeapDesc;		// 상수 버퍼와 서술자 힙을 연결하는 정보

public:
	void Create(UINT _iBufferSize, UINT _iMaxCount, CONST_REGISTER _eRegisterNum);
	void SetData(void* _pSrc) { memcpy(m_pData, _pSrc, m_iBufferSize); }
	void SetData(void* _pSrc, UINT _iSize) { memcpy(m_pData, _pSrc, _iSize); }
	void SetData(void* _pSrc, UINT _iSize, UINT _iOffsetPos) { memcpy(m_pData + (m_iBufferSize * _iOffsetPos), _pSrc, _iSize); }

	ComPtr<ID3D12DescriptorHeap> GetCBV() { return m_pCbvHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return m_hCbvHeapHandle; }
	UINT GetRegisterNum() { return (UINT)m_eRegisterNum; }

public:
	CConstantBuffer();
	virtual ~CConstantBuffer();
};

