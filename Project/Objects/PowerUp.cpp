#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Collisions/CollisionManager.h"
#include "Utilities/SoundManager.h"
#include "ObjectFactory/GameObjectManager.h"
#include "PowerUp.h"
#include "RenderContext.h"


PowerUp::PowerUp( const Matrix4x4f& world, PowerUpType type )
{
	m_materialId = AssetManager::Instance().GetAsset( ConvertType(type) ).mat;
	m_meshId = AssetManager::Instance().GetAsset( ConvertType(type) ).mesh;
	m_transform = world;
	m_offsetMult = kOffsetMultiplier;
	SetPosition( GetPosition() + GetUp() * m_offsetMult );
	m_speed = 0.1f;
	m_powerupType = type;

	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), 0.1f );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCol );
}

PowerUp::~PowerUp()
{
	CollisionManager::Instance().ReleaseCollidable( m_pCol );
}

void PowerUp::Update(float dT)
{
	OrbitalRotation( dT );

	Vector3f offsetToPlayer = GameObjectManager::Instance().GetPlayer()->GetPosition() - GetPosition();
	float distanceFromPlayer = length( offsetToPlayer );

	if ( distanceFromPlayer < 1.2f && GameObjectManager::Instance().GetPlayer()->IsGameActive() )
	{
		SetForward( normalize(offsetToPlayer) );
		Vector3f newRight = cross(  GetUp(), GetForward() );
		SetRight( newRight );
		m_speed = 1.0f;
	}

	m_pCol->UpdateBoundPosition( GetPosition() );
}

void PowerUp::ResolveCollision( GameObject* obj, float overlap )
{
	switch (obj->GetType())
	{
		case GameObjectType::TYPE_ASTEROID:
		case GameObjectType::TYPE_VOLCANO:
		case GameObjectType::TYPE_ENEMY:
			BounceOff( obj, overlap );
			break;

		case GameObjectType::TYPE_PLAYER:
			if ( GameObjectManager::Instance().GetPlayer()->IsGameActive() )
			{
				SoundManager::Instance().PlaySound( SoundType::POWERUP );
				ActivatePlayerPowerUp();
				DestroySelf();
			}
			break;

		default:
			break;
	}
}

void PowerUp::ActivatePlayerPowerUp()
{
	switch ( m_powerupType )
	{
		case PowerUpType::TYPE_HEALTH:
			GameObjectManager::Instance().GetPlayer()->ActivatePowerUp( PowerUpType::TYPE_HEALTH );
			GameObjectManager::Instance().GetPlayer()->AddHealth( kHealthIncrease );
			break;

		case PowerUpType::TYPE_SHIELD:
		{
			GameObjectManager::Instance().GetPlayer()->ActivatePowerUp(PowerUpType::TYPE_SHIELD);
			break;
		}

		case PowerUpType::TYPE_SPEED:
			GameObjectManager::Instance().GetPlayer()->ActivatePowerUp( PowerUpType::TYPE_SPEED );
			break;
	}
}

AssetType PowerUp::ConvertType( PowerUpType type )
{
	if ( type == PowerUpType::TYPE_HEALTH ) return AssetType::TYPE_HEALTH;
	if ( type == PowerUpType::TYPE_SPEED ) return AssetType::TYPE_SPEED;
	return AssetType::TYPE_SHIELD;
}

void PowerUp::Render( RenderContext& ctx ) const
{
	if (ctx.GetRenderPassType() != RenderPassType::kDebugPass)
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