#pragma once
#include "ObjectFactory/CollidableObject.h"


class RenderContext;

class PowerUp : public CollidableObject
{
public:
	PowerUp( const Matrix4x4f& world, PowerUpType type );
	~PowerUp() override;
	void Update( float dT ) override;
	void Render( RenderContext& ctx ) const override;
	void ResolveCollision( GameObject* obj, float overlap ) override;

	GameObjectType GetType() const override { return GameObjectType::TYPE_POWERUP; };
	AssetType ConvertType( PowerUpType type );
	void ActivatePlayerPowerUp();

private:
	PowerUpType m_powerupType;
};