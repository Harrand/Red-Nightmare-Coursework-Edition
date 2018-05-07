#include "header.h"
#include "templates.h"
#include "PsyhshTileManager.h"
#include <string>

#include <iostream>

PsyhshTileManager::PsyhshTileManager(int iTileHeight, int iTileWidth, bool default_tree_dead) : TileManager(iTileHeight, iTileWidth), sprite(), sprite_dead(), default_tree_dead(default_tree_dead), tree_statuses()
{
	this->sprite.LoadImage("resources/tree.png");
	this->sprite_dead.LoadImage("resources/tree_dead.png");
}

void PsyhshTileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY)
const
{
	// Invoke user-defined literals for easy string r-value concatenation
	const ImageData& tree_sprite = this->IsTreeDead({iMapX, iMapY}) ? this->sprite_dead : this->sprite;
	tree_sprite.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, tree_sprite.GetWidth(), tree_sprite.GetHeight());
	//using namespace std::string_literals;
	//pEngine->DrawBackgroundString(iStartPositionScreenX, iStartPositionScreenY, ("Tile "s + std::to_string(iMapX + iMapY * this->m_iTileWidth)).c_str(), 0x00aa00, NULL/*BaseEngine::g_pMainFont will be used as NULL (0) is passed. May not work with nullptr so stick with NULL*/);
}

bool PsyhshTileManager::IsTreeDead(Vector2i map_key) const
{
	try
	{
		return this->tree_statuses.at(map_key);
	}
	catch (const std::out_of_range& oore) { this->tree_statuses.insert({map_key, this->default_tree_dead}); return this->default_tree_dead; }
}

void PsyhshTileManager::SetTreeDead(Vector2i map_key, bool dead)
{
	try
	{
		this->tree_statuses.at(map_key) = dead;
	}
	catch (const std::out_of_range& oore) { this->tree_statuses.insert({ map_key, dead }); }
}