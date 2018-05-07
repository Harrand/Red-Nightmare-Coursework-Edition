#include "header.h"
#include "templates.h"
#include "HUD.h"
#include "PsyhshEngine.h"
#include <string>

HUD::HUD(BaseEngine* engine): DisplayableObject(engine)
{
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = 0;
	m_iCurrentScreenY = m_iPreviousScreenY = 0;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = engine->GetWindowWidth();
	m_iDrawHeight = engine->GetWindowHeight();
	// And make it visible
	SetVisible(true);
}

void HUD::Draw()
{
	PsyhshEngine* engine = dynamic_cast<PsyhshEngine*>(this->m_pEngine);
	if (engine == nullptr)
		return;
	using namespace std::string_literals;
	this->m_pEngine->DrawForegroundString(0, 0, ("Stage "s + std::to_string(engine->GetStage())).c_str(), 0xffffff);
	this->m_pEngine->DrawForegroundString(0, 50, ("Ghosts: "s + std::to_string(engine->GetNumberOfAliveCreatures())).c_str(), 0xffffff);
	this->m_pEngine->DrawForegroundString(0, 100, ("Score: "s + std::to_string(engine->GetScore())).c_str(), 0xffffff);
	if(engine->IsPaused())
		this->m_pEngine->DrawForegroundString(0, 150, "Paused", 0xaaaa00);
	if (!engine->GetPlayer().IsAlive())
	{
		this->m_pEngine->DrawForegroundString(this->m_pEngine->GetWindowWidth() / 2.0f - 100, this->m_pEngine->GetWindowHeight() / 2.0f, "You died", 0xff0000);
	}
	if (engine->IsCompleted())
	{
		this->m_pEngine->DrawForegroundString(this->m_pEngine->GetWindowWidth() / 2.0f - 100, this->m_pEngine->GetWindowHeight() / 2.0f, "You win!", 0x00ff00);
	}
	StoreLastScreenPositionForUndraw();
}