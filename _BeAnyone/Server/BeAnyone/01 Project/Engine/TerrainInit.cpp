#include "pch.h"
#include "Terrain.h"

#include "RenderMgr.h"

#include "Camera.h"
#include "Transform.h"

#include "ResMgr.h"
#include "MeshRender.h"
#include "Shader.h"

void CTerrain::init(Ptr<CTexture> _pMap) {
	ChangeFaceCount( m_iFaceX, m_iFaceZ );

	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData( SHADER_PARAM::INT_1, &m_iFaceX );
	pMtrl->SetData( SHADER_PARAM::INT_2, &m_iFaceZ );

	// 높이맵 텍스쳐 Create	
	if(_pMap != nullptr)
		m_pHeightMap = _pMap;
	else
		m_pHeightMap = CResMgr::GetInst()->Load<CTexture>(L"HeightMap", L"Texture\\Terrain\\test2.bmp");
		//m_pHeightMap = CResMgr::GetInst()->Load<CTexture>( L"HeightMap", L"Texture\\Terrain\\HeightMap.jpg" );

	
	Vector2 vHeightMapRes = Vector2( m_pHeightMap->Width(), m_pHeightMap->Height() );

	// 타일 텍스쳐
	Ptr<CTexture> pTile = CResMgr::GetInst()->FindRes<CTexture>( L"Texture\\Tile\\TILE_01.tga" );
	Ptr<CTexture> pTile_n = CResMgr::GetInst()->FindRes<CTexture>( L"Texture\\Tile\\TILE_01_N.tga" );

	pMtrl->SetData( SHADER_PARAM::TEX_2, m_pHeightMap.GetPointer() );
	pMtrl->SetData( SHADER_PARAM::VEC2_0, &vHeightMapRes );
	pMtrl->SetData( SHADER_PARAM::FLOAT_0, &m_fMaxTess );

	
	//Ptr<CTexture> pBase = CResMgr::GetInst()->Load<CTexture>(L"BaseTexture", L"Texture\\Terrain\\Base_Texture1.jpg");
	Ptr<CTexture> pBase = CResMgr::GetInst()->Load<CTexture>(L"BaseTexture", L"Texture\\Terrain\\Desert_Base.png");
	pMtrl->SetData(SHADER_PARAM::TEX_3, pBase.GetPointer());

	
	CreateHeightmapPixelsInfo();

}

void CTerrain::ChangeFaceCount( UINT _iFaceX, UINT _iFaceZ ) {
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceZ;

	CreateTerrainMesh();
}

void CTerrain::CreateTerrainMesh() {
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	Ptr<CMesh> pMesh = nullptr;

	// =============	
	// Terrain Mesh		
	// =============	
	pMesh = new CMesh;

	VTX v;

	for (UINT i = 0; i < m_iFaceZ + 1; ++i)
	{
		for (UINT j = 0; j < m_iFaceX + 1; ++j)
		{
			v.vPos = Vector3( (float)j, 0.f, (float)i );
			v.vColor = Vector4( 1.f, 1.f, 1.f, 1.f );
			v.vUV = Vector2( (float)j, (float)(m_iFaceZ - i) );
			v.vNormal = Vector3( 0.f, 1.f, 0.f );
			v.vTangent = Vector3( 1.f, 0.f, 0.f );
			v.vBinormal = Vector3( 0.f, 0.f, 1.f );
			vecVtx.push_back( v );
		}
	}

	for (UINT i = 0; i < m_iFaceZ; ++i)
	{
		for (UINT j = 0; j < m_iFaceX; ++j)
		{
			// 0       
			// | \  
			// 2--1 
			vecIdx.push_back( (m_iFaceX + 1) * (i + 1) + j );
			vecIdx.push_back( (m_iFaceX + 1) * i + j + 1 );
			vecIdx.push_back( (m_iFaceX + 1) * i + j );

			// 1--2
			//  \ |
			//	  0
			vecIdx.push_back( (m_iFaceX + 1) * (i)+(j + 1) );
			vecIdx.push_back( (m_iFaceX + 1) * (i + 1) + j );
			vecIdx.push_back( (m_iFaceX + 1) * (i + 1) + j + 1 );
		}
	}

	pMesh->Create( sizeof( VTX ), (UINT)vecVtx.size(), (BYTE*)vecVtx.data()
				   , DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data() );

	Ptr<CMesh> pOriginMesh = CResMgr::GetInst()->FindRes<CMesh>( L"TerrainMesh" );
	if (nullptr != pOriginMesh)
	{
		CResMgr::GetInst()->DestroyResource<CMesh>( L"TerrainMesh" );
	}

	CResMgr::GetInst()->AddRes<CMesh>( L"TerrainMesh", pMesh );

	MeshRender()->SetMesh( pMesh );
}

void CTerrain::CreateHeightmapPixelsInfo()
{
	int m_nWidth = m_pHeightMap->Width();
	int m_nLength = m_pHeightMap->Height();

	BYTE* pHeightMapPixels = new BYTE[m_nWidth * m_nLength];

	HANDLE hFile = ::CreateFile(L"../\\../\\02 File\\bin\\content\\Texture\\Terrain\\HeightMap.raw"
		, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	/*HANDLE hFile = ::CreateFile(L"../\\../\\02 File\\bin\\content\\Texture\\Terrain\\T3.raw"
		, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);*/
	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapPixels, (m_nWidth * m_nLength), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	m_pHeightMapPixels = new BYTE[m_nWidth * m_nLength];
	for (int y = 0; y < m_nLength; ++y)
	{
		for (int x = 0; x < m_nWidth; ++x)
		{
			m_pHeightMapPixels[x + ((m_nLength - 1 - y) * m_nWidth)] = pHeightMapPixels[x + (y * m_nWidth)];
		}
	}

	if (pHeightMapPixels) 
		delete[] pHeightMapPixels;
}

float CTerrain::GetHeight(float _fx, float _fz, bool _check)
{
	_check = true;

	float fx = _fx / Transform()->GetLocalScale().x * 8.f;
	float fz = _fz / Transform()->GetLocalScale().z * 8.f;
	
	int m_nWidth = m_pHeightMap->Width();
	int m_nLength = m_pHeightMap->Height();

	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength)) 
		return(0.0f);

	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;

	float fBottomLeft = (float)m_pHeightMapPixels[x + (z * m_nWidth)];
	float fBottomRight = (float)m_pHeightMapPixels[(x + 1) + (z * m_nWidth)];
	float fTopLeft = (float)m_pHeightMapPixels[x + ((z + 1) * m_nWidth)];
	float fTopRight = (float)m_pHeightMapPixels[(x + 1) + ((z + 1) * m_nWidth)];

	if (_check)
	{
		if (fzPercent >= fxPercent)
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		else
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
	}
	else
	{
		if (fzPercent < (1.0f - fxPercent))
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		else
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}

	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;

	float NormalizedHeight = fHeight / 255.f * Transform()->GetLocalScale().y / 2.f + 200.f;

	return NormalizedHeight;
}

void CTerrain::CreateHeightmapPixelsInfo__()
{
	int m_nWidth = m_pHeightMap->Width();
	int m_nLength = m_pHeightMap->Height();

	UCHAR* pHeightMapPixels = new UCHAR[m_nWidth * m_nLength];

	HANDLE hFile = ::CreateFile(L"C:\\Users\\채영문\\Desktop\\BeAnyone\\BeAnyone\\02 File\\bin\\content\\Texture\\Terrain\\test2.raw"
		, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	//HANDLE hFile = ::CreateFile(L"C:\\Users\\채영문\\Desktop\\BeAnyone\\BeAnyone\\02 File\\bin\\content\\Texture\\Terrain\\T3.raw"
		//, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapPixels, (m_nWidth * m_nLength), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	m_pHeightMapPixels = new BYTE[m_nWidth * m_nLength];
	for (int y = 0; y < m_nLength; ++y)
	{
		if (y == 2500)
		{
			int a = 0;
		}
		for (int x = 0; x < m_nWidth; ++x)
		{
			m_pHeightMapPixels[x + ((m_nLength - 1 - y) * m_nWidth)] = pHeightMapPixels[x + (y * m_nWidth)];
		}
	}
	auto a = m_pHeightMapPixels;
	if (pHeightMapPixels)
		delete[] pHeightMapPixels;
}