#include "header.h"
#include "templates.h"
#include "Boss.h"
#include "PsyhshEngine.h"

Boss::Boss(BaseEngine* engine, Vector2i position): Creature(engine, "resources/boss", 200, position, 2, 100, std::numeric_limits<int>::max(), false), previous_time(engine->GetTime()), damage_timer(0){}

void Boss::DoUpdate(int iCurrentTime)
{
	if (!this->IsActive())
		return;
	if (this->damage_timer < (iCurrentTime - this->previous_time))
		this->damage_timer = 0;
	else
		this->damage_timer -= iCurrentTime - this->previous_time;
	this->previous_time = iCurrentTime;
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	this->SetTarget(engine->GetPlayer().GetPosition());
	Creature::DoUpdate(iCurrentTime);
	unsigned int current_speed = 2;
	if (engine->GetPlayer().IsAlive() && engine->GetCursor().IsSpecial() && utility::intersects(engine->GetMissile().GetBoundingBox(), this->GetBoundingBox()))
		current_speed--;
	if (engine->GetPlayer().IsAlive() && engine->GetCursor().IsSpecial() && utility::intersects(engine->GetCursor().GetBoundingBox(), this->GetBoundingBox()))
		current_speed--;
	this->speed = current_speed;
	if (this->IsAlive() && engine->GetPlayer().IsAlive() && std::hypot(this->GetPosition().first - engine->GetPlayer().GetPosition().first, this->GetPosition().second - engine->GetPlayer().GetPosition().second) < 20)
	{
		engine->GetPlayer().Damage(15);
		this->Damage(15);
		if(this->IsAlive())
			this->SetPosition({rand() % engine->GetWindowWidth(), rand() % engine->GetWindowHeight()});
	}
	if (!this->IsAlive())
	{
		engine->Complete();
	}
}

void Boss::Draw()
{
	Creature::Draw();
	if (this->IsActive())
	{
		this->m_pEngine->DrawForegroundString(this->m_iCurrentScreenX, this->m_iCurrentScreenY + this->GetActiveSprite().GetHeight(), "Nightmare", 0x000000);
		StoreLastScreenPositionForUndraw();
	}
}

void Boss::Spawn()
{
	this->activation_time = -1;
	this->SetHealth(this->GetMaxHealth());
	this->SetTarget({10, 10});
}

void Boss::Damage(unsigned int amount)
{
	if (this->damage_timer != 0)
		return;
	if (this->GetHealth() <= amount)
		this->Kill();
	else
		this->SetHealth(this->health - amount);
	this->damage_timer = 120;
}