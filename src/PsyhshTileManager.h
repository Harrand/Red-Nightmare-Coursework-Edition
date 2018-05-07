#ifndef PSYHSH_TILE_MANAGER_H
#define PSYHSH_TILE_MANAGER_H
#include "TileManager.h"
#include "JPGImage.h"
#include "Utility.h"
#include <map>

class PsyhshTileManager : public TileManager
{
public:
	PsyhshTileManager(int iTileHeight, int iTileWidth, bool default_tree_dead = false);
	~PsyhshTileManager() = default;

	virtual void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;
	bool IsTreeDead(Vector2i map_key) const;
	void SetTreeDead(Vector2i map_key, bool dead);
private:
	ImageData sprite, sprite_dead;
	bool default_tree_dead;
	/// TileManager::DrawTileAt is const but shouldn't be in this case. Nasty hack to get around it.
	mutable std::map<Vector2i, bool> tree_statuses;
};

#endif