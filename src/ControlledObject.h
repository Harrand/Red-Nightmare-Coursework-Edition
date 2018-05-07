#ifndef CONTROLLED_OBJECT_H
#define CONTROLLED_OBJECT_H
#include "DisplayableObject.h"
#include <functional>

class ControlledObject : public DisplayableObject
{
public:
	ControlledObject(BaseEngine* engine);
	~ControlledObject() = default;

	static constexpr int UP_KEY = SDLK_w;
	static constexpr int DOWN_KEY = SDLK_s;
	static constexpr int RIGHT_KEY = SDLK_d;
	static constexpr int LEFT_KEY = SDLK_a;

	virtual void Draw() override;
	virtual void DoUpdate(int iCurrentTime) override;
	bool IsMovingUp() const;
	bool IsMovingDown() const;
	bool IsMovingRight() const;
	bool IsMovingLeft() const;
	bool IsMoving() const;
protected:
	bool can_move;
};

#endif

