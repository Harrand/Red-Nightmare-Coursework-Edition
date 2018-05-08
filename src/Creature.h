#ifndef CREATURE_H
#define CREATURE_H
#include "Sprite.h"

class Creature: public DisplayableObject, public Sprite
{
public:
	Creature(BaseEngine* engine, const std::string& sprite_directory, unsigned int health = 1, const Vector2i& position = {0, 0}, unsigned int speed = utility::DEFAULT_SPEED, unsigned int aggro_range = utility::DEFAULT_AGGRO_RANGE, int activation_time = -1, bool draggable = true, bool collideable = true);
	~Creature() = default;
	virtual void Draw() override;
	virtual void DoUpdate(int iCurrentTime) override;

	friend class HealthBar;

	BoundingBox2i GetBoundingBox() const;
	bool IsAlive() const;
	void Kill();
	unsigned int GetMaxHealth() const;
	unsigned int GetHealth() const;
	void SetHealth(unsigned int health);
	void SetSpeed(unsigned int speed);
	Vector2i GetPosition() const;
	void SetPosition(const Vector2i& position);
	Vector2i GetTarget() const;
	void SetTarget(Vector2i target);
	unsigned int GetAggroRange() const;
	void SetAggroRange(unsigned int range);
	void SetActivationTime(int activation_time);
	bool IsActive() const;
	bool IsFleeing() const;
	void SetFleeing(bool fleeing);
protected:
	void HandleMovement(int iCurrentTime);
	unsigned int max_health, health;
	unsigned int speed;
	unsigned int aggro_range;
	int activation_time;
	bool fleeing;
	bool draggable;
	bool collideable;
	Vector2i target;
	/// No C++17 so no optional vector2i. just have a pair<bool, vector2i> instead
	std::pair<bool, Vector2i> drag_offset;
};

#endif

