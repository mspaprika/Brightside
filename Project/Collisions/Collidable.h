#pragma once

class GameObject;

struct BoundSphere
{
	Vector3f position;
	float radius;
};


class Collidable
{
public:
	Collidable( GameObject* obj, const Vector3f pos, float radius, bool ignored = false );
	bool IsDestroyed() const { return m_destroyed; }
	void SetDestroyed( bool destroyed ) { m_destroyed = destroyed; }
	GameObject* GetGameObject() { return m_pObj; }
	const BoundSphere& GetBoundSphere() { return m_boundSphere; }
	void UpdateBoundPosition( Vector3f pos ) { m_boundSphere.position = pos; }
	void UpdateSphereRadius( float rad ) { m_boundSphere.radius = rad; }
	bool IsIgnored() const { return m_ignored; }

	~Collidable();
private:
	BoundSphere m_boundSphere;
	GameObject* m_pObj{ nullptr };
	bool m_destroyed{ false };
	bool m_ignored{ false };
};