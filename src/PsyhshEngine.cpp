#include "header.h"
#include "templates.h"
#include "PsyhshEngine.h"
#include "HealthBar.h"
#include "Stage.h"
#include "HUD.h"

PsyhshEngine::PsyhshEngine() : player(new Player(this, "resources/phoenix")), cursor(new Cursor(this)), creatures(), player_missile(new Missile(this, this->player->GetPosition())), boss(new Boss(this, {200, 200})), tile_manager(300, 300), score(0), stage(0), paused(false), completed(false) {}

void PsyhshEngine::SetupBackgroundBuffer()
{
	SDL_ShowCursor(SDL_DISABLE);
	this->FillBackground(0x001000);
	//this->DrawBackgroundOval(0, 0, 200, 200, 0xff0000);
	this->tile_manager.SetBaseTilesPositionOnScreen(0, 0);
	this->tile_manager.DrawAllTiles(this, this->GetBackground(), 0, 0, 3, 1);
}

int PsyhshEngine::InitialiseObjects()
{
	//this->creatures.push_back(new Creature(this, "resources/zombie", 1, {400, 300}, 1, 150, -1));
	//this->creatures.push_back(new Creature(this, "resources/zombie", 1, {600,50}, 1, 150, this->GetTime() + 5000));
	for (std::size_t i = 0; i < utility::NUMBER_OF_CREATURES; i++)
	{
		this->creatures.push_back(new Creature(this, "resources/zombie", 1, { rand() % this->GetWindowWidth(), rand() % this->GetWindowHeight() }, 1 + (rand() % 2), 100 + (rand() % 300), std::numeric_limits<int>::max()));
		this->creatures.back()->SetHealth(0);
	}
	this->NextStage();
	DrawableObjectsChanged();
	// Destroy any existing objects
	DestroyOldObjects();
	// Creates an array to store the objects
	// Needs to have room for the NULL at the end
	// Number of objects = creature_vector.size + 1 for the cursor and + 1 for the player and + 1 for the missile and + 1 for the HUD and + 1 for the boss and +1 for the boss healthbar
	CreateObjectArray(this->creatures.size() + 7);
	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	//StoreObjectInArray(1, new ControlledObject(this));
	std::size_t counter = 0;
	HealthBar* player_health = new HealthBar(this);
	player_health->AttachPositionTo(*this->player);
	HealthBar* boss_health = new HealthBar(this);
	boss_health->AttachPositionTo(*this->boss);
	StoreObjectInArray(counter++, this->boss);
	StoreObjectInArray(counter++, boss_health);
	for (Creature* creature_ptr : this->creatures)
		StoreObjectInArray(counter++, creature_ptr);
	StoreObjectInArray(counter++, this->player);
	StoreObjectInArray(counter++, player_health);
	StoreObjectInArray(counter++, this->player_missile);
	StoreObjectInArray(counter++, this->cursor);
	StoreObjectInArray(counter++, new HUD(this));
	StoreObjectInArray(counter++, NULL);
	return 0;
}

Player& PsyhshEngine::GetPlayer() const
{
	return *this->player;
}

const Cursor& PsyhshEngine::GetCursor() const
{
	return *this->cursor;
}

Missile& PsyhshEngine::GetMissile() const
{
	return *this->player_missile;
}

Boss& PsyhshEngine::GetBoss() const
{
	return *this->boss;
}

const std::vector<Creature*>& PsyhshEngine::GetCreatures() const
{
	return this->creatures;
}

unsigned int PsyhshEngine::GetScore() const
{
	return this->score;
}

void PsyhshEngine::AddScore(unsigned int amount)
{
	this->score += amount;
}

int PsyhshEngine::GetStage() const
{
	return this->stage;
}

void PsyhshEngine::LoadStage(const std::string& stage)
{
	unsigned int background = Stage(stage).Load(*this);
	this->KillTree({ this->stage - 1, 0 });
	this->KillTree({ this->stage++ - 1, 1 });
	this->FillBackground(background);
	this->tile_manager.DrawAllTiles(this, this->GetBackground(), 0, 0, 3, 1);
}

unsigned int PsyhshEngine::GetNumberOfAliveCreatures() const
{
	unsigned int counter = 0;
	for (const auto& creature : this->creatures)
		if (creature->IsAlive())
			counter++;
	return counter;
}

void PsyhshEngine::NextStage()
{
	using namespace std::string_literals;
	std::string filename = "resources/stage_"s + std::to_string(this->stage + 1) + ".txt"s;
	auto exists_file = [&filename]()->bool {return std::ifstream(filename).good(); };
	if (exists_file())
		this->LoadStage(filename);
	else
		this->GetBoss().Spawn();
}

void PsyhshEngine::RespawnAllCreatures()
{
	for (Creature* creature : this->creatures)
		creature->SetHealth(1);
}

bool PsyhshEngine::IsPaused() const
{
	return this->paused;
}

void PsyhshEngine::SetPaused(bool paused)
{
	this->paused = paused;
}

bool PsyhshEngine::IsCompleted() const
{
	return this->completed;
}

void PsyhshEngine::Complete()
{
	this->completed = true;
}

void PsyhshEngine::KillTree(Vector2i map_key)
{
	this->tile_manager.SetTreeDead(map_key, true);
}