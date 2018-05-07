#include "header.h"
#include "templates.h"
#include "Missile.h"
#include "PsyhshEngine.h"

Missile::Missile(BaseEngine* engine, Vector2i position): Creature(engine, "resources/missile", 1, position, 1, 0, -1, false)
{
	this->SetTarget(this->GetPosition());
	//this->activation_time = std::numeric_limits<int>::max();
}

void Missile::DoUpdate(int iCurrentTime)
{
	Creature::DoUpdate(iCurrentTime);
	if (!this->IsActive())
		return;
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (engine != nullptr)
	{
		for (Creature* creature : engine->GetCreatures())
		{
			if (!creature->IsActive() || !creature->IsAlive())
				continue;
			const Vector2i displacement = { this->GetPosition().first - creature->GetPosition().first, this->GetPosition().second - creature->GetPosition().second };
			if (utility::intersects(this->GetBoundingBox(), creature->GetBoundingBox()))
			{
				creature->Kill();
				engine->AddScore(10);
			}
			else if(!engine->GetCursor().IsSpecial() && std::hypot(displacement.first, displacement.second) <= creature->GetAggroRange() / 2.0f) // if the missile is less than 50 units away from the creature, make it run away
			{
				Vector2i run_target = {creature->GetPosition().first - displacement.first, creature->GetPosition().second - displacement.second };
				//Vector2i previous_target = creature->GetTarget();
				creature->SetFleeing(true);
				creature->SetTarget(run_target/*{ run_target.first + previous_target.first / 2.0f, run_target.second + previous_target.second / 2.0f }*/);
			}
			else
				creature->SetFleeing(false);
		}
	}
}

void Missile::Shoot(Vector2i to)
{
	this->activation_time = -1;
	this->SetTarget(to);
}

