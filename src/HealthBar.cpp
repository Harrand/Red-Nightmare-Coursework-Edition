#include "header.h"
#include "templates.h"
#include "HealthBar.h"

HealthBar::HealthBar(BaseEngine* engine) : DisplayableObject(engine), player_attachment(nullptr), percentage(100)
{
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = 100;
	m_iCurrentScreenY = m_iPreviousScreenY = 100;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the ball size as both height and width
	m_iDrawWidth = 128;
	m_iDrawHeight = 10;
	// And make it visible
	SetVisible(true);
}

void HealthBar::DoUpdate(int iCurrentTime)
{
	if (this->player_attachment != nullptr)
	{
		this->m_iCurrentScreenX = this->player_attachment->m_iCurrentScreenX;
		this->m_iCurrentScreenY = this->player_attachment->m_iCurrentScreenY + this->player_attachment->m_iDrawHeight;
		if (this->player_attachment->IsAlive())
			this->percentage = 100.0f * this->player_attachment->GetHealth() / this->player_attachment->GetMaxHealth();
		else
			this->percentage = 0;
		this->RedrawObjects();
	}
	else if (this->creature_attachment != nullptr)
	{
		this->m_iCurrentScreenX = this->creature_attachment->m_iCurrentScreenX;
		this->m_iCurrentScreenY = this->creature_attachment->m_iCurrentScreenY + this->creature_attachment->m_iDrawHeight;
		if (this->creature_attachment->IsAlive())
			this->percentage = 100.0f * this->creature_attachment->GetHealth() / this->creature_attachment->GetMaxHealth();
		else
			this->percentage = 0;
		this->RedrawObjects();
	}
}

void HealthBar::Draw()
{
	if (this->creature_attachment != nullptr && !this->creature_attachment->IsActive())
		return;
	this->m_pEngine->DrawForegroundRectangle(this->m_iCurrentScreenX, this->m_iCurrentScreenY, this->m_iCurrentScreenX + this->m_iDrawWidth, this->m_iCurrentScreenY + this->m_iDrawHeight, 0xffffff);
	if(this->percentage > 0)
		this->m_pEngine->DrawForegroundRectangle(this->m_iCurrentScreenX + 2, this->m_iCurrentScreenY + 2, this->m_iCurrentScreenX - 2 + (this->m_iDrawWidth * this->percentage / 100.0f), this->m_iCurrentScreenY + this->m_iDrawHeight - 2, 0x00ff00);
	this->m_pEngine->DrawForegroundString(this->m_iCurrentScreenX + (this->m_iDrawWidth / 2.0f) - 5, this->m_iCurrentScreenY + this->m_iDrawHeight + 2, std::to_string(static_cast<int>(this->percentage)).c_str(), 0xffffff);
	StoreLastScreenPositionForUndraw();
}

void HealthBar::AttachPositionTo(const Player& player)
{
	this->player_attachment = &player;
}

void HealthBar::AttachPositionTo(const Creature& creature)
{
	this->creature_attachment = &creature;
}
