#pragma once

class Player;

class GameStateManager
{
	GameStateManager();
	~GameStateManager();

	PLAY_SINGLETON_INTERFACE( GameStateManager );

public:
	void GenerateLevel();
	void InitialiseManagers();
	void CreateDebugCamera();
	void DrawDebugAxes();
	void CameraControls();
	void Update( float dT );
	void DrawUI();

private:
	void DestroyLevel();
	void GameOver();
	void UpdateGameState();
	void CreatePlayerCamera( Player* p );

	int m_gLevel{ LEVEL_1 };
	bool m_gameOver{ false };
	bool m_levelPassed{ false };
	u32 m_gameScore{ 0 };
	UI::FontId m_debugFontId;
};


