#include "header.h"
#include "templates.h"
#include "Player.h"
#include "Cursor.h"
#include "PsyhshEngine.h"

Player::Player(BaseEngine* engine, const std::string& sprite_directory, unsigned int max_health, unsigned int health) : ControlledObject(engine), Sprite(sprite_directory), previous_time(engine->GetTime()), max_health(max_health), health(health), regeneration(0), damage_timer(0)
{
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = 100;
	m_iCurrentScreenY = m_iPreviousScreenY = 100;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = this->GetActiveSprite().GetWidth();
	m_iDrawHeight = this->GetActiveSprite().GetHeight();
	// And make it visible
	SetVisible(true);
}

void Player::Draw()
{
	const ImageData& sprite = this->GetActiveSprite();
	if(sprite.IsLoaded())
		sprite.RenderImageWithMask(this->m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, sprite.GetWidth(), sprite.GetHeight());
	StoreLastScreenPositionForUndraw();
	/* draws the bounding box for debug purposes
	utility::draw_bounding_box(this->GetBoundingBox(), this->m_pEngine);
	StoreLastScreenPositionForUndraw();
	*/
}

void Player::DoUpdate(int iCurrentTime)
{
	ControlledObject::DoUpdate(iCurrentTime);

	this->regeneration += iCurrentTime - this->previous_time;
	if (this->damage_timer < (iCurrentTime - this->previous_time))
		this->damage_timer = 0;
	else
		this->damage_timer -= iCurrentTime - this->previous_time;
	this->previous_time = iCurrentTime;

	if (!this->IsAlive())
	{
		this->SetActiveSpriteState(SpriteState::DEAD);
		this->can_move = false;
		return;
	}

	bool frame_toggle = iCurrentTime % 100 <= 50;

	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (engine != nullptr)
	{
		if (engine->IsPaused())
			return;
		if (this->IsMovingRight())
			this->SetActiveSpriteState(SpriteState::RIGHT, frame_toggle);
		else if (this->IsMovingLeft())
			this->SetActiveSpriteState(SpriteState::LEFT, frame_toggle);
		else if (this->IsCasting())
			this->SetActiveSpriteState(SpriteState::SPECIAL);
		else
			this->SetActiveSpriteState(SpriteState::IDLE);

		if (this->regeneration >= 500)
		{
			if (++this->health > this->GetMaxHealth())
				this->health = this->GetMaxHealth();
			this->regeneration = 0;
		}
		if (engine->GetNumberOfAliveCreatures() == 0 && !engine->GetBoss().IsActive())
		{
			this->SetHealth(this->GetMaxHealth());
			engine->NextStage();
		}
		Missile& missile = engine->GetMissile();
		if (Cursor::MouseRightClicked(*engine))
			missile.Shoot(Cursor::GetMousePosition(*engine));
		if (!engine->IsCompleted() && utility::intersects(missile.GetBoundingBox(), this->GetBoundingBox()))
			this->Damage(5);

		/// test. make all creatures chase player.
		for (Creature* creature_ptr : engine->GetCreatures())
		{
			if (!creature_ptr->IsActive())
				continue;
			if (creature_ptr->IsAlive() && std::hypot(this->GetPosition().first - creature_ptr->GetPosition().first, this->GetPosition().second - creature_ptr->GetPosition().second) < 20)
			{
				this->Damage(10);
			}
			if (!creature_ptr->IsFleeing() && std::hypot(creature_ptr->GetPosition().first - this->GetPosition().first, creature_ptr->GetPosition().second - this->GetPosition().second) <= creature_ptr->GetAggroRange())
				creature_ptr->SetTarget(this->GetPosition());
		}
	}
}

BoundingBox2i Player::GetBoundingBox() const
{
	const ImageData& sprite = this->GetActiveSprite();
	return { { this->m_iCurrentScreenX, this->m_iCurrentScreenY},{ this->m_iCurrentScreenX + sprite.GetWidth(), this->m_iCurrentScreenY + sprite.GetHeight() } };
}

bool Player::IsCasting() const
{
	return Cursor::MouseLeftClicked(*this->m_pEngine) && !this->IsMoving();
}

Vector2i Player::GetPosition() const
{
	return {this->m_iCurrentScreenX, this->m_iCurrentScreenY};
}

void Player::SetPosition(Vector2i position)
{
	this->m_iCurrentScreenX = position.first;
	this->m_iCurrentScreenY = position.second;
}

unsigned int Player::GetMaxHealth() const
{
	return this->max_health;
}

void Player::SetMaxHealth(unsigned int max_health)
{
	this->max_health = max_health;
}

unsigned int Player::GetHealth() const
{
	return this->health;
}

void Player::SetHealth(unsigned int health)
{
	this->health = health;
}

bool Player::IsAlive() const
{
	return this->health != 0;
}

void Player::Damage(unsigned int amount)
{
	if (this->damage_timer != 0)
		return;
	if (this->GetHealth() <= amount)
		this->Kill();
	else
		this->SetHealth(this->health - amount);
	this->damage_timer = 120;
}

void Player::Kill()
{
	this->health = 0;
}
