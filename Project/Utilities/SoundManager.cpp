#include "GameMath.h"
#include "Types.h"
#include "SoundManager.h"


enum SoundParams
{
	SOUND_PATH, 
	LOOPING,
};

std::tuple< const char*, bool > g_sounds[ static_cast< int >(SoundType::TOTAL_SOUNDS) ]
{
	{ "Data/Audio/BGM.wav", true },
	{ "Data/Audio/Ambience.wav", true },
	{ "Data/Audio/lazer.wav", false },
	{ "Data/Audio/LazerHit.wav", false },
	{ "Data/Audio/HumShort.wav", false },
	{ "Data/Audio/Crash.wav", false },
	{ "Data/Audio/Explosion.wav", false },
	{ "Data/Audio/powerup.wav", false },
	{ "Data/Audio/Blast.wav", false },
};



PLAY_SINGLETON_IMPL( SoundManager );

SoundManager::SoundManager()
{
	LoadSounds();
}

SoundManager::~SoundManager()
{

}

void SoundManager::LoadSounds()
{
	for (int i = 0; i < static_cast< int >(SoundType::TOTAL_SOUNDS); i++)
	{
		m_soundIdList[ i ] = Audio::LoadSoundFromFile( std::get< SOUND_PATH >(g_sounds[ i ]), std::get< LOOPING >(g_sounds[ i ]) );
	}
}

void SoundManager::Update()
{
	if ( Input::IsKeyDown('1') && m_background )
	{
		m_currentVolume -= 0.1f;
		Audio::SetGain( m_voiceIdBack, m_currentVolume );
	}
	if ( Input::IsKeyDown('2') && m_background )
	{
		m_currentVolume += 0.1f;
		Audio::SetGain( m_voiceIdBack, m_currentVolume );
	}

	if ( Input::IsKeyPressed('3') && m_sound ) m_sound = false;
	else if ( Input::IsKeyPressed('3') && !m_sound ) m_sound = true;

	if ( !m_gameOver && m_sound && !m_background ) PlayBackgroundSound();
	else if ( !m_gameOver && !m_sound && m_background ) StopBackgroundSound();
}

void SoundManager::PlayBackgroundSound()
{
	m_voiceIdBack = Audio::PlaySound( m_soundIdList[ static_cast< int >(SoundType::BACKGROUND) ], m_baseVolume );
	m_background = true;
}

void SoundManager::StopBackgroundSound() 
{ 
	Audio::StopSound( m_voiceIdBack ); 
	m_background = false;
}

void SoundManager::PlaySound( SoundType sound ) 
{
	if ( m_sound ) m_voiceId = Audio::PlaySound( m_soundIdList[ static_cast< int >(sound) ], m_currentVolume );
}




