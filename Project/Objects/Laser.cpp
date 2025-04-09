#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Collisions/CollisionManager.h"
#include "ObjectFactory/GameObjectManager.h"
#include "Utilities/SoundManager.h"
#include "Laser.h"
#include "RenderContext.h"


Laser::Laser( const Matrix4x4f& world)
{
	m_materialId = AssetManager::Instance().GetAsset( AssetType::TYPE_LASER ).mat;
	m_meshId = AssetManager::Instance().GetAsset( AssetType::TYPE_LASER ).mesh;
	m_transform =  world ;
	m_speed = 3.0f;
	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), 0.05f );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere(m_pCol);
}

Laser::~Laser()
{
	CollisionManager::Instance().ReleaseCollidable( m_pCol );
}

void Laser::Update( float dT )
{
	ManageLifetime( dT );
	ManageActivity( dT, 0.2f );
	OrbitalRotation( dT );

	m_pCol->UpdateBoundPosition( GetPosition() ); 
}

void Laser::ManageLifetime( float dT )
{
	m_life += dT;
	if ( m_life > m_maxLife || !GameObjectManager::Instance().GetPlayer()->IsGameActive() ) DestroySelf();
}

void Laser::ResolveCollision( GameObject* obj, float overlap )
{
	CollidableObject* colObj = ( CollidableObject* )obj;

	switch ( obj->GetType() )
	{
		case GameObjectType::TYPE_VOLCANO:
		case GameObjectType::TYPE_DEBRIS:
			SoundManager::Instance().PlaySound( SoundType::LASER_HIT );
			DestroySelf();
			break;

		case GameObjectType::TYPE_ASTEROID:
			if ( m_active ) 
			{
				DestroySelf();
				SoundManager::Instance().PlaySound( SoundType::EXPLOSION );
			}
			else SoundManager::Instance().PlaySound( SoundType::LASER_HIT );
			break;
	}
}

void Laser::Render( RenderContext& ctx ) const
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
