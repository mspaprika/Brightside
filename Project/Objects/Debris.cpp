#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Debris.h"
#include "RenderContext.h"


Debris::Debris( const Matrix4x4f& world, AsteroidType type )
{
	m_materialId = AssetManager::Instance().GetAsset( ConvertType(type) ).mat;
	m_meshId = AssetManager::Instance().GetAsset( ConvertType(type) ).mesh;
	m_transform = world;
	SetPosition( GetPosition() + GetUp() * kOffsetMultiplier );
	m_speed = 0.1f;
	m_asteroidType = type;
	m_offsetMult = 0.5f;
}

Debris::~Debris()
{
	
}

void Debris::Update( float dT )
{
	OrbitalRotation( dT );

}

AssetType Debris::ConvertType( AsteroidType type ) const
{
	if ( type == AsteroidType::TYPE_DEBRIS_S ) return AssetType::TYPE_DEBRIS_S;
	return AssetType::TYPE_DEBRIS_M;
}

void Debris::Render( RenderContext& ctx ) const
{
	if ( ctx.GetRenderPassType() != RenderPassType::kDebugPass )
	{
		MeshRenderCommand cmd;
		cmd.meshId = m_meshId;
		cmd.materialId = m_materialId;
		cmd.worldMatrix = m_transform;
		ctx.RenderMesh( cmd );
	}
	else // collect commands only if right pass is being rendered
	{
		AxisRenderCommand cmd;
		cmd.worldMatrix = m_transform;
		ctx.RenderAxis( cmd );
	}
}