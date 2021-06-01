#pragma once
#include "pch.h"
#include "func.h"
#include "global.h"
#include "Ptr.h"
#include "Mesh.h"
#include "ResMgr.h"
//#include "KeyMgr.h"
////#include "TimeMgr.h"
////
//#include "PathMgr.h"
//
//// 물체 하나 그리기
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
//	// 1. 입력 조립기 단계에 전달할, 정점 3개로 구성된 삼각형 1개
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
//	// 쉐이더 생성
//	g_pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Test", "vs_5_1");
//	g_pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Test", "ps_5_1");
//	g_pShader->Create();
//
//	// Texture 로드
//	wstring strPath = CPathMgr::GetResPath();
//	strPath += L"Texture\\cookie.png";
//
//	g_pTexture = new CTexture;
//	g_pTexture->Load(strPath);
//
//	CDevice::GetInst()->FlushCommandQueue();
//
//	//  GameObject 만들기
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
//	//// 그리기 준비
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
//	//// 그리기 종료
//	//CDevice::GetInst()->render_present();
//
//	// ----------------
//
//	// 그리기 준비
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
//	// 그리기 종료
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

void CreateBoundingSphereCollisionMesh(float _fRadius, wstring _fName)
{
	// 바운딩 구 콜리젼 매쉬 등록
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;
	VTX v;

	float fRadius = _fRadius;

	// Top
	v.vPos = Vector3(0.f, fRadius, 0.f);
	v.vUV = Vector2(0.5f, 0.f);
	v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vector3(1.f, 0.f, 0.f);
	v.vBinormal = Vector3(0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	// Body
	UINT iStackCount = 10; // 가로 분할 개수
	UINT iSliceCount = 10; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vector3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vector2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVTX.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vector3(0.f, -fRadius, 0.f);
	v.vUV = Vector2(0.5f, 1.f);
	v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vector3(1.f, 0.f, 0.f);
	v.vBinormal = Vector3(0.f, 0.f, -1.f);
	vecVTX.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
		            // +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVTX.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	Ptr<CMesh> pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(_fName /*+ L"_Sphere"*/);
	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

}

void CreateBoundingCubeCollisionMesh(Vector4* vecMMax, wstring _fName)
{
	Vector3 vecMin = vecMMax[0];
	Vector3 vecMax = vecMMax[1];

	VTX arrCube[24] = {};
	{
		// 윗면
		arrCube[0].vPos = Vector3(vecMin.x, vecMax.y, vecMax.z);
		arrCube[0].vColor = Vector4(1.f, 1.f, 1.f, 1.f);

		arrCube[1].vPos = Vector3(vecMax.x, vecMax.y, vecMax.z);
		arrCube[1].vColor = Vector4(1.f, 1.f, 1.f, 1.f);

		arrCube[2].vPos = Vector3(vecMax.x, vecMax.y, vecMin.z);
		arrCube[2].vColor = Vector4(1.f, 1.f, 1.f, 1.f);

		arrCube[3].vPos = Vector3(vecMin.x, vecMax.y, vecMin.z);
		arrCube[3].vColor = Vector4(1.f, 1.f, 1.f, 1.f);


		// 아랫 면
		arrCube[4].vPos = Vector3(vecMin.x, vecMin.y, vecMin.z);
		arrCube[4].vColor = Vector4(1.f, 0.f, 0.f, 1.f);

		arrCube[5].vPos = Vector3(vecMax.x, vecMin.y, vecMin.z);
		arrCube[5].vColor = Vector4(1.f, 0.f, 0.f, 1.f);

		arrCube[6].vPos = Vector3(vecMax.x, vecMin.y, vecMax.z);
		arrCube[6].vColor = Vector4(1.f, 0.f, 0.f, 1.f);

		arrCube[7].vPos = Vector3(vecMin.x, vecMin.y, vecMax.z);
		arrCube[7].vColor = Vector4(1.f, 0.f, 0.f, 1.f);

		// 왼쪽 면
		arrCube[8].vPos = Vector3(vecMin.x, vecMax.y, vecMax.z);
		arrCube[8].vColor = Vector4(0.f, 1.f, 0.f, 1.f);

		arrCube[9].vPos = Vector3(vecMin.x, vecMax.y, vecMin.z);
		arrCube[9].vColor = Vector4(0.f, 1.f, 0.f, 1.f);

		arrCube[10].vPos = Vector3(vecMin.x, vecMin.y, vecMin.z);
		arrCube[10].vColor = Vector4(0.f, 1.f, 0.f, 1.f);

		arrCube[11].vPos = Vector3(vecMin.x, vecMin.y, vecMax.z);
		arrCube[11].vColor = Vector4(0.f, 1.f, 0.f, 1.f);

		// 오른쪽 면
		arrCube[12].vPos = Vector3(vecMax.x, vecMax.y, vecMin.z);
		arrCube[12].vColor = Vector4(0.f, 0.f, 1.f, 1.f);

		arrCube[13].vPos = Vector3(vecMax.x, vecMax.y, vecMax.z);
		arrCube[13].vColor = Vector4(0.f, 0.f, 1.f, 1.f);

		arrCube[14].vPos = Vector3(vecMax.x, vecMin.y, vecMax.z);
		arrCube[14].vColor = Vector4(0.f, 0.f, 1.f, 1.f);

		arrCube[15].vPos = Vector3(vecMax.x, vecMin.y, vecMin.z);
		arrCube[15].vColor = Vector4(0.f, 0.f, 1.f, 1.f);

		// 뒷 면
		arrCube[16].vPos = Vector3(vecMax.x, vecMax.y, vecMax.z);
		arrCube[16].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

		arrCube[17].vPos = Vector3(vecMin.x, vecMax.y, vecMax.z);
		arrCube[17].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

		arrCube[18].vPos = Vector3(vecMin.x, vecMin.y, vecMax.z);
		arrCube[18].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

		arrCube[19].vPos = Vector3(vecMax.x, vecMin.y, vecMax.z);
		arrCube[19].vColor = Vector4(1.f, 1.f, 0.f, 1.f);

		// 앞 면
		arrCube[20].vPos = Vector3(vecMin.x, vecMax.y, vecMin.z);;
		arrCube[20].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

		arrCube[21].vPos = Vector3(vecMax.x, vecMax.y, vecMin.z);
		arrCube[21].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

		arrCube[22].vPos = Vector3(vecMax.x, vecMin.y, vecMin.z);
		arrCube[22].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

		arrCube[23].vPos = Vector3(vecMin.x, vecMin.y, vecMin.z);
		arrCube[23].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

		vecMMax[0] = arrCube[23].vPos;
		vecMMax[1] = arrCube[16].vPos;
	}

	vector<UINT> vecIdx;

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	Ptr<CMesh> pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), 24, (BYTE*)arrCube
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	//   mesh별 아이디 필요
	pMesh->SetName(_fName /*+ L"_Cube"*/);
	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

}

bool closeEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon())
{
	return (epsilon > std::abs(a - b));
}

Vector3 DecomposeRotMat(const Matrix& _matRot)
{
	// _mat 을 분해 후 다시 행렬 만들기	
	Vector4 vMat[4];
	XMStoreFloat4(&vMat[0], _matRot.Right());   // 1
	XMStoreFloat4(&vMat[1], _matRot.Up());      // 2
	XMStoreFloat4(&vMat[2], _matRot.Front());   // 3
	XMStoreFloat4(&vMat[3], _matRot.Translation());//4

	Vector3 vNewRot;
	if (closeEnough(vMat[0].z, -1.0f))
	{
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2(vMat[1].x, vMat[2].x);
		vNewRot = Vector3{ x, y, z };
	}
	else if (closeEnough(vMat[0].z, 1.0f)) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-vMat[1].x, -vMat[2].x);
		vNewRot = Vector3{ x, y, z };
	}
	else { //two solutions exist
		float y1 = -asin(vMat[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(vMat[1].z / cos(y1), vMat[2].z / cos(y1));
		float x2 = atan2f(vMat[1].z / cos(y2), vMat[2].z / cos(y2));

		float z1 = atan2f(vMat[0].y / cos(y1), vMat[0].x / cos(y1));
		float z2 = atan2f(vMat[0].y / cos(y2), vMat[0].x / cos(y2));

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			vNewRot = Vector3{ x1, y1, z1 };
		}
		else {
			vNewRot = Vector3{ x2, y2, z2 };
		}
	}
	return vNewRot;
}