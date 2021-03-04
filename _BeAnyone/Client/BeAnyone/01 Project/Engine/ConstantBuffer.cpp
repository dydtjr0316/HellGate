#include "pch.h"
#include "ConstantBuffer.h"

#include "Device.h"

CConstantBuffer::CConstantBuffer()
	: m_pBuffer(nullptr)
	, m_pCbvHeap(nullptr)
	, m_iBufferSize(0)
	, m_iMaxCount(2)
	, m_eRegisterNum(CONST_REGISTER::END)
	, m_pData(nullptr)
	, m_tHeapDesc{}
	, m_iCurCount(0)
{
}

CConstantBuffer::~CConstantBuffer()
{
}

void CConstantBuffer::Create(UINT _iBufferSize, UINT _iMaxCount, CONST_REGISTER _eRegisterNum)
{
	m_iBufferSize = (_iBufferSize + 255) & ~255;	// ���� ������� 256 ����Ʈ �������� �Ѵ�.
	m_iMaxCount = _iMaxCount;
	m_eRegisterNum = _eRegisterNum;

	// ���� ��� ���۸� GPU �޸𸮿� �Ҵ�
	D3D12_HEAP_PROPERTIES properties = {};
	properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	properties.CreationNodeMask = 1;
	properties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resource = {};
	resource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource.Alignment = 0;
	resource.Width = m_iBufferSize * m_iMaxCount;
	resource.Height = 1;
	resource.DepthOrArraySize = 1;
	resource.MipLevels = 1;

	resource.Format = DXGI_FORMAT_UNKNOWN;
	
	resource.SampleDesc.Count = 1;
	resource.SampleDesc.Quality = 0;
	resource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource.Flags = D3D12_RESOURCE_FLAG_NONE;

	DEVICE->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&resource,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pBuffer));

	// DescriptorHeap�� ���� (View�ν�, ��� ���ۿ� ����� ������)
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = m_iMaxCount;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pCbvHeap));

	// DescriptorHeap�� Buffer�� ������
	m_hCbvHeapHandle = m_pCbvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hCbvHeapHandle;
	UINT mCbvSrvUavDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int i = 0; i < m_iMaxCount; ++i) {
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_pBuffer->GetGPUVirtualAddress() + (m_iBufferSize * i);
		cbvDesc.SizeInBytes = m_iBufferSize;	// CB size is required to be 256-byte aligned.
		DEVICE->CreateConstantBufferView(&cbvDesc, handle);
		handle.ptr += mCbvSrvUavDescriptorSize;
	}

	// ��� ���ۿ� �����ϱ� ���ؼ� ������ �صд� 
	// 21.01.31 Q. ������ �� �־��µ� ,, ���߿� �ֳ�?
	// 21.02.01 A. ���߿� setdata �� �� ������ ���� ���۶� ���εǾ� �ִ� pData��
	D3D12_RANGE readRange{ 0, 0 };
	m_pBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pData));
}

UINT CConstantBuffer::AddData(void* _pSrc)
{
	// ������ ũ���� ������۸� �Ѿ�� �����Ͱ� ������ ���
	assert(!(m_iCurCount >= m_iMaxCount));

	UINT iOffsetPos = m_iCurCount++;

	memcpy(m_pData + (m_iBufferSize * iOffsetPos), _pSrc, m_iBufferSize);

	return iOffsetPos;
}