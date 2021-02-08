#pragma once
#include "Entity.h"

class CConstantBuffer :
	public CEntity
{
private:
	ComPtr<ID3D12Resource>			m_pBuffer;			// ���� ��� ����(���ҽ�)
	ComPtr<ID3D12DescriptorHeap>	m_pCbvHeap;			// ��� ���� ������ ��(View ����) 
	D3D12_CPU_DESCRIPTOR_HANDLE		m_hCbvHeapHandle;	// ������ �� �ڵ�
	UINT							m_iBufferSize;		// ��� ���� ũ��
	UINT							m_iMaxCount;		// ��� ���� �ִ� ����
	CONST_REGISTER					m_eRegisterNum;		// ��� ���� ���� �������� ��ȣ
	BYTE*							m_pData;			// ��� ���ۿ� ���ε� �ּ�

	D3D12_DESCRIPTOR_HEAP_DESC		m_tHeapDesc;		// ��� ���ۿ� ������ ���� �����ϴ� ����

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

