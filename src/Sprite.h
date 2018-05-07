#ifndef SPRITE_H
#define SPRITE_H
#include "DisplayableObject.h"
#include "JPGImage.h"
#include "Utility.h"
#include <string>

enum class SpriteState : unsigned int
{
	IDLE = 0,
	RIGHT = 1,
	LEFT = 2,
	DEAD = 3,
	SPECIAL = 4,
};

class Sprite
{
public:
	Sprite(const std::string& sprite_directory);
	~Sprite() = default;
	const ImageData& GetActiveSprite() const;
	SpriteState GetActiveSpriteState() const;
	void SetActiveSpriteState(SpriteState state, bool toggle = false);
protected:
	ImageData sprite_idle, sprite_right_1, sprite_right_2, sprite_left_1, sprite_left_2, sprite_special, sprite_dead;
	ImageData* active_sprite;
};

#endif

