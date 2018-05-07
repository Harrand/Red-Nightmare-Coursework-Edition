#ifndef HEALTHBAR_H
#define HEALTHBAR_H
#include "Player.h"
#include "Creature.h"

class HealthBar : public DisplayableObject
{
public:
	HealthBar(BaseEngine* engine);
	~HealthBar() = default;

	virtual void DoUpdate(int iCurrentTime) override;
	virtual void Draw() override;
	void AttachPositionTo(const Player& player);
	void AttachPositionTo(const Creature& creature);
private:
	const Player* player_attachment;
	const Creature* creature_attachment;
	float percentage;
};

#endif

