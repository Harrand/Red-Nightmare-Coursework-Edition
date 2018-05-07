#include "header.h"
#include "templates.h"
#include "Sprite.h"

Sprite::Sprite(const std::string& sprite_directory) : sprite_idle(), sprite_left_1(), sprite_left_2(), sprite_right_1(), sprite_right_2(), sprite_special(), sprite_dead(), active_sprite(&sprite_idle)
{
	this->sprite_idle.LoadImage((sprite_directory + "/idle.png").c_str());
	this->sprite_left_1.LoadImage((sprite_directory + "/left_1.png").c_str());
	this->sprite_left_2.LoadImage((sprite_directory + "/left_2.png").c_str());
	this->sprite_right_1.LoadImage((sprite_directory + "/right_1.png").c_str());
	this->sprite_right_2.LoadImage((sprite_directory + "/right_2.png").c_str());
	this->sprite_special.LoadImage((sprite_directory + "/special.png").c_str());
	this->sprite_dead.LoadImage((sprite_directory + "/dead.png").c_str());
}

const ImageData& Sprite::GetActiveSprite() const
{
	return *this->active_sprite;
}

SpriteState Sprite::GetActiveSpriteState() const
{
	if (this->active_sprite == &this->sprite_right_1 || this->active_sprite == &this->sprite_right_2)
		return SpriteState::RIGHT;
	else if (this->active_sprite == &this->sprite_left_1 || this->active_sprite == &this->sprite_left_2)
		return SpriteState::LEFT;
	else if (this->active_sprite == &this->sprite_special)
		return SpriteState::SPECIAL;
	else if (this->active_sprite == &this->sprite_dead)
		return SpriteState::DEAD;
	else
		return SpriteState::IDLE;
}

void Sprite::SetActiveSpriteState(SpriteState state, bool toggle)
{
	switch (state)
	{
	case SpriteState::RIGHT:
		if (toggle)
			this->active_sprite = &this->sprite_right_2;
		else
			this->active_sprite = &this->sprite_right_1;
		break;
	case SpriteState::LEFT:
		if (toggle)
			this->active_sprite = &this->sprite_left_2;
		else
			this->active_sprite = &this->sprite_left_1;
		break;
	case SpriteState::SPECIAL:
		this->active_sprite = &this->sprite_special;
		break;
	case SpriteState::DEAD:
		this->active_sprite = &this->sprite_dead;
		break;
	case SpriteState::IDLE:
	default:
		this->active_sprite = &this->sprite_idle;
		break;
	}
}
