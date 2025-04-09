#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Planet.h"
#include "RenderContext.h"


Planet::Planet()
{
	m_materialId = AssetManager::Instance().GetAsset( AssetType::TYPE_PLANET ).mat;
	m_meshId = AssetManager::Instance().GetAsset( AssetType::TYPE_PLANET ).mesh;
	m_transform = MatrixScale( Vector3f(0.09f, 0.09f, 0.09f) );
}

Planet::~Planet()
{

}

void Planet::Update( float dT )
{

}

void Planet::Render( RenderContext& ctx ) const
{
	if ( ctx.GetRenderPassType() != RenderPassType::kDebugPass )
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