#pragma once


enum class SoundType
{
	BACKGROUND,
	AMBIENCE,
	LASER,
	LASER_HIT,
	HUM,
	CRASH,
	EXPLOSION,
	POWERUP,
	BLAST,

	TOTAL_SOUNDS,
};

class SoundManager
{
	SoundManager();
	~SoundManager();

	PLAY_SINGLETON_INTERFACE( SoundManager );


public:
	
	void Update();
	void PlayBackgroundSound();
	void PlaySound( SoundType sound );
	void StopSound() { Audio::StopSound( m_voiceId ); }
	void StopBackgroundSound();
	bool IsBackgroundMusicOn() const { return m_background; }
	void SetBackgroundMusic( bool isOn ) { m_background = isOn; }
	void SetSound(bool isOn) { m_sound = isOn; }
	bool IsSoundOn() const { return m_sound; }
	void SetGameOver( bool isOver ) { m_gameOver = isOver; }

private:
	void LoadSounds();

	Audio::SoundId m_soundIdList[ static_cast< int >(SoundType::TOTAL_SOUNDS) ];
	Audio::VoiceId m_voiceId;
	Audio::VoiceId m_voiceIdBack;
	float m_baseVolume{ 0.7f };
	float m_currentVolume{ m_baseVolume };
	bool m_background{ true };
	bool m_sound{ true };
	bool m_gameOver{ false };
};

