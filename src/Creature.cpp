#include "header.h"
#include "templates.h"
#include "Creature.h"
#include "PsyhshEngine.h"

Creature::Creature(BaseEngine* engine, const std::string& sprite_directory, unsigned int health, const Vector2i& position, unsigned int speed, unsigned int aggro_range, int activation_time, bool draggable, bool collideable): DisplayableObject(engine), Sprite(sprite_directory), max_health(health), health(health), speed(speed), aggro_range(aggro_range), activation_time(activation_time), fleeing(false), draggable(draggable), collideable(collideable), target(position), drag_offset(false, {0, 0})
{
	this->SetPosition(position);
}

void Creature::Draw()
{
	if (!this->IsActive())
		return;
	const ImageData& sprite = this->GetActiveSprite();
	if (sprite.IsLoaded())
		sprite.RenderImageWithMask(this->m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, sprite.GetWidth(), sprite.GetHeight());
	StoreLastScreenPositionForUndraw();
	/* draws the bounding box for debug purposes
	utility::draw_bounding_box(this->GetBoundingBox(), this->m_pEngine, this->health != 0 ? 0xffff00 : 0xff0000);
	StoreLastScreenPositionForUndraw();
	*/
}

void Creature::DoUpdate(int iCurrentTime)
{
	DisplayableObject::DoUpdate(iCurrentTime);
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (engine != nullptr)
	{
		if (!engine->IsPaused())
		{
			if (this->activation_time > iCurrentTime && this->activation_time != -1)
				return;
			else if (this->activation_time <= iCurrentTime)
				this->activation_time = -1;
			if (engine->GetPlayer().IsAlive() && this->draggable && !drag_offset.first && engine->GetCursor().IsSpecial() && utility::intersects(engine->GetCursor().GetBoundingBox(), this->GetBoundingBox()))
			{
				Vector2i offset = { engine->GetCursor().GetXCentre() - this->m_iCurrentScreenX, engine->GetCursor().GetYCentre() - this->m_iCurrentScreenY };
				this->drag_offset = { true, offset };
			}

			if (drag_offset.first && this->IsAlive())
			{
				// if we were being casted on but aren't anymore, then stop.
				if (!engine->GetCursor().IsSpecial())
				{
					this->drag_offset = { false, {0, 0} };
				}
				else
				{
					// we are currently being dragged, update our position as needed.
					Vector2i offset = this->drag_offset.second;
					this->m_iCurrentScreenX = engine->GetCursor().GetXCentre() - offset.first;
					this->m_iCurrentScreenY = engine->GetCursor().GetYCentre() - offset.second;
					this->RedrawWholeScreen();
				}
			}
		}
	}
	
	// handle target chasing
	this->HandleMovement(iCurrentTime);
}

BoundingBox2i Creature::GetBoundingBox() const
{
	const ImageData& sprite = this->GetActiveSprite();
	return { { this->m_iCurrentScreenX, this->m_iCurrentScreenY},{ this->m_iCurrentScreenX + sprite.GetWidth(), this->m_iCurrentScreenY + sprite.GetHeight()} };
}

bool Creature::IsAlive() const
{
	return this->health != 0;
}

void Creature::Kill()
{
	this->health = 0;
	this->SetActiveSpriteState(SpriteState::DEAD);
	this->RedrawWholeScreen();
}

unsigned int Creature::GetMaxHealth() const
{
	return this->max_health;
}

unsigned int Creature::GetHealth() const
{
	return this->health;
}

void Creature::SetHealth(unsigned int health)
{
	this->health = health;
}

void Creature::SetSpeed(unsigned int speed)
{
	this->speed = speed;
}

Vector2i Creature::GetPosition() const
{
	return {this->m_iCurrentScreenX, this->m_iCurrentScreenY};
}

void Creature::SetPosition(const Vector2i& position)
{
	this->m_iCurrentScreenX = position.first;
	this->m_iCurrentScreenY = position.second;
	this->RedrawObjects();
}

Vector2i Creature::GetTarget() const
{
	return this->target;
}

void Creature::SetTarget(Vector2i target)
{
	this->target = target;
}

unsigned int Creature::GetAggroRange() const
{
	return this->aggro_range;
}

void Creature::SetAggroRange(unsigned int aggro_range)
{
	this->aggro_range = aggro_range;
}

void Creature::SetActivationTime(int activation_time)
{
	this->activation_time = activation_time;
}

bool Creature::IsActive() const
{
	return this->activation_time == -1;
}

bool Creature::IsFleeing() const
{
	return this->fleeing;
}

void Creature::SetFleeing(bool fleeing)
{
	this->fleeing = fleeing;
}

void Creature::HandleMovement(int iCurrentTime)
{
	if (this->health == 0)
		return;
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (!engine->IsPaused())
	{
		bool frame_toggle = iCurrentTime % 100 <= 50;
		if (this->GetPosition().first < this->GetTarget().first)
		{
			bool allow = true;
			for (Creature* creature : engine->GetCreatures())
			{
				if (creature == this || !creature->collideable || !creature->IsAlive() || !creature->IsActive())
					continue;
				if (std::hypot(this->GetPosition().first + this->speed - creature->GetPosition().first, this->GetPosition().second - creature->GetPosition().second) <= 20)
				{
					allow = false;
					break;
				}
			}
			if (allow || !this->collideable)
			{
				if (this->GetPosition().first + this->speed >= this->GetTarget().first)
					this->m_iCurrentScreenX = this->GetTarget().first;
				else
					this->m_iCurrentScreenX += this->speed;
				this->SetActiveSpriteState(SpriteState::RIGHT, frame_toggle);
			}
			else
				this->SetActiveSpriteState(SpriteState::IDLE);
		}
		else if (this->GetPosition().first > this->GetTarget().first)
		{
			bool allow = true;
			for (Creature* creature : engine->GetCreatures())
			{
				if (creature == this || !creature->collideable || !creature->IsAlive() || !creature->IsActive())
					continue;
				if (std::hypot(this->GetPosition().first - this->speed - creature->GetPosition().first, this->GetPosition().second - creature->GetPosition().second) <= 20)
				{
					allow = false;
					break;
				}
			}
			if (allow || !this->collideable)
			{
				if (this->GetPosition().first - this->speed <= this->GetTarget().first)
					this->m_iCurrentScreenX = this->GetTarget().first;
				else
					this->m_iCurrentScreenX -= this->speed;
				this->SetActiveSpriteState(SpriteState::LEFT, frame_toggle);
			}
			else
				this->SetActiveSpriteState(SpriteState::IDLE);
		}
		else
			this->SetActiveSpriteState(SpriteState::IDLE);

		if (this->GetPosition().second < this->GetTarget().second)
		{
			bool allow = true;
			for (Creature* creature : engine->GetCreatures())
			{
				if (creature == this || !creature->collideable || !creature->IsAlive() || !creature->IsActive())
					continue;
				if (std::hypot(this->GetPosition().first - creature->GetPosition().first, this->GetPosition().second + this->speed - creature->GetPosition().second) <= 20)
				{
					allow = false;
					break;
				}
			}
			if (allow || this->collideable)
			{
				if (this->GetPosition().second + this->speed >= this->GetTarget().second)
					this->m_iCurrentScreenY = this->GetTarget().second;
				else
					this->m_iCurrentScreenY += this->speed;
			}
			else
				this->SetActiveSpriteState(SpriteState::IDLE);
		}
		else if (this->GetPosition().second > this->GetTarget().second)
		{
			bool allow = true;
			for (Creature* creature : engine->GetCreatures())
			{
				if (creature == this || !creature->collideable || !creature->IsAlive() || !creature->IsActive())
					continue;
				if (std::hypot(this->GetPosition().first - creature->GetPosition().first, this->GetPosition().second - this->speed - creature->GetPosition().second) <= 20)
				{
					allow = false;
					break;
				}
			}
			if (allow || this->collideable)
			{
				if (this->GetPosition().second - this->speed <= this->GetTarget().second)
					this->m_iCurrentScreenY = this->GetTarget().second;
				else
					this->m_iCurrentScreenY -= this->speed;
			}
			else
				this->SetActiveSpriteState(SpriteState::IDLE);
		}
	}
	this->RedrawWholeScreen();
}