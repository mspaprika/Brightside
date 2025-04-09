#pragma once


enum class LightType
{
	DIRECT_1 = 0,
	DIRECT_2,
	PLAYER_SPOT,
	ENEMY_SPOT,

	TOTAL_LIGHTS,
};


class LightManager
{
	LightManager();
	~LightManager();

	PLAY_SINGLETON_INTERFACE( LightManager );

public:
	void SetupLighting();
	void UpdateDirectionalLights();
	void SetSpotlightColour( LightType type, ColourValue col, float intensity = -1.0f );
	void SetSpotlightDirection( LightType type, Vector3f dir );
	void SetSpotlightPosition( LightType type, Vector3f pos );
	void SetSpotlightDefaults( LightType type );
	void TurnOffLight( LightType type );

private:
	Vector3f m_dirLight1{ 1.0f, 1.0f, 1.0f };
	Vector3f m_dirLight2{ -1.0f, 1, 1.f };
};
