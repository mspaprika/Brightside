#include "GameMath.h"
#include "Types.h"
#include "AssetManager.h"
#include "Collisions/CollisionManager.h"
#include "Player.h"
#include "Laser.h"
#include "RenderContext.h"
#include "ObjectFactory/GameObjectManager.h"
#include "Utilities/SoundManager.h"
#include "Utilities/ShakeManager.h"
#include "Utilities/LightManager.h"
#include "Particles/SpaceshipEmission.h"
#include "Particles/RingBlast.h"


enum PowerupStat
{
	FLAG,
	TIMER,
};

std::tuple<bool, float> g_powerupStats[ static_cast< int >(PowerUpType::TOTAL_POWERUPS) ]
{
	{ false, 0.0f },
	{ false, 0.0f },
	{ false, 0.0f },
};


Player::Player()
{
	m_materialId = AssetManager::Instance().GetAsset( AssetType::TYPE_PLAYER ).mat;
	m_meshId = AssetManager::Instance().GetAsset( AssetType::TYPE_PLAYER ).mesh;

	// because of the inverted mesh the cross product for RIGHT is flipped, also UP and POS z component is inverted
	Vector3f forward = { 0.f, 1.0f, 0.0f };
	Vector3f up = { 0.f, 0.0f, -1.0f };
	Vector3f right = cross( up, forward );
	Vector3f pos = { 0.f, 0.0f, -1.0f };

	m_transform = MatrixIdentity4x4f();
	m_transform.m_column[ 0 ] = { right, 0 };
	m_transform.m_column[ 1 ] = { up, 0 };
	m_transform.m_column[ 2 ] = { forward, 0 };
	m_transform.m_column[ 3 ] = { pos, 1 };
	m_velocity = forward;
	
	m_pCol = CollisionManager::Instance().CreateCollidable( this, GetPosition(), 0.3f );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCol );
}

Player::~Player()
{
	CollisionManager::Instance().ReleaseCollidable( m_pCol );
}

void Player::Update( float dT )
{
	UpdateGameOver();
	UpdatePowerUps( dT );
	if ( !m_gameActive ) return;

	static Vector3f deltaV{ 0.0f, 0.0f, 0.0f };
	MouseRotation(); // rotate the player towards mouse pos
	deltaV += MouseControlls(); // rotational movement ( using player's FORWARD )
	deltaV += ButtonControlls(); // directional movement ( using player's velocity and it's right )
	deltaV = normalize( deltaV );

	m_currentSpeed = DampingFactor( m_currentSpeed, dT, 0.2f );
	if ( m_currentSpeed < 0.01f ) deltaV = { 0.0f, 0.0f, 0.0f };
	if ( ShakeManager::Instance().IsActive() ) m_currentSpeed = 0.0f;
	
	UpdateWorld( dT, deltaV );
	UpdateParticleEmitters( dT );
	UpdateSpotlight();

	m_pCol->UpdateBoundPosition( GetPosition() ); // update collidable
}

void Player::UpdateGameOver()
{
	if ( m_health < 0 ) m_health = 0.0f;
	if ( !m_gameActive && m_pEmitter != nullptr )		
	{
		m_pEmitter->DestroySelf();
		m_pEmitter = nullptr;
	}
}

void Player::UpdateWorld( float dT, const Vector3f& dV )
{
	// calculate new POS for player
	Vector3f newPosition = normalize( GetPosition() + (dV * m_currentSpeed * dT) );
	// calculate new RIGHT ( perpendicular to UP and FORWARD )
	Vector3f newRight = cross( newPosition, GetForward() );
	// calculate new FORWARD ( perpendicular to RIGHT and UP )
	Vector3f newForward = cross( newRight, newPosition );

	// update RIGHT 
	SetRight( normalize(newRight) );
	// set the UP vector which is equal to POS
	SetUp( newPosition );
	// upadate FORWARD
	SetForward( normalize(newForward) );
	// set normalized POS for player so it's attached to the sphere, then add scaled up vector to move it a bit above
	SetPosition( newPosition + ( normalize( GetUp() ) * kOffsetMultiplier) );
}

void Player::UpdateSpotlight()
{
	LightManager::Instance().SetSpotlightPosition( LightType::PLAYER_SPOT, GetPosition() + GetUp() * 1.2f );
	LightManager::Instance().SetSpotlightDirection( LightType::PLAYER_SPOT, -GetUp() );

	std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_SHIELD) ]) ?
		LightManager::Instance().SetSpotlightColour( LightType::PLAYER_SPOT, Colour::Cyan, 15.0f ) :
		LightManager::Instance().SetSpotlightDefaults( LightType::PLAYER_SPOT );
}

Vector3f Player::ButtonControlls()
{
	Vector3f deltaV{ 0.0f, 0.0f, 0.0f };
	Vector3f vRight = UpdateVelocity();

	if ( Input::IsKeyDown( VK_RIGHT ) )
	{
		deltaV += vRight;
		SetupMovement();
	}
	if ( Input::IsKeyDown( VK_LEFT ) )
	{
		deltaV -= vRight;
		SetupMovement();
	}
	if ( Input::IsKeyDown( VK_UP ) )
	{
		deltaV += m_velocity;
		SetupMovement();
	}
	if ( Input::IsKeyDown( VK_DOWN ) )
	{
		deltaV -= m_velocity;
		SetupMovement();
	} 
	if ( Input::IsKeyPressed( VK_SPACE ) )
	{
		CreateBlast();
	}
	return deltaV;
}

Vector3f Player::UpdateVelocity()
{
	// update player's velocity vector ( right is perpendicular to old velocity and UP )
	Vector3f vRight = cross( normalize( m_velocity ), GetUp() );
	// correct velocity ( forward is perpendicular to new up and new right )
	m_velocity = cross( GetUp(), vRight );

	return normalize( vRight );
}

void Player::MouseRotation()
{
	Input::MouseState mouseState = Input::GetMouseState();
	float screenWidth = static_cast< float >(Graphics::GetDisplaySurfaceSize().m_width);
	float screenHeight = static_cast< float >(Graphics::GetDisplaySurfaceSize().m_height);
		
	Vector4f nearPlane = 
	{
		(mouseState.m_x / screenWidth - 0.5f) * 2.0f,
		(mouseState.m_y / screenHeight - 0.5f) * -2.0f,
		0.0f,
		0.0f
	};

	// convert nearplane mouse pos into view space	
	Vector4f nearPlaneViewSpace = Transform( m_camInverseProjMtx, nearPlane );
	// convert near plane mouse pos into world space
	Vector4f nearPlaneWorldSpace = Transform( m_camInverseViewMtx, nearPlaneViewSpace );
	// mouse ray is just a normalized mouse world pos
	Vector3f mouseRay = normalize( nearPlaneWorldSpace.xyz() );

	// recalculate all RIGHT and FORWARD for player 
	Vector3f forward = mouseRay;
	Vector3f right = cross( GetUp(), forward );

	SetRight( right );
	SetForward( forward );
}

Vector3f Player::MouseControlls()
{
	Vector3f deltaV{ 0.0f, 0.0f, 0.0f };
	Input::MouseState mouseState = Input::GetMouseState();
	
	static bool leftButtonPressed{ false };

	if ( mouseState.m_leftButton )
	{
		if ( !leftButtonPressed )
		{
			CreateLasers();
			leftButtonPressed = true;
		}	
	}
	else leftButtonPressed = false;

	if ( mouseState.m_rightButton ) 
	{
		deltaV += GetForward();
		SetupMovement();
	}
	return deltaV;
}

void Player::SetupMovement()
{
	m_currentSpeed = m_maxSpeed;
	m_pEmitter->UpdateBatch( m_thrustIntensity );
}

void Player::ResolveCollision( GameObject* obj, float overlap )
{
	switch ( obj->GetType() )
	{
		case GameObjectType::TYPE_ASTEROID:
			if ( m_gameActive && !m_blastActive )
			{
				if ( !std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_SHIELD) ]) ) m_health -= 0.1f;
				SoundManager::Instance().PlaySound( SoundType::CRASH );
			}
			break;

		case GameObjectType::TYPE_VOLCANO:
			if ( m_gameActive && !m_blastActive ) BounceOff( obj, overlap );
			break;

		case GameObjectType::TYPE_MISSILE:
			if ( m_gameActive && !std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_SHIELD) ]) ) m_health -= 0.5f;
			break;

		default: 
			break;
	}
}

void Player::BounceOff( GameObject* obj, float overlap )
{
	Vector3f bounceDirection = normalize( GetPosition() - obj->GetPosition() );
	SetPosition( GetPosition() + bounceDirection * overlap * 2.0f );
}

void Player::CreateLasers()
{
	GameObjectManager::Instance().CreateGameObject< Laser >(m_transform);
	GameObjectManager::Instance().CreateGameObject< Laser >(m_transform  * MatrixRotationY( 0.2f ));
	GameObjectManager::Instance().CreateGameObject< Laser >(m_transform * MatrixRotationY( -0.2f ));
	SoundManager::Instance().PlaySound( SoundType::LASER );
}

void Player::CreateSpaceshipEmitter()
{
	m_pEmitter = GameObjectManager::Instance().CreateGameObject< SpaceshipEmission >(ParticleType::TYPE_STAR, m_idleIntensity, GetRight(), GetPosition(), -GetForward(), 0.6f);
}

void Player::CreateBlast()
{
	GameObjectManager::Instance().CreateGameObject< RingBlast >(ParticleType::TYPE_SCORCH, GetPosition(), GetRight(), 300, 1.0f);
	m_pCollBlast = CollisionManager::Instance().CreateCollidable( this, GetPosition(), m_blastRadius + 0.1f, true );
	if ( m_debugMode ) CollisionManager::Instance().CreateDebugSphere( m_pCollBlast );
	SoundManager::Instance().PlaySound( SoundType::BLAST );
	ShakeManager::Instance().ShakePlayerCamera( 0.05f, 0.7f );
	m_blastActive = true;
	m_blastTimer = 0.01f;
}

void Player::ActivatePowerUp( PowerUpType type )
{
	std::get< FLAG >(g_powerupStats[ static_cast< int >(type) ]) = true;
	std::get< TIMER >(g_powerupStats[ static_cast< int >(type) ]) = 0.0f;
}

void Player::UpdatePowerUps( float dT )
{
	for (int i = 0; i < static_cast< int >(PowerUpType::TOTAL_POWERUPS); i++)
	{
		if ( std::get< FLAG >(g_powerupStats[ i ]) ) std::get< TIMER >(g_powerupStats[ i ]) += dT;
		if ( !m_gameActive || std::get< TIMER >(g_powerupStats[ i ]) > kPowerUpDuration )
		{
			std::get< TIMER >(g_powerupStats[ i ]) = 0.0f;
			std::get< FLAG >(g_powerupStats[ i ]) = false;
		}
	}

	m_shieldActive = std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_SHIELD) ]);
	m_maxSpeed = (std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_SPEED) ])) ? 2.0f : 1.2f;

	if (std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_HEALTH) ]) 
		&& std::get< TIMER >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_HEALTH) ]) > 2.0f )
	{
		std::get< FLAG >(g_powerupStats[ static_cast< int >(PowerUpType::TYPE_HEALTH) ]) = false;
	}
}

void Player::AddHealth( float increase )
{ 
	m_health += increase;
	if ( m_health > kMaxHealth ) m_health = kMaxHealth;
}

bool Player::IsPowerUpActive( PowerUpType type ) const 
{
	return std::get< FLAG >(g_powerupStats[ static_cast< int >(type) ]);
}

float Player::GetPowerUpTimer( PowerUpType type ) const
{
	return std::get< TIMER >(g_powerupStats[ static_cast< int >(type) ]);
}

void Player::UpdateBlast( float dT )
{
	if ( m_blastTimer > 0.7f )
	{
		m_blastTimer = -1.0f;
		m_blastActive = false;
		CollisionManager::Instance().ReleaseCollidable( m_pCollBlast );
		m_pCollBlast = nullptr;
		m_blastRadius = 0.0f;
	}

	else if ( m_blastTimer > 0.0f ) 
	{
		m_blastTimer += dT;
		m_blastRadius += 0.05f;
		m_pCollBlast->UpdateSphereRadius( m_blastRadius );
	}
}

void Player::UpdateParticleEmitters( float dT )
{
	if ( m_pEmitter == nullptr ) CreateSpaceshipEmitter();

	// update spaceship emitter
	m_pEmitter->UpdatePosition( GetPosition() );
	m_pEmitter->UpdateVelocity( -GetForward() );
	m_pEmitter->UpdateRight(GetRight());
	if ( m_currentSpeed < (m_maxSpeed / 1.5f) ) m_pEmitter->UpdateBatch( m_idleIntensity );

	UpdateBlast( dT );
}

void Player::Render( RenderContext& ctx ) const
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

void Player::UpdateCameraInverseMtx( const Matrix4x4f& view, const Matrix4x4f& proj )
{
	m_camInverseViewMtx = view;
	m_camInverseProjMtx = proj;
}