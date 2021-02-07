#pragma once
#include "Entity.h"


class CConstantBuffer :
	public CEntity
{
private:
	ComPtr<ID3D12Resource>			m_pBuffer;		// ���� �������(���ҽ�)
	ComPtr<ID3D12DescriptorHeap>	m_pCBV;			// ������� ������ ��(View ����)
	D3D12_CPU_DESCRIPTOR_HANDLE		m_hCBV;			// ������ �� �ڵ�
	UINT							m_iBufferSize;  // ������� ���� ũ��
	UINT							m_iMaxCount;	// ������� �ִ� ����
	UINT							m_iCurCount;	// ������ۿ� Add �� ���� ������ ����
	CONST_REGISTER					m_eRegisterNum; // ������� ���� �������� ��ȣ	
	BYTE*							m_pData;		// ������ۿ� ���ε� �ּ�

	D3D12_DESCRIPTOR_HEAP_DESC		m_tHeapDesc;	// ������ۿ� ������ ���� �����ϴ� ����
	
public:
	void Create(UINT _iBufferSize, UINT _iMaxCount, CONST_REGISTER _eRegisterNum);
	//void SetData(void* _pSrc){memcpy(m_pData, _pSrc, m_iBufferSize);}
	//void SetData(void* _pSrc, UINT _iSize) { memcpy(m_pData, _pSrc, _iSize); }
	//void SetData(void* _pSrc, UINT _iSize, UINT _iOffsetPos);
	UINT AddData(void* _pSrc);
	void Clear() { m_iCurCount = 0; }

	ComPtr<ID3D12DescriptorHeap> GetCBV() { return m_pCBV; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() {return m_hCBV;}
	CONST_REGISTER GetRegisterNum() { return m_eRegisterNum; }
	
public:
	CConstantBuffer();
	virtual ~CConstantBuffer();
};

