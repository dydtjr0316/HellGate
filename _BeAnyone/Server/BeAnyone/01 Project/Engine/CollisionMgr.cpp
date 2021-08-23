#include "pch.h"
#include "CollisionMgr.h"

#include "ResMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Collider.h"
#include "Mesh.h"


CCollisionMgr::CCollisionMgr()
	: m_LayerCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::init()
{
}

void CCollisionMgr::update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (int j = 0; j < MAX_LAYER; ++j)
		{
			if (m_LayerCheck[i] & (1 << j))
			{
				// i <= j
				CollisionLayer(pCurScene->GetLayer(i), pCurScene->GetLayer(j));
			}
		}
	}
}

void CCollisionMgr::CheckCollisionLayer(const wstring& _strLayerName1, const wstring& _strLayerName2)
{
	CLayer* pLayer1 = CSceneMgr::GetInst()->GetCurScene()->FindLayer(_strLayerName1);
	CLayer* pLayer2 = CSceneMgr::GetInst()->GetCurScene()->FindLayer(_strLayerName2);

	assert(nullptr != pLayer1 && nullptr != pLayer2);

	int iIdx1 = pLayer1->GetLayerIdx();
	int iIdx2 = pLayer2->GetLayerIdx();

	int iMinIdx = min(iIdx1, iIdx2);
	int iMaxIdx = max(iIdx1, iIdx2);

	m_LayerCheck[iMinIdx] |= (1 << iMaxIdx);
}

void CCollisionMgr::CheckCollisionLayer(int _iLayerIdx1, int _iLyaerIdx2)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

	assert(pScene->GetLayer(_iLayerIdx1) && pScene->GetLayer(_iLyaerIdx2));

	int iMinIdx = min(_iLayerIdx1, _iLyaerIdx2);
	int iMaxIdx = max(_iLayerIdx1, _iLyaerIdx2);

	m_LayerCheck[iMinIdx] |= (1 << iMaxIdx);
}

void CCollisionMgr::CollisionLayer(const CLayer* _pLayer1, const CLayer* _pLayer2)
{
	const vector<CGameObject*>& vecObj1 = _pLayer1->GetObjects();
	const vector<CGameObject*>& vecObj2 = _pLayer2->GetObjects();
	map<DWORD_PTR, bool>::iterator iter;

	for (size_t i = 0; i < vecObj1.size(); ++i)
	{
		CCollider* pCollider1 = vecObj1[i]->Collider();

		if (nullptr == pCollider1)
			continue;

		size_t j = 0;
		if (_pLayer1 == _pLayer2) // 동일 레이어 간 충돌
			j = i + 1;

		for (; j < vecObj2.size(); ++j)
		{
			CCollider* pCollider2 = vecObj2[j]->Collider();

			if (nullptr == pCollider2)
				continue;

			tColID id;
			id.iColID1 = pCollider1->GetColID();
			id.iColID2 = pCollider2->GetColID();
			iter = m_mapCol.find(id.ID);

			bool IsDead = false;
			if (pCollider1->GetObj()->IsDead() || pCollider2->GetObj()->IsDead())
			{

				IsDead = true;
			}

			// 충돌했다.
			if (IsCollision(pCollider1, pCollider2))
			{

				
				
				// 충돌 중이다
				if (m_mapCol.end() != iter && iter->second == true)
				{
					if (IsDead)
					{
						pCollider1->OnCollisionExit(pCollider2);
						pCollider2->OnCollisionExit(pCollider1);
						iter->second = false;
					}
					else
					{
						pCollider1->OnCollision(pCollider2);
						pCollider2->OnCollision(pCollider1);
					}
				}
				// 처음 충돌했다
				else
				{
					if (IsDead)
						continue;

					pCollider1->OnCollisionEnter(pCollider2);
					pCollider2->OnCollisionEnter(pCollider1);

					if (m_mapCol.end() == iter)
					{
						m_mapCol.insert(make_pair(id.ID, true));
					}
					else
					{
						iter->second = true;
					}
				}
			}
			else // 충돌하지 않는다.
			{
				
				if (m_mapCol.end() != iter && true == iter->second)
				{
					pCollider1->OnCollisionExit(pCollider2);
					pCollider2->OnCollisionExit(pCollider1);

					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pCollider1, CCollider* _pCollider2)
{
	if (!_pCollider1->IsActive() || !_pCollider1->GetObj()->IsActive() || !_pCollider2->IsActive() || !_pCollider2->GetObj()->IsActive())
		return false;

	if (COLLIDER_TYPE::BOX == _pCollider1->GetColliderType() && COLLIDER_TYPE::BOX == _pCollider2->GetColliderType())
	{
		return CollisionSphere(_pCollider1, _pCollider2);
	}
	else if (COLLIDER_TYPE::MESH == _pCollider1->GetColliderType() && COLLIDER_TYPE::MESH == _pCollider2->GetColliderType())
	{
		//return CollisionBox(_pCollider1, _pCollider2);
		return CollisionSphere(_pCollider1, _pCollider2);

	}
	else if (COLLIDER_TYPE::MESH == _pCollider1->GetColliderType() && COLLIDER_TYPE::BOX == _pCollider2->GetColliderType())
	{
		return CollisionSphere(_pCollider1, _pCollider2);
	}
	else if (COLLIDER_TYPE::BOX == _pCollider1->GetColliderType() && COLLIDER_TYPE::MESH == _pCollider2->GetColliderType())
	{
		return CollisionSphere(_pCollider1, _pCollider2);
	}
	else if (COLLIDER_TYPE::RECT == _pCollider1->GetColliderType() && COLLIDER_TYPE::RECT == _pCollider2->GetColliderType())
	{
		return CollisionRect(_pCollider1, _pCollider2);
	}
	else if (COLLIDER_TYPE::MESH == _pCollider1->GetColliderType() && COLLIDER_TYPE::RANGE == _pCollider2->GetColliderType())
	{
		return CollisionSphere(_pCollider1, _pCollider2);
	}
	else
	{
		return CollisionRectCircle(_pCollider1, _pCollider2);
	}

	return false;
}

// 3D
bool CCollisionMgr::CollisionSphere(CCollider* _pCollider1, CCollider* _pCollider2)
{
	if ((_pCollider1->GetName() == L"Item1" || _pCollider1->GetName() == L"Item2" || _pCollider1->GetName() == L"Item3"))
	{
		int a = 0;
	}
	BoundingSphere bBX1 = _pCollider1->GetBoundingSphere();
	BoundingSphere bBX2 = _pCollider2->GetBoundingSphere();

	if (bBX1.Contains(bBX2) == 1)
	{
		//	바운딩 구 중점, 크기 check용 cout.
		wstring a = _pCollider1->GetObj()->GetName();
		wstring b = _pCollider2->GetObj()->GetName();
		return true;
	}
	return false;
}

//bool CCollisionMgr::CollisionBoxSphere(CCollider* _pCollider1, CCollider* _pCollider2)
//{
//	BoundingSphere bBX1 = _pCollider1->GetBoundingSphere();
//	BoundingSphere bBX2 = _pCollider2->GetBoundingSphere();
//
//	if (bBX1.Contains(bBX2) == 1)
//	{
//		//	바운딩 구 중점, 크기 check용 cout.
//		wstring a = _pCollider1->GetObj()->GetName();
//		wstring b = _pCollider2->GetObj()->GetName();
//		return true;
//	}
//	return false;
//}


bool CCollisionMgr::CollisionBox(CCollider* _pCollider1, CCollider* _pCollider2)
{
	BoundingBox bBX1 = _pCollider1->GetBoundingBox();
	BoundingBox bBX2 = _pCollider2->GetBoundingBox();

	if (bBX1.Contains(bBX2) == 1)
	{
		//	바운딩 박스 중점, 크기 check용 cout.
		wstring a = _pCollider1->GetObj()->GetName();
		wstring b = _pCollider2->GetObj()->GetName();
	
		return true;
	}

	return false;
}

// 2D
bool CCollisionMgr::CollisionRect(CCollider* _pCollider1, CCollider* _pCollider2)
{
	static Vector3 arrLocal[4] = {					// 0 -- 1
		  Vector3(-0.5f, 0.5f, 0.f)					// |	|
		, Vector3(0.5f, 0.5f, 0.f)					// 3 -- 2
		, Vector3(0.5f, -0.5f, 0.f)
		, Vector3(-0.5f, -0.5f, 0.f) };


	const Matrix& matCol1 = _pCollider1->GetColliderWorldMat();
	const Matrix& matCol2 = _pCollider2->GetColliderWorldMat();

	Vector3 arrCol1[4] = {};
	Vector3 arrCol2[4] = {};
	Vector3 arrCenter[2] = {};

	for (UINT i = 0; i < 4; ++i)
	{
		arrCol1[i] = XMVector3TransformCoord(arrLocal[i], matCol1);
		arrCol2[i] = XMVector3TransformCoord(arrLocal[i], matCol2);

		// 2D 충돌이기 때문에 같은 Z 좌표상에서 충돌을 계산한다.
		arrCol1[i].z = 0.f;
		arrCol2[i].z = 0.f;
	}

	arrCenter[0] = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matCol1);
	arrCenter[1] = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matCol2);
	arrCenter[0].z = 0.f;
	arrCenter[1].z = 0.f;

	Vector3 vCenter = arrCenter[1] - arrCenter[0];

	Vector3 arrOriginVec[4] = { arrCol1[3] - arrCol1[0]
		, arrCol1[1] - arrCol1[0]
		, arrCol2[3] - arrCol2[0]
		, arrCol2[1] - arrCol2[0]
	};

	Vector3 arrProjVec[4] = {};
	for (UINT i = 0; i < 4; ++i)
	{
		arrOriginVec[i].Normalize(arrProjVec[i]);
	}


	// 투영을 통해서 분리축 테스트
	// vCenter		 두 사각형의 중심을 잇는 벡터
	// arrOriginVec  각 사각형의 표면 벡터
	// arrProjVec    사각형의 표면과 평행한 투영축 벡터(단위벡터)

	for (UINT i = 0; i < 4; ++i)
	{
		float fCenter = abs(vCenter.Dot(arrProjVec[i])); // 중심 거리 벡터를 해당 투영축으로 투영시킨 길이

		float fAcc = 0.f;
		for (UINT j = 0; j < 4; ++j)
			fAcc += abs(arrOriginVec[j].Dot(arrProjVec[i]));

		fAcc /= 2.f;

		if (fCenter > fAcc)
			return false;
	}

	return true;
}

bool CCollisionMgr::CollisionCircle(CCollider* _pCollider1, CCollider* _pCollider2)
{
	return false;
}

bool CCollisionMgr::CollisionRectCircle(CCollider* _pCollider1, CCollider* _pCollider2)
{
	return false;
}

void CCollisionMgr::SetBoundingSphereCollisionMesh(float _fRadius, wstring _fName)
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

	pMesh->SetName(_fName);
	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

}

void CCollisionMgr::SetBoundingCubeCollisionMesh(Vector4* vecMMax , wstring _fName)
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
	pMesh->SetName(_fName);
	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

}
