#pragma once
#include "ObjectFactory/CollidableObject.h"


class RenderContext;
class Collidable;
class SpaceshipEmission;

class Player : public CollidableObject
{
public:
	Player(); // construct the player
	~Player() override; // destruct the player
	void Update( float dT ) override;  // Update the player
	void Render( RenderContext& ctx ) const override; // REnder the player using render context object 
	void ResolveCollision( GameObject* obj, float overlap ) override;
	void BounceOff( GameObject* obj, float overlap ) override;
	GameObjectType GetType() const override { return GameObjectType::TYPE_PLAYER; };

	// Movement
	Vector3f UpdateVelocity(); // update the velocity vector and retun right direction
	Vector3f ButtonControlls(); // return delta velocity
	Vector3f MouseControlls(); // return delta velocity
	void MouseRotation();
	void SetupMovement();

	// Update Game Play
	void UpdateGameOver();
	void UpdateSpotlight();
	void CreateLasers();
	void CreateSpaceshipEmitter();
	void CreateBlast();
	void UpdateBlast( float dT );
	void UpdateParticleEmitters( float dT );

	// Getters
	const Vector3f& GetVelocity() const { return m_velocity; }
	const Matrix4x4f& GetWorldMtx() const { return m_transform; }
	bool IsBlastActive() const { return m_blastActive; }
	bool IsGameActive() const { return m_gameActive; }
	bool IsPowerUpActive( PowerUpType type ) const;
	float GetHealth() const { return m_health; }
	float GetPowerUpTimer( PowerUpType type ) const;
	u32 GetScore() const { return m_score; }

	// Setters
	void UpdateWorld( float dT, const Vector3f& dV );
	void UpdateCameraInverseMtx( const Matrix4x4f& view, const Matrix4x4f& proj );
	void SetGameActive( bool active ) { m_gameActive = active; }
	void SetSpeed( float increase ) { m_maxSpeed += increase; };
	void AddHealth( float increase );
	void AddScore( u32 score ) { m_score += score; }
	void ActivatePowerUp( PowerUpType type );
	void UpdatePowerUps( float dT );

private:
	Matrix4x4f m_camInverseViewMtx;
	Matrix4x4f m_camInverseProjMtx;
	SpaceshipEmission* m_pEmitter{ nullptr };
	Collidable* m_pCollBlast{ nullptr };

	Vector3f m_velocity;
	u32 m_thrustIntensity{ 5 };
	u32 m_idleIntensity{ 2 };
	u32 m_score{ 0 };
	float m_maxSpeed{ 0.8f };
	float m_currentSpeed{ 0.0f };
	float m_health{ 5.0f };
	float m_blastRadius{ 0.0f };
	float m_blastTimer{ -1.0f };

	// flags
	bool m_shieldActive{ false };
	bool m_blastActive{ false };
	bool m_gameActive{ true };
};