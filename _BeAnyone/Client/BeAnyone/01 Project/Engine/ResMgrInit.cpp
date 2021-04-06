#include "pch.h"
#include "ResMgr.h"
#include "RenderMgr.h"


void CResMgr::CreateDefaultMesh()
{
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;

	Ptr<CMesh> pMesh = nullptr;


	// =============	// 0 --- 1
	// Rect Mesh		// |  \  |
	//					// 3 --- 2
	// =============	
	pMesh = new CMesh;

	VTX v;
	// 1. 입력 조립기 단계에 전달할, 정점 3개로 구성된 삼각형 1개
	v.vPos = Vector3(-0.5f, 0.5f, 0.f);
	v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vector2(0.f, 0.f);
	v.vNormal = Vector3(0.f, 0.f, -1.f);
	v.vTangent = Vector3(1.f, 0.f, 0.f);
	v.vBinormal = Vector3(0.f, 1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(0.5f, 0.5f, 0.f);
	v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vector2(1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(0.5f, -0.5f, 0.f);
	v.vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vector2(1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(-0.5f, -0.5f, 0.f);
	v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vector2(0.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	AddRes(L"RectMesh", pMesh);

	// =============		// 0 --- 1
	// Collider Rect Mesh	// |     |
	//						// 3 --- 2
	// =============	
	pMesh = new CMesh;

	vecIdx.clear();
	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(0);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); //D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

	AddRes(L"ColRectMesh", pMesh);

	//				       3
	// ===========	//    /
	// Dir Mesh		//0,2,4 ㅡ ㅡ 5
	//				//    \
	// ===========	//     1
	pMesh = new CMesh;

	vecVTX.clear();
	vecIdx.clear();

	v.vPos = Vector3(0.f, 0.f, 0.f);
	v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(1.f, 0.f, 0.f);
	v.vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(0.f, 0.f, 0.f);
	v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(0.f, 1.f, 0.f);
	v.vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(0.f, 0.f, 0.f);
	v.vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vector3(0.f, 0.f, 1.f);
	v.vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3);
	vecIdx.push_back(4); vecIdx.push_back(5);

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); // D3D11_PRIMITIVE_TOPOLOGY_LINELIST

	AddRes(L"DirMesh", pMesh);

	// ============
	// Circle Mesh
	// ============
	vecVTX.clear();
	vecIdx.clear();

	pMesh = new CMesh;

	// 원의 중심점
	v.vPos = Vector3(0.f, 0.f, 0.f);
	v.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vector3(0.f, 0.f, -1.f);
	v.vTangent = Vector3(1.f, 0.f, 0.f);
	v.vBinormal = Vector3(0.f, 1.f, 0.f);
	v.vUV = Vector2(0.5f, 0.5f);

	vecVTX.push_back(v);

	UINT iSliceCount = 32;
	float fRadius = 0.5f;
	float fSliceTheta = XM_2PI / iSliceCount;

	float fCurTheta = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vector3(fRadius * cosf(fCurTheta), fRadius * sinf(fCurTheta), 0.f);
		v.vColor = Vector4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vector2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta += fSliceTheta;

		vecVTX.push_back(v);
	}

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	AddRes(L"CircleMesh", pMesh);

	// ============
	// ColCircle Mesh
	// ============	
	vecIdx.clear();

	pMesh = new CMesh;

	for (UINT i = 1; i < iSliceCount + 2; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data()); // D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP

	AddRes(L"ColCircleMesh", pMesh);

	vecVTX.clear();
	vecIdx.clear();

	// =========
	// Cube Mesh
	// =========
	VTX arrCube[24] = {};

	// 윗면
	arrCube[0].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vector2(0.f, 0.f);
	arrCube[0].vNormal = Vector3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vector3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vector2(1.f, 0.f);
	arrCube[1].vNormal = Vector3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vector3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vector2(1.f, 1.f);
	arrCube[2].vNormal = Vector3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vector3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vector2(0.f, 1.f);
	arrCube[3].vNormal = Vector3(0.f, 1.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vector2(0.f, 0.f);
	arrCube[4].vNormal = Vector3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vector3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vector2(0.f, 1.f);
	arrCube[5].vNormal = Vector3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vector3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vector2(1.f, 1.f);
	arrCube[6].vNormal = Vector3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vector4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vector2(1.f, 0.f);
	arrCube[7].vNormal = Vector3(0.f, -1.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vector2(0.f, 0.f);
	arrCube[8].vNormal = Vector3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vector3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vector2(0.f, 1.f);
	arrCube[9].vNormal = Vector3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vector2(1.f, 1.f);
	arrCube[10].vNormal = Vector3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vector4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vector2(1.f, 0.f);
	arrCube[11].vNormal = Vector3(-1.f, 0.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos = Vector3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vector2(0.f, 0.f);
	arrCube[12].vNormal = Vector3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vector3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vector2(0.f, 1.f);
	arrCube[13].vNormal = Vector3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vector3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vector2(1.f, 1.f);
	arrCube[14].vNormal = Vector3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vector3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vector4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vector2(1.f, 0.f);
	arrCube[15].vNormal = Vector3(1.f, 0.f, 0.f);

	// 뒷 면
	arrCube[16].vPos = Vector3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vector2(0.f, 0.f);
	arrCube[16].vNormal = Vector3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vector3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vector2(0.f, 1.f);
	arrCube[17].vNormal = Vector3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vector3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vector2(1.f, 1.f);
	arrCube[18].vNormal = Vector3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vector3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vector4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vector2(1.f, 0.f);
	arrCube[19].vNormal = Vector3(0.f, 0.f, 1.f);

	// 앞 면
	arrCube[20].vPos = Vector3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vector2(0.f, 0.f);
	arrCube[20].vNormal = Vector3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vector3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vector2(0.f, 1.f);
	arrCube[21].vNormal = Vector3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vector3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vector2(1.f, 1.f);
	arrCube[22].vNormal = Vector3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vector2(1.f, 0.f);
	arrCube[23].vNormal = Vector3(0.f, 0.f, -1.f);

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

	pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), 24, (BYTE*)arrCube
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(L"CubeMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();

	// test
	/*VTX Cube = {};

	Cube.vPos = Vector3(-1.0f, -1.0f, -1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(-1.0f, 1.0f, -1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(1.0f, 1.0f, -1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(1.0f, -1.0f, -1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(-1.0f, -1.0f, 1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(-1.0f, 1.0f, 1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(1.0f, 1.0f, 1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);

	Cube.vPos = Vector3(1.0f, -1.0f, 1.0f);
	Cube.vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Cube.vUV = Vector2(0.f, 0.f);
	Cube.vNormal = Vector3(0.f, 1.f, 0.f);

	vecVTX.push_back(Cube);


	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	vecIdx.push_back(4); vecIdx.push_back(6); vecIdx.push_back(5);
	vecIdx.push_back(4); vecIdx.push_back(7); vecIdx.push_back(6);

	vecIdx.push_back(4); vecIdx.push_back(5); vecIdx.push_back(1);
	vecIdx.push_back(4); vecIdx.push_back(1); vecIdx.push_back(0);

	vecIdx.push_back(3); vecIdx.push_back(2); vecIdx.push_back(6);
	vecIdx.push_back(3); vecIdx.push_back(6); vecIdx.push_back(7);

	vecIdx.push_back(1); vecIdx.push_back(5); vecIdx.push_back(6);
	vecIdx.push_back(1); vecIdx.push_back(6); vecIdx.push_back(2);

	vecIdx.push_back(4); vecIdx.push_back(0); vecIdx.push_back(3);
	vecIdx.push_back(4); vecIdx.push_back(3); vecIdx.push_back(7);

	pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), 24, (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(L"TestCube");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();*/

	// ============
	// Sphere Mesh
	// ============
	fRadius = 1.f;

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
	UINT iStackCount = 40; // 가로 분할 개수
	iSliceCount = 40; // 세로 분할 개수

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

	pMesh = new CMesh;

	pMesh->Create(sizeof(VTX), (UINT)vecVTX.size(), (BYTE*)vecVTX.data()
		, DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), (BYTE*)vecIdx.data());

	pMesh->SetName(L"SphereMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();
}

void CResMgr::CreateDefaultShader()
{
	//=============
	// TestShader
	// int 1 개 필요
	//=============
	Ptr<CShader> pShader = nullptr;

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Test", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Test", "ps_5_0");

	// BlendState 설정
	//pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);
	pShader->Create(SHADER_POV::FORWARD);

	pShader->AddShaderParam(tShaderParam{ L"Test Value", SHADER_PARAM::INT_0 });

	AddRes(L"TestShader", pShader);

	//// ==============
	//// Texture Shader
	//// ==============
	//pShader = new CShader;
	//pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Tex", "vs_5_0");
	//pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Tex", "ps_5_0");

	//// BlendState 설정
	//pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	//// Shader Parameter 알림
	//pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });

	//pShader->Create();

	//AddRes(L"TexShader", pShader);

	//// =================
	//// Collider2D Shader
	//// =================
	//pShader = new CShader;
	//pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Collider2D", "vs_5_0");
	//pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Collider2D", "ps_5_0");
	//pShader->Create();
	//AddRes(L"Collider2DShader", pShader);

	//// =================
	//// STD2D Shader
	//// =================
	//pShader = new CShader;
	//pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Std2D", "vs_5_0");
	//pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Std2D", "ps_5_0");

	//// BlendState 설정
	//pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	//// Parameter 설정
	//pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	//pShader->AddShaderParam(tShaderParam{ L"Anim Tex", SHADER_PARAM::TEX_3 });

	//pShader->Create();
	//AddRes(L"Std2DShader", pShader);

	//// =================
	//// 2DShadow Shader
	//// =================
	//pShader = new CShader;
	//pShader->CreateVertexShader(L"Shader\\std.fx", "VS_2DShadow", "vs_5_0");
	//pShader->CreatePixelShader(L"Shader\\std.fx", "PS_2DShadow", "ps_5_0");

	//// BlendState 설정
	//pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);

	//// Parameter 설정
	//pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	//pShader->Create();
	//AddRes(L"2DShadowShader", pShader);

	// ============
	// Std3D Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Std3D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Std3D", "ps_5_0");
	//pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->Create(SHADER_POV::DEFERRED);
	AddRes(L"Std3DShader", pShader);

	// ============
	// Skybox Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Skybox", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Skybox", "ps_5_0");
	pShader->SetRasterizerType(RS_TYPE::CULL_FRONT);  // 구의 안쪽면을 보기 때문에, 시계방향 컬링을 사용한다(반시계를 그림)
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS_EQUAL); // 스카이박스 표면을 최대거리로 셋팅하기 때문에, 깊이 판정을 같거나 작다로 설정한다.
	pShader->Create(SHADER_POV::FORWARD);
	AddRes(L"SkyboxShader", pShader);

	// ============
	// GridShader
	// ============
	pShader = new CShader;

	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Grid", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Grid", "ps_5_0");
	pShader->SetBlendState(BLEND_TYPE::ALPHABLEND);
	pShader->SetRasterizerType(RS_TYPE::CULL_NONE);
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::LESS_NO_WRITE);
	pShader->Create(SHADER_POV::FORWARD);
	AddRes(L"GridShader", pShader);

	// ===============
	// DirLight Shader
	// ===============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_DirLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_DirLight", "ps_5_0");

	// One-One Blend
	pShader->SetBlendState(BLEND_TYPE::ONEBLEND);

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	pShader->AddShaderParam(tShaderParam{ L"Light Index", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Normal Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });
	pShader->Create(SHADER_POV::LIGHTING);

	AddRes(L"DirLightShader", pShader);

	// ==================
	// PointLight Shader
	// ==================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_PointLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_PointLight", "ps_5_0");

	// One-One Blend
	pShader->SetBlendState(BLEND_TYPE::ONEBLEND);

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	// Cull_None
	pShader->SetRasterizerType(RS_TYPE::CULL_FRONT);

	pShader->AddShaderParam(tShaderParam{ L"Light Index", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Normal Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Render Target Resolution", SHADER_PARAM::VEC2_0 });

	pShader->Create(SHADER_POV::LIGHTING);
	AddRes(L"PointLightShader", pShader);

	// ==================
	// MergeLight Shader
	// ==================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_MergeLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_MergeLight", "ps_5_0");

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilType(DEPTH_STENCIL_TYPE::NO_DEPTHTEST_NO_WRITE);

	pShader->AddShaderParam(tShaderParam{ L"Diffuse Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Light Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Specular Target Texture", SHADER_PARAM::TEX_2 });

	pShader->Create(SHADER_POV::LIGHTING);
	AddRes(L"MergeLightShader", pShader);

	//=======================
	// Test Compute Shader
	// int 1 개 필요
	//=======================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\compute.fx", "CS_TEST", "cs_5_0");
	pShader->AddShaderParam(tShaderParam{ L"Test Value", SHADER_PARAM::INT_0 });
	AddRes(L"CSTestShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMtrl);

	/*pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TexShader"));
	AddRes(L"TexMtrl", pMtrl);

	int a = 0;
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	AddRes(L"Collider2DMtrl_0", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	a = 1;
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	AddRes(L"Collider2DMtrl_1", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"DirShader"));
	AddRes(L"DirMtrl", pMtrl);*/

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std3DShader"));
	AddRes(L"Std3DMtrl", pMtrl);


	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"SkyboxShader"));
	AddRes(L"SkyboxMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"GridShader"));
	//Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");
	//pMtrl->SetData(SHADER_PARAM::TEX_0, pPositionTargetTex.GetPointer());
	AddRes(L"GridMtrl", pMtrl);



	//pMtrl = new CMaterial;
	////pMtrl->DisableFileSave();
	//pMtrl->SetShader(FindRes<CShader>(L"2DShadowShader"));
	//pMtrl->SetPath(L"Material\\2DShadowMtrl.mtrl");
	//AddRes(L"Material\\2DShadowMtrl.mtrl", pMtrl);

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"DirLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"NormalTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, pTargetTex.GetPointer());

		AddRes(L"DirLightMtrl", pMtrl);
	}

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"PointLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"NormalTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, pTargetTex.GetPointer());

		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);

		AddRes(L"PointLightMtrl", pMtrl);
	}

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"MergeLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"DiffuseTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"DiffuseLightTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, pTargetTex.GetPointer());

		pTargetTex = FindRes<CTexture>(L"SpecularLightTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_2, pTargetTex.GetPointer());

		AddRes(L"MergeLightMtrl", pMtrl);
	}

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"CSTestShader"));
	AddRes(L"CSTestMtrl", pMtrl);
}
