#pragma once
#include "pch.h"
#include "func.h"
//#include "KeyMgr.h"
////#include "TimeMgr.h"
////
//#include "PathMgr.h"
//
//// ��ü �ϳ� �׸���
//#include "Core.h"
//#include "Device.h"
//
//#include "Mesh.h"
//#include "shader.h"
//#include "Texture.h"
//#include "ConstantBuffer.h"
//
//#include "GameObject.h"
//#include "Transform.h"
//#include "MeshRender.h"
//
//vector<VTX> g_vecVTX;
//vector<UINT> g_vecIDX;
//
//CMesh* g_pMesh = nullptr;
//CShader* g_pShader = nullptr;
//CTexture* g_pTexture = nullptr;
//
//vector<CGameObject*> g_vecObj;
//
//void TestInit()
//{
//	g_pMesh = new CMesh;
//	g_pShader = new CShader;
//
//	VTX v;
//
//	/*v.vPos = XMFLOAT3(-0.5f, -0.5f, 0.2f);
//	v.vColor = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(-0.5f, 0.5f, 0.2f);
//	v.vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(0.5f, 0.5f, 0.2f);
//	v.vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(0.5f, -0.5f, 0.2f);
//	v.vColor = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
//	v.vColor = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
//	v.vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(0.5f, 0.5f, 0.5f);
//	v.vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(0.5f, -0.5f, 0.5f);
//	v.vColor = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	g_vecIDX.push_back(0); g_vecIDX.push_back(1); g_vecIDX.push_back(2);
//	g_vecIDX.push_back(0); g_vecIDX.push_back(2); g_vecIDX.push_back(3);
//
//	g_vecIDX.push_back(4); g_vecIDX.push_back(6); g_vecIDX.push_back(5);
//	g_vecIDX.push_back(4); g_vecIDX.push_back(7); g_vecIDX.push_back(6);
//
//	g_vecIDX.push_back(4); g_vecIDX.push_back(5); g_vecIDX.push_back(1);
//	g_vecIDX.push_back(4); g_vecIDX.push_back(1); g_vecIDX.push_back(0);
//
//	g_vecIDX.push_back(3); g_vecIDX.push_back(2); g_vecIDX.push_back(6);
//	g_vecIDX.push_back(3); g_vecIDX.push_back(6); g_vecIDX.push_back(7);
//
//	g_vecIDX.push_back(1); g_vecIDX.push_back(5); g_vecIDX.push_back(6);
//	g_vecIDX.push_back(1); g_vecIDX.push_back(6); g_vecIDX.push_back(2);
//
//	g_vecIDX.push_back(4); g_vecIDX.push_back(0); g_vecIDX.push_back(3);
//	g_vecIDX.push_back(4); g_vecIDX.push_back(3); g_vecIDX.push_back(7);*/
//
//	//-------------------------
//	
//	// 1. �Է� ������ �ܰ迡 ������, ���� 3���� ������ �ﰢ�� 1��
//	v.vPos = XMFLOAT3(-0.5f, 0.5f, 0.5f);
//	v.vColor = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
//	v.vUV = XMFLOAT2(0.f, 0.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(0.5f, 0.5f, 0.5f);
//	v.vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
//	v.vUV = XMFLOAT2(1.f, 0.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(0.5f, -0.5f, 0.5f);
//	v.vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);
//	v.vUV = XMFLOAT2(1.f, 1.f);
//	g_vecVTX.push_back(v);
//
//	v.vPos = XMFLOAT3(-0.5f, -0.5f, 0.5f);
//	v.vColor = XMFLOAT4(0.f, 0.f, 1.f, 1.f);
//	v.vUV = XMFLOAT2(0.f, 1.f);
//	g_vecVTX.push_back(v);
//	
//	g_vecIDX.push_back(0); g_vecIDX.push_back(1); g_vecIDX.push_back(2);
//	g_vecIDX.push_back(0); g_vecIDX.push_back(2); g_vecIDX.push_back(3);
//
//	g_pMesh->Create(sizeof(VTX), g_vecVTX.size(), (BYTE*)g_vecVTX.data()
//		, DXGI_FORMAT_R32_UINT, g_vecIDX.size(), (BYTE*)g_vecIDX.data());
//
//
//	// ���̴� ����
//	g_pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Test", "vs_5_1");
//	g_pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Test", "ps_5_1");
//	g_pShader->Create();
//
//	// Texture �ε�
//	wstring strPath = CPathMgr::GetResPath();
//	strPath += L"Texture\\cookie.png";
//
//	g_pTexture = new CTexture;
//	g_pTexture->Load(strPath);
//
//	CDevice::GetInst()->FlushCommandQueue();
//
//	//  GameObject �����
//	CGameObject* pObject = nullptr;
//	{
//		pObject = new CGameObject;
//		pObject->AddComponent(new CTransform);
//		pObject->AddComponent(new CMeshRender);
//
//		pObject->MeshRender()->SetMesh(g_pMesh);
//		//pObject->MeshRender()->SetShader(g_pShader);
//
//		g_vecObj.push_back(pObject);
//
//		/*pObject = new CGameObject;
//		pObject->AddComponent(new CTransform);
//		pObject->AddComponent(new CMeshRender);
//
//		pObject->MeshRender()->SetMesh(g_pMesh);
//		pObject->MeshRender()->SetShader(g_pShader);
//
//		g_vecObj.push_back(pObject);*/
//	}
//}
//
//void TestUpdate()
//{
//	g_vecObj[0]->update();
//	g_vecObj[0]->lateupdate();
//	g_vecObj[0]->finalupdate();
//}
//
//void TestRender()
//{
//	//// �׸��� �غ�
//	//float arrColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
//
//	//CDevice::GetInst()->render_start(arrColor);
//
//	//g_pShader->UpdateData();
//
//	//XMFLOAT4 vData = XMFLOAT4{ 0.f, 0.f, 0.f, 0.f };
//
//	//CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);
//	//pCB->SetData(&vData, sizeof(XMFLOAT4), 0);
//	//CDevice::GetInst()->SetConstBufferToRegister(pCB, 0);
//
//	//g_pMesh->render();
//
//	///*vData = XMFLOAT4{ -0.5f, 0.f, 0.f, 0.f };
//
//	//pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);
//	//pCB->SetData(&vData, sizeof(XMFLOAT4), 1);
//	//CDevice::GetInst()->SetConstBufferToRegister(pCB, 1);
//
//	//g_pMesh->render();*/
//
//	//// �׸��� ����
//	//CDevice::GetInst()->render_present();
//
//	// ----------------
//
//	// �׸��� �غ�
//	float arrColor[4] = { 0.0f, 0.0f, 0.0f, 0.f };
//
//	CDevice::GetInst()->render_start(arrColor);
//
//	CDevice::GetInst()->SetTextureToRegister(g_pTexture, TEXTURE_REGISTER::t0);
//	for (size_t i = 0; i < g_vecObj.size(); ++i)
//	{
//		g_vecObj[i]->render();
//	}
//
//	// �׸��� ����
//	CDevice::GetInst()->render_present();
//}
//
//void TestRelease()
//{
//	SAFE_DELETE(g_pMesh);
//	SAFE_DELETE(g_pShader);
//}
//
int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}

void SaveWString(FILE* _pFile, const wstring& _str)
{
	BYTE c = (BYTE)_str.length();
	fwrite(&c, 1, 1, _pFile);
	fwrite(_str.c_str(), 2, c, _pFile);
}

wchar_t* LoadWString(FILE* _pFile)
{
	static wchar_t szStr[255] = {};

	BYTE c = 0;
	fread(&c, 1, 1, _pFile);
	fread(szStr, 2, c, _pFile);
	szStr[c] = 0;

	return szStr;
}

Matrix GetMatrix(FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat.m[i][j] = (float)_mat.Get(i, j);
		}
	}
	return mat;
}