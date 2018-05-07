#ifndef PLAYER_H
#define PLAYER_H
#include "ControlledObject.h"
#include "Sprite.h"
#include "JPGImage.h"
#include "Utility.h"

class Player : public ControlledObject, Sprite
{
public:
	Player(BaseEngine* engine, const std::string& sprite_directory, unsigned int max_health = 100, unsigned int health = 100);
	~Player() = default;
	virtual void Draw() override;
	virtual void DoUpdate(int iCurrentTime) override;

	BoundingBox2i GetBoundingBox() const;
	bool IsCasting() const;
	Vector2i GetPosition() const;
	void SetPosition(Vector2i position);
	unsigned int GetMaxHealth() const;
	void SetMaxHealth(unsigned int max_health);
	unsigned int GetHealth() const;
	void SetHealth(unsigned int health);
	bool IsAlive() const;
	void Damage(unsigned int amount);
	void Kill();

	friend class HealthBar;
private:
	int previous_time;
	unsigned int max_health, health, regeneration, damage_timer;
};

#endif