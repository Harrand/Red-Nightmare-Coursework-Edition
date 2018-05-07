#ifndef PSYHSH_ENGINE_H
#define PSYHSH_ENGINE_H
#include "BaseEngine.h"
#include "PsyhshTileManager.h"
#include "Player.h"
#include "Cursor.h"
#include "Missile.h"
#include "Boss.h"
#include <vector>

class PsyhshEngine : public BaseEngine
{
public:
	PsyhshEngine();
	~PsyhshEngine() = default;

	virtual void SetupBackgroundBuffer() override;
	virtual int InitialiseObjects() override;
	Player& GetPlayer() const;
	const Cursor& GetCursor() const;
	Missile& GetMissile() const;
	Boss& GetBoss() const;
	const std::vector<Creature*>& GetCreatures() const;
	unsigned int GetScore() const;
	void AddScore(unsigned int amount);
	int GetStage() const;
	void LoadStage(const std::string& stage);
	std::size_t GetNumberOfAliveCreatures() const;
	void NextStage();
	void RespawnAllCreatures();
	bool IsPaused() const;
	void SetPaused(bool paused);
	bool IsCompleted() const;
	void Complete();

	void KillTree(Vector2i map_key);
private:
	/// All of these raw pointers are (grudgingly) owning. For coursework purposes, assume their lifetime is handled perfectly.
	Player* player;
	Cursor* cursor;
	Missile* player_missile;
	Boss* boss;
	std::vector<Creature*> creatures;
	PsyhshTileManager tile_manager;
	unsigned int score;
	int stage;
	bool paused;
	bool completed;
};

#endif