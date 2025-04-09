#include "GameMath.h"
#include "Collisions/CollisionManager.h"
#include "ExampleGameObject.h"
#include "AssetManager.h"
#include "RenderContext.h"


ExampleGameObject::ExampleGameObject( Collidable* col )
{
	m_meshId = Graphics::CreateSphere( col->GetBoundSphere().radius, 20, 20, Colour::White );
	m_materialId = AssetManager::Instance().GetSphereMaterial();
	m_pColidable = col;
	m_transform = MatrixTranslate( col->GetBoundSphere().position );
}

ExampleGameObject::~ExampleGameObject()
{
	CollisionManager::Instance().ReleaseCollidable( m_pColidable );
}

void ExampleGameObject::Update( float dT )
{
	SetPosition( m_pColidable->GetBoundSphere().position );
	m_meshId = Graphics::CreateSphere( m_pColidable->GetBoundSphere().radius, 30, 30, Colour::White );
}

void ExampleGameObject::Render( RenderContext& ctx ) const
{
	if ( ctx.GetRenderPassType() != RenderPassType::kDebugPass )
	{
		MeshRenderCommand cmd;
		cmd.meshId = m_meshId;
		cmd.materialId = m_materialId;
		cmd.worldMatrix = m_transform;
		ctx.RenderMesh(cmd);
	}
	else
	{
		AxisRenderCommand cmd;
		cmd.worldMatrix = m_transform;
		ctx.RenderAxis(cmd);
	}
}
