#ifndef BOSS_H
#define BOSS_H
#include "Creature.h"

class Boss : public Creature
{
public:
	Boss(BaseEngine* engine, Vector2i position);
	~Boss() = default;
	virtual void DoUpdate(int iCurrentTime) override;
	virtual void Draw() override;
	void Spawn();
	void Damage(unsigned int amount);
private:
	int previous_time;
	unsigned int damage_timer;
};

#endif