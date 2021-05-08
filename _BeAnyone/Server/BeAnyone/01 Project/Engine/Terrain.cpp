#include "pch.h"
#include "Terrain.h"

#include "ResMgr.h"
#include "MeshRenderer.h"

#include "KeyMgr.h"
#include "Texture.h"

#include "RenderMgr.h"
#include "Camera.h"
#include "Transform.h"


CTerrain::CTerrain()
	: CComponent( COMPONENT_TYPE::TERRAIN )
	, m_iFaceX( 64 )	//	64
	, m_iFaceZ( 64 )	//	64
	, m_fMaxTess( 4 )	//	4
	, m_vBrushScale( Vec2( 0.1f, 0.1f ) )
	, m_iBrushIdx( 0 )
	, m_iTileIdx( 0 )

{
}

CTerrain::~CTerrain() {
	if (m_pHeightMapPixels != nullptr)
		delete[]m_pHeightMapPixels;
}



void CTerrain::update() {
}

void CTerrain::finalupdate() {
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr != pMainCam)
	{
		Vec4 vPos = Vec4( pMainCam->Transform()->GetLocalPos(), 0.f );
		MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::VEC4_0, &vPos );
	}

}


void CTerrain::SaveToScene( FILE* _pFile ) {
}

void CTerrain::LoadFromScene( FILE* _pFile ) {
}