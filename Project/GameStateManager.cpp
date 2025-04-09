#include "GameMath.h"
#include "Types.h"
#include "Level.h"
#include "Collisions/CollisionManager.h"
#include "Particles/TwinkleLittleStar.h"
#include "Cameras/CameraManager.h"
#include "Cameras/PlayerCamera.h"
#include "Cameras/DebugCamera.h"
#include "Objects/Planet.h"
#include "Objects/Enemy.h"
#include "Objects/Player.h"
#include "Utilities/LightManager.h"
#include "Utilities/SoundManager.h"
#include "Utilities/ShakeManager.h"
#include "AssetManager.h"
#include "ObjectFactory/GameObjectManager.h"
#include "GameStateManager.h"


PLAY_SINGLETON_IMPL( GameStateManager );


GameStateManager::GameStateManager()
{
	GameStateManager::Instance().InitialiseManagers();
	GameStateManager::Instance().CreateDebugCamera();
	AssetManager::Instance().LoadGameAssets();
	LightManager::Instance().SetupLighting();
	m_debugFontId = UI::GetDebugFont();
}

GameStateManager::~GameStateManager()
{

}

void GameStateManager::Update( float dT )
{
	UpdateGameState();
	CameraControls();
	SoundManager::Instance().Update();
	CollisionManager::Instance().UpdateCollisions();
	GameObjectManager::Instance().Update(dT);
	CameraManager::Instance().Update();
	LightManager::Instance().UpdateDirectionalLights();
	GameObjectManager::Instance().PostUpdate();
}

void GameStateManager::UpdateGameState()
{
	SoundManager::Instance().SetGameOver( m_gameOver );
	if ( GameObjectManager::Instance().GetPlayer()->GetHealth() <= 0.0f )
	{
		GameOver();
		m_gameScore = 0;
	}
	if ( GameObjectManager::Instance().CollectGameObjectsByType( GameObjectType::TYPE_ASTEROID ).size() < 1 )
	{
		GameOver();
		m_levelPassed = true;
		
	}
	if ( m_gameOver && Input::IsKeyPressed( VK_RETURN ) )
	{
		m_gLevel = m_levelPassed ? m_gLevel + 1 : LEVEL_1;
		m_gameOver = false;
		m_levelPassed = false;
		m_gameScore += GameObjectManager::Instance().GetPlayer()->GetScore();
		DestroyLevel();
		GenerateLevel();
	}
}

void GameStateManager::GameOver()
{
	m_gameOver = true;
	GameObjectManager::Instance().GetEnemy()->SetGameActive( false );
	GameObjectManager::Instance().GetPlayer()->SetGameActive( false );

	if ( SoundManager::Instance().IsBackgroundMusicOn() ) SoundManager::Instance().StopBackgroundSound();
}

void GameStateManager::DrawUI()
{
	float middleWidth = static_cast< float >(Graphics::GetDisplaySurfaceSize().m_width) / 2;
	float bottomHeight = static_cast< float >(Graphics::GetDisplaySurfaceSize().m_height) - 20;

	UI::DrawPrintf( m_debugFontId, Vector2f( middleWidth, 30 ), Colour::White, "Health: %.1f", GameObjectManager::Instance().GetPlayer()->GetHealth() );
	UI::DrawPrintf( m_debugFontId, Vector2f( middleWidth + 400, 30 ), Colour::White,"Score: %d", GameObjectManager::Instance().GetPlayer()->GetScore() + m_gameScore );
	UI::DrawPrintf( m_debugFontId, Vector2f( middleWidth + 700, 30 ), Colour::White, "Level: %d", m_gLevel );
	UI::DrawPrintf( m_debugFontId, Vector2f( middleWidth + 700, bottomHeight ), Colour::White, "Sound:  %s", SoundManager::Instance().IsSoundOn() ? "ON" : "OFF");

	if (!m_gameOver && GameObjectManager::Instance().GetPlayer()->IsPowerUpActive( PowerUpType::TYPE_SHIELD ))
	{
		UI::DrawPrintf(m_debugFontId, Vector2f( middleWidth, 130 ), Colour::Lightcyan, "Shield active %.1fs", kPowerUpDuration - GameObjectManager::Instance().GetPlayer()->GetPowerUpTimer(PowerUpType::TYPE_SHIELD));
	}
	if (!m_gameOver && GameObjectManager::Instance().GetPlayer()->IsPowerUpActive( PowerUpType::TYPE_SPEED ))
	{
		UI::DrawPrintf(m_debugFontId, Vector2f( middleWidth, 160 ), Colour::Lightcyan, "Power speed %.1fs", kPowerUpDuration - GameObjectManager::Instance().GetPlayer()->GetPowerUpTimer(PowerUpType::TYPE_SPEED));
	}
	if (!m_gameOver && GameObjectManager::Instance().GetPlayer()->IsPowerUpActive( PowerUpType::TYPE_HEALTH ))
	{
		UI::DrawPrintf(m_debugFontId, Vector2f( middleWidth + 20, 100), Colour::Lightcyan,"Health + 0.5");
	}
	if ( m_gameOver )
	{
		UI::DrawString( m_debugFontId, Vector2f( middleWidth, 100 ), Colour::Cyan, m_levelPassed ? "You Won!!!" : "Game Over" );
		UI::DrawString( m_debugFontId, Vector2f( middleWidth - 100, 150 ), Colour::Cyan, m_levelPassed ? "Press Enter To Continue": "Press Enter To Start New Game" );
	}
}

void GameStateManager::GenerateLevel()
{
	GameObjectManager::Instance().CreateGameObject< TwinkleLittleStar >(600);
	GameObjectManager::Instance().CreateGameObject< TwinkleLittleStar >(250, g_vParticleColours[ static_cast< int >(Color::L_YELLOW) ]);
	GameObjectManager::Instance().CreateGameObject< TwinkleLittleStar >(150, g_vParticleColours[ static_cast< int >(Color::L_GREEN) ]);

	GameObjectManager::Instance().CreateGameObject< Planet >();
	Player* p = GameObjectManager::Instance().CreateGameObject< Player >();
	CreatePlayerCamera( p );

	int level = m_gLevel > LEVEL_3 ? LEVEL_DEFAULT : m_gLevel;
	GameObjectManager::Instance().CreateGameObject< Enemy >( std::get< SHOOT_INTERVAL >(g_levelConstants[ level ]), std::get< ENEMY_SPEED >(g_levelConstants[ level ]));
	GameObjectManager::Instance().CreateAsteroids( AsteroidType::TYPE_LARGE, std::get< ASTEROID_COUNT >(g_levelConstants[ level ]));
	GameObjectManager::Instance().CreateVolcanos( VolcanoType::TYPE_TALL, std::get< VOLCANO_COUNT >(g_levelConstants[ level ]));
	GameObjectManager::Instance().CreateVolcanos( VolcanoType::TYPE_CRATER, std::get< CRATER_COUNT >(g_levelConstants[ level ]));

	if ( SoundManager::Instance().IsSoundOn() ) SoundManager::Instance().PlayBackgroundSound();
}

void GameStateManager::DestroyLevel()
{
	CameraManager::Instance().DestroyPlayerCamera();
	GameObjectManager::Instance().DestroyAllObjects();
	CollisionManager::Instance().ReleaseAllCollidables();
}

void GameStateManager::CreateDebugCamera()
{
	DebugCamera* pDebugCamera = new DebugCamera;
	u32 id = CameraManager::Instance().RegisterCamera( pDebugCamera );
	CameraManager::Instance().SetDebugCamID( id );
}

void GameStateManager::CreatePlayerCamera( Player* p )
{
	PlayerCamera* pPlayerCamera = new PlayerCamera;
	ShakeManager::Instance().SetPlayerCamera(pPlayerCamera);
	u32 id = CameraManager::Instance().RegisterCamera( pPlayerCamera );
	CameraManager::Instance().SetPlayerCamID( id );
	CameraManager::Instance().SetActiveCamera( id );
	pPlayerCamera->SetPlayer( p );
}

void GameStateManager::InitialiseManagers()
{
	CollisionManager::Initialise();
	GameObjectManager::Initialise();
	CameraManager::Initialise();
	AssetManager::Initialise();
	LightManager::Initialise();
	ShakeManager::Initialise();
	SoundManager::Initialise();
}

void GameStateManager::DrawDebugAxes()
{
	Graphics::DrawAxes( MatrixTranslate( Graphics::GetLightPosition( 1 ) ), 1.0f );

	Player* p = GameObjectManager::Instance().GetPlayer();
	Graphics::DrawAxes( p->GetWorldMtx(), 1.0f );
}

void GameStateManager::CameraControls()
{
	if ( Input::IsKeyDown('P') )
	{
		CameraManager::Instance().SetActiveCamera( CameraManager::Instance().GetPlayerCamID() );
	} 
	if ( Input::IsKeyDown('C') )
	{
		CameraManager::Instance().SetActiveCamera( CameraManager::Instance().GetDebugCamID() );
	}
}


