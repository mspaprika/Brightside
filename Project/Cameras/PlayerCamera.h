#pragma once
#include "CameraManager.h"
#include <random>

class Player;

class PlayerCamera : public Camera
{
public:
	PlayerCamera();
	void Update() override;
	void SetPlayer( Player* p );

	void ApplyShakeEffect(float dT);
	void Shake(float intensity, float duration);
	bool IsActive() { return m_shakeActive; }

private:
	Player* m_pPlayer{ nullptr };
	float m_offset{ 5.0f };

	std::uniform_real_distribution< float > m_distribution;
	std::default_random_engine m_generator;
	float m_shakeDuration{ 0.0f };
	float m_shakeIntensity{ 0.0f };
	float m_shakeTimer{ 0.0f };
	bool m_shakeActive{ false };

};