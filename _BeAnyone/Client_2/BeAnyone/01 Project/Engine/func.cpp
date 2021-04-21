#pragma once
#include "pch.h"
#include "func.h"
#include "KeyMgr.h"
#include "PathMgr.h"
#include "TimeMgr.h"

// 물체 하나 그리기
#include "Core.h"
#include "Device.h"

#include "Mesh.h"
#include "pch.h"
#include "SceneMgr.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

#include "Device.h"
#include "Core.h"

#include "MeshRenderer.h"
#include "ToolCamScript.h"
#include "RenderMgr.h"
vector<VTX> g_vecVTX;
vector<UINT> g_vecIDX;	

CMesh* g_pMesh = nullptr;

CShader* g_pShader = nullptr;

CTexture* g_pTexture = nullptr;

CMaterial* g_pMtrl_0 = nullptr;
CMaterial* g_pMtrl_1 = nullptr;

CTexture* g_pTex = nullptr;

vector<CGameObject*> g_vecObj;
//
//void TestInit()
//{
//	g_pMesh = new CMesh;
//	g_pShader = new CShader;
//
//	VTX v;
//
//	v.vPos = XMFLOAT3(-0.5f, -0.5f, 0.2f);
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
//	g_vecIDX.push_back(4); g_vecIDX.push_back(3); g_vecIDX.push_back(7);
//
//	g_pMesh->Create(sizeof(VTX), g_vecVTX.size(), (BYTE*)g_vecVTX.data()
//		, DXGI_FORMAT_R32_UINT, g_vecIDX.size(), (BYTE*)g_vecIDX.data());
//
//	// 쉐이더 생성
//	g_pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Test", "vs_5_1");
//	g_pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Test", "ps_5_1");
//	g_pShader->Create();
//
//	CDevice::GetInst()->FlushCommandQueue();
//
//	// GameObject 만들기
//	CGameObject* pObject = nullptr;
//	{
//		pObject = new CGameObject;
//		pObject->AddComponent( new CTransform );
//		pObject->AddComponent( new CMeshRender );
//
//		pObject->MeshRender()->SetMesh( g_pMesh );
//		pObject->MeshRender()->SetMaterial( g_pShader );
//		
//		g_vecObj.push_back( pObject );
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
//
//	if (KEY_HOLD( KEY_TYPE::KEY_LEFT ))
//	{
//		Vec3 vPos = g_vecObj[0]->Transform()->GetLocalPos();
//		vPos.x -= DT * 1.f;
//		g_vecObj[0]->Transform()->SetLocalPos( vPos );
//	}
//
//	if (KEY_HOLD( KEY_TYPE::KEY_RIGHT ))
//	{
//		Vec3 vPos = g_vecObj[0]->Transform()->GetLocalPos();
//		vPos.x += DT * 1.f;
//		g_vecObj[0]->Transform()->SetLocalPos( vPos );
//	}
//}
//
//void TestRender()
//{
//	// 그리기 준비
//	float arrColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
//
//	CDevice::GetInst()->render_start(arrColor);
//	for (size_t i = 0; i < g_vecObj.size(); ++i)
//	{
//		g_vecObj[i]->render();
//	}
//
//	/*g_pShader->UpdateData();
//
//	XMFLOAT4 vData = XMFLOAT4{ 0.f, 0.f, 0.f, 0.f };
//
//	CConstantBuffer* pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);
//	pCB->SetData(&vData, sizeof(XMFLOAT4), 0);	
//	CDevice::GetInst()->SetConstBufferToRegister(pCB, 0);
//
//	g_pMesh->render();*/
//
//	/*vData = XMFLOAT4{ -0.5f, 0.f, 0.f, 0.f };
//
//	pCB = CDevice::GetInst()->GetCB(CONST_REGISTER::b0);
//	pCB->SetData(&vData, sizeof(XMFLOAT4), 1);
//	CDevice::GetInst()->SetConstBufferToRegister(pCB, 1);	
//
//	g_pMesh->render();*/
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

void SaveWString( FILE* _pFile, const wstring& _str ) {
	BYTE c = (BYTE)_str.length();
	fwrite( &c, 1, 1, _pFile );
	fwrite( _str.c_str(), 2, c, _pFile );
}

wchar_t* LoadWString( FILE* _pFile ) {
	static wchar_t szStr[255] = {};

	BYTE c = 0;
	fread( &c, 1, 1, _pFile );
	fread( szStr, 2, c, _pFile );
	szStr[c] = 0;

	return szStr;
}


void TestRelease() {
	SAFE_DELETE( g_pMesh );

	SAFE_DELETE( g_pMtrl_0 );
	SAFE_DELETE( g_pMtrl_1 );

	SAFE_DELETE( g_pShader );
	SAFE_DELETE( g_pTex );
}

bool closeEnough( const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon() ) {
	return (epsilon > std::abs( a - b ));
}

Vec3 DecomposeRotMat( const Matrix& _matRot ) {
	// _mat 을 분해 후 다시 행렬 만들기	
	Vec4 vMat[4];
	XMStoreFloat4( &vMat[0], _matRot.Right() );   // 1
	XMStoreFloat4( &vMat[1], _matRot.Up() );      // 2
	XMStoreFloat4( &vMat[2], _matRot.Front() );   // 3
	XMStoreFloat4( &vMat[3], _matRot.Translation() );//4

	Vec3 vNewRot;
	if (closeEnough( vMat[0].z, -1.0f ))
	{
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2( vMat[1].x, vMat[2].x );
		vNewRot = Vec3 {x, y, z};
	}
	else if (closeEnough( vMat[0].z, 1.0f )) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2( -vMat[1].x, -vMat[2].x );
		vNewRot = Vec3 {x, y, z};
	}
	else { //two solutions exist
		float y1 = -asin( vMat[0].z );
		float y2 = XM_PI - y1;

		float x1 = atan2f( vMat[1].z / cos( y1 ), vMat[2].z / cos( y1 ) );
		float x2 = atan2f( vMat[1].z / cos( y2 ), vMat[2].z / cos( y2 ) );

		float z1 = atan2f( vMat[0].y / cos( y1 ), vMat[0].x / cos( y1 ) );
		float z2 = atan2f( vMat[0].y / cos( y2 ), vMat[0].x / cos( y2 ) );

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs( x1 ) + std::abs( y1 ) + std::abs( z1 )) <= (std::abs( x2 ) + std::abs( y2 ) + std::abs( z2 )))
		{
			vNewRot = Vec3 {x1, y1, z1};
		}
		else {
			vNewRot = Vec3 {x2, y2, z2};
		}
	}
	return vNewRot;
}