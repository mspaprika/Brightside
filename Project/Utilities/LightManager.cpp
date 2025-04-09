#include "GameMath.h"
#include "Types.h"
#include "LightManager.h"


PLAY_SINGLETON_IMPL( LightManager );

ColourValue g_vLightColours[ kNumLights ] =
{
	 ColourValue((u8)128.f, (u8)0.0f, (u8)128.0f), // magenta
	 ColourValue((u8)128.f, (u8)128.0f, (u8)255.0f), // blue
	 ColourValue((u8)148.f, (u8)255.0f, (u8)255.0f), // light blue
	 Colour::White,
};

float g_vLightIntensity[ kNumLights ] =
{
	10.0f, 10.0f, 5.0f, 75.0f
};

Vector3f g_vLightDirections[ kNumLights ] =
{
	Vector3f(1, 1, 1), Vector3f(-1.0f, 1, 1.f), Vector3f(0, -1, -1), Vector3f(-1, -1, -1)
};

LightManager::LightManager()
{

}

LightManager::~LightManager()
{

}

void LightManager::SetupLighting()
{
	int lightID = static_cast< int >(LightType::DIRECT_1);
	Graphics::SetLightColour(lightID, g_vLightColours[ lightID ], g_vLightIntensity[ lightID ]);
	Graphics::SetLightDirection(lightID, g_vLightDirections[ lightID ]);
	Graphics::SetLightAttributes(lightID, Graphics::LightType::LIGHT_DIRECTIONAL);
	Graphics::SetShadowFactor(lightID, 1.0f);
	
	lightID = static_cast< int >(LightType::DIRECT_2);
	Graphics::SetLightDirection(lightID, g_vLightDirections[ lightID ]);
	Graphics::SetLightColour(lightID, g_vLightColours[ lightID ], g_vLightIntensity[ lightID ]);
	Graphics::SetLightAttributes(lightID, Graphics::LightType::LIGHT_DIRECTIONAL);
	Graphics::SetShadowFactor(lightID, 1.0f);

	lightID = static_cast< int >(LightType::PLAYER_SPOT);
	Graphics::SetLightDirection(lightID, g_vLightDirections[ lightID ]);
	Graphics::SetLightColour(lightID, g_vLightColours[ lightID ], g_vLightIntensity[ lightID ]);
	Graphics::SetLightAttributes(lightID, Graphics::LightType::LIGHT_SPOT, 15.0f * kfPi180, 12.5f * kfPi180);

	lightID = static_cast< int >(LightType::ENEMY_SPOT);
	Graphics::SetLightDirection(lightID, g_vLightDirections[ lightID ]);
	Graphics::SetLightColour(lightID, g_vLightColours[ lightID ], g_vLightIntensity[ lightID ]);
	Graphics::SetLightAttributes(lightID, Graphics::LightType::LIGHT_SPOT, 15.0f * kfPi180, 12.5f * kfPi180);
}

void LightManager::SetSpotlightColour( LightType type, ColourValue col, float intensity )
{
	int lightID = static_cast< int >(type);
	float lightIntensity = intensity < 0.0f ? g_vLightIntensity[ lightID ] : intensity;

	Graphics::SetLightColour( lightID, col, lightIntensity);
}

void LightManager::SetSpotlightDirection( LightType type, Vector3f dir )
{
	Graphics::SetLightDirection( static_cast< int >(type), dir );
}

void LightManager::SetSpotlightPosition( LightType type, Vector3f pos )
{
	Graphics::SetLightPosition( static_cast< int >(type), pos );
}

void LightManager::SetSpotlightDefaults( LightType type )
{
	int lightID = static_cast< int >(type);
	Graphics::SetLightColour( lightID, g_vLightColours[ lightID ], g_vLightIntensity[ lightID ]);
}

void LightManager::UpdateDirectionalLights()
{
	Vector3f up = { 0.0f, 1.0f, 0.0f };
	int lightID = static_cast< int >(LightType::DIRECT_1);

	Vector3f newDir = RotateAroundAxis( m_dirLight1, up, 0.8f );
	Graphics::SetLightDirection( lightID, -newDir );
	m_dirLight1 = normalize( newDir );

	lightID = static_cast< int >(LightType::DIRECT_2);
	newDir = RotateAroundAxis( m_dirLight2, up, 0.8f );
	Graphics::SetLightDirection( lightID, -newDir );
	m_dirLight2 = normalize( newDir );
}

void LightManager::TurnOffLight( LightType type )
{
	int lightID = static_cast< int >(type);
	Graphics::SetLightColour( lightID, g_vLightColours[ lightID ], 0.0f );
}

