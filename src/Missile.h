#ifndef MISSILE_H
#define MISSILE_H
#include "Creature.h"

class Missile : public Creature
{
public:
	Missile(BaseEngine* engine, Vector2i position);
	~Missile() = default;
	virtual void DoUpdate(int iCurrentTime) override;
	void Shoot(Vector2i to);
};

#endif