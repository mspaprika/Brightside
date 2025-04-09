#pragma once

class PlayerCamera;

class ShakeManager
{
	ShakeManager();
	~ShakeManager();

	PLAY_SINGLETON_INTERFACE( ShakeManager );

public:
	void SetPlayerCamera( PlayerCamera* c ) { m_pPlayerCam = c; }
	void ShakePlayerCamera( float intensity, float duration );
	bool IsActive();

private:
	PlayerCamera* m_pPlayerCam{ nullptr };
};
