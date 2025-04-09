#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Collisions/CollisionManager.h"
#include "Volcano.h"
#include "RenderContext.h"


Volcano::Volcano( VolcanoType type, Matrix4x4f world )
{
	m_materialId = AssetManager::Instance().GetAsset( ConvertType(type) ).mat;
	m_meshId = AssetManager::Instance().GetAsset( ConvertType(type) ).mesh;
	m_transform = world;
	m_offsetMult = 0.2f;
	SetPosition( GetPosition() + GetUp() * m_offsetMult );

	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), 0.8f, true );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCol );
}

Volcano::~Volcano()
{

}

void Volcano::Update(float dT)
{

}

AssetType Volcano::ConvertType( VolcanoType type )
{
	if (type == VolcanoType::TYPE_TALL) return AssetType::TYPE_VOLCANO;
	else return AssetType::TYPE_CRATER;
}

void Volcano::Render( RenderContext& ctx ) const
{
	if (ctx.GetRenderPassType() != RenderPassType::kDebugPass)
	{
		MeshRenderCommand cmd; // create render command 
		cmd.meshId = m_meshId;
		cmd.materialId = m_materialId;
		cmd.worldMatrix = m_transform;
		ctx.RenderMesh(cmd); // add command to the render context
	}
	else
	{
		AxisRenderCommand cmd;
		cmd.worldMatrix = m_transform;
		ctx.RenderAxis(cmd);
	}
}